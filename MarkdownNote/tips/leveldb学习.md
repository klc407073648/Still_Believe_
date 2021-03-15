- 使用方法

```
1.使用方法，make 生成so文件
2.g++ main.cpp -o main -I./include -lpthread -L. -lleveldb 生成可执行文件
3.testdb文件夹是数据库相关的内容


LevelDB是Google开源的持久化KV单机数据库，具有很高的随机写，顺序读/写性能，但是随机读的性能很一般，也就是说，LevelDB很适合应用在查询较少，而写很多的场景。
LevelDB应用了LSM (Log Structured Merge) 策略，lsm_tree对索引变更进行延迟及批量处理，并通过一种类似于归并排序的方式高效地将更新迁移到磁盘，降低索引插入开销，
关于LSM，本文在后面也会简单提及。


参考资料：
https://blog.csdn.net/qq_26222859/article/details/79645203
https://blog.csdn.net/swartz2015/article/details/66970885
https://github.com/google/leveldb

VS code  跳转和回退 ctrl+Z alt+→
```


- Slice

```
levelDB中Slice非常简单的数据结构，它包括一个指向外部字节数组的指针和size。
相对于返回string而言，返回Slice的开销会小的多（没有拷贝（数据），Slice中没有实际数据，只有指向数据的指针，开销低，因为拷贝也只是拷贝了指针而已）
```

- Status

```
 // OK status has a NULL state_.  Otherwise, state_ is a new[] array
  // of the following form:
  //    state_[0..3] == length of message
  //    state_[4]    == code
  //    state_[5..]  == message
  const char* state_;
  
  因此，CopyState里
  const char* Status::CopyState(const char* state) {
  uint32_t size;
  memcpy(&size, state, sizeof(size));//获取msg的length
  char* result = new char[size + 5];//+5,前5位是msg的length和code
  memcpy(result, state, size + 5);
  return result;
}

  enum Code {
    kOk = 0,
    kNotFound = 1,
    kCorruption = 2,
    kNotSupported = 3,
    kInvalidArgument = 4,
    kIOError = 5
  };
```

- Iterator

```
Iterator用确定的遍历接口将上层需求和下层实现解耦和。熟悉STL的同学一定不会陌生Iterator的使用方式，这里LevelDB的Iterator接口包括：

1.Seek到某一位置：Seek，SeekToFirst，SeekToLast；
2.访问前驱后继：Next，Prev；
3.判断当前位置是否有效：Valid；
4.获取当前位置数据信息：key，value，status；
5.可以注册多个Cleanup方法，当Iterator析构前做一些清理操作。


https://www.jianshu.com/p/7fe24a77002a

```

- Env

```
levelDB考虑到移植性问题，将系统相关的处理(文件/进程/时间之类)抽象成Env ，用户可以自己实现相应的接口，作为Options传入，默认使用自带的实现。

用户没有定义Env时，会默认使用PosixEnv。
可以通过传递给EnvWrapper，以PosixEnv值，使得最后调用的都是PosixEnv的成员函数。

http://www.360doc.com/content/14/0325/16/15064667_363619343.shtml
```

- table

```
Table也叫SSTable（Sorted String Table），是数据在.sst文件中的存储形式。Table的逻辑结构如下所示，包括存储数据的Block，存储索引信息的Block，存储Filter的Block：

1.Footer：为于Table尾部，记录指向Metaindex Block的Handle和指向Index Block的Handle。需要说明的是Table中所有的Handle是通过偏移量Offset以及Size一同来表示的，用来指明所指向的Block位置。Footer是SST文件解析开始的地方，通过Footer中记录的这两个关键元信息Block的位置，可以方便的开启之后的解析工作。另外Footer种还记录了用于验证文件是否为合法SST文件的常数值Magicnum。
2.Index Block：记录Data Block位置信息的Block，其中的每一条Entry指向一个Data Block，其Key值为所指向的Data Block最后一条数据的Key，Value为指向该Data Block位置的Handle。
3.Metaindex Block：与Index Block类似，由一组Handle组成，不同的是这里的Handle指向的Meta Block。
4.Data Block：以Key-Value的方式存储实际数据，其中Key定义为：
    DataBlock Key := UserKey + SequenceNum + Type
    Type := kDelete or kValue
对比Memtable中的Key，可以发现Data Block中的Key并没有拼接UserKey的长度在UserKey前，这是由于上面讲到的物理结构中已经有了Key的长度信息。
5.Meta Block：比较特殊的Block，用来存储元信息，目前LevelDB使用的仅有对布隆过滤器的存储。写入Data Block的数据会同时更新对应Meta Block中的过滤器。读取数据时也会首先经过布隆过滤器（Bloom Filter）过滤，我看的源码还未用到Bloom Filter，可参考：BloomFilter——大规模数据处理利器。Meta Block的物理结构也与其他Block有所不同：


https://blog.csdn.net/caoshangpa/article/details/79021568
```

- block

```
        unsigned char[]    1byte        4byte
block = block data      +  type    +    crc32

1.block data
block data是具体的KV对存储区域。虽然Block有好几种，但是block data都是有序的KV对，因此写入、读取block data的接口都是统一的。
2.type
type指明使用的是哪种压缩方式，当前支持none和snappy压缩。
3.crc32
数据校验位:LevelDB对block data的管理是读写分离的，读取后的遍历查询操作由Block类实现，block data的构建则由BlockBuilder类实现。

block data可分为KV存储区和重启点信息存储区两部分。
假设每一个KV对是一条记录（Record），则记录的格式如下。
——共享前缀长度                     shared_bytes:    varint32
——前缀之后的字符串长度             unshared_bytes:  varint32
——值的长度                         value_length:    varint32
——前缀之后的字符串                 key_delta:       char[unshared_bytes]
——值                               value:           char[value_length]
对于重启点而言，shared_bytes = 0，重启点存储完整的key。
block data的结尾段格式是：
——restarts:      uint32[num_restarts]
——num_restarts:  uint32

https://blog.csdn.net/caoshangpa/article/details/78977743
```

- comparator

```
这里的comparator包括抽象类Comparator及其两个实现类：一个是内置的BytewiseComparatorImpl，另一个是InternalKeyComparator。

BytewiseComparatorImpl是按字典逐字节序进行比较的，也就是说i>helloworld，因为先比较i和h，i>h，比较结束。

  // 直接调用Slice的compare函数
  virtual int Compare(const Slice& a, const Slice& b) const {
    return a.compare(b);
  }

Internal Key：
下面是一个未编码前，或者说是一个解码后的Internal Key结构，它由user_key、sequence和type三个字段组成。

Slice user_key;
SequenceNumber sequence;
ValueType type;

//SequenceNumber 预留8位用于存储 ValueType
AppendInternalKey 中的 PackSequenceAndType就是将sequence和type打包

typedef uint64_t SequenceNumber;
// We leave eight bits empty at the bottom so a type and sequence#
// can be packed together into 64-bits.

inline bool ParseInternalKey(const Slice& internal_key,
                             ParsedInternalKey* result) {
  const size_t n = internal_key.size();
  if (n < 8) return false;
  // DecodeFixed64函数是PutFixed64函数的逆过程，返回sequence和type的打包结果，并赋值给num。
  uint64_t num = DecodeFixed64(internal_key.data() + n - 8);
  // 截取低8位，赋值给c
  unsigned char c = num & 0xff;
  // 右移8位，赋值给sequence
  result->sequence = num >> 8;
  // 将c转换为type
  result->type = static_cast<ValueType>(c);
  // 取出user_key
  result->user_key = Slice(internal_key.data(), n - 8);
  return (c <= static_cast<unsigned char>(kTypeValue));
}

https://blog.csdn.net/caoshangpa/article/details/78835561
```

- codeing

```
LevelDB默认使用的是小端字节序存储，低位字节排放在内存的低地址端，高位字节排放在内存的高地址端。 
编码分为变长的EncodeVarint和固定大小的EncodeFixed两种，每种又分32位和64位。


void EncodeFixed32(char* buf, uint32_t value) {
#if __BYTE_ORDER == __LITTLE_ENDIAN
  memcpy(buf, &value, sizeof(value));
#else
  buf[0] = value & 0xff;
  buf[1] = (value >> 8) & 0xff;
  buf[2] = (value >> 16) & 0xff;
  buf[3] = (value >> 24) & 0xff;
#endif
}
功能就是判断系统是否为小端存储，如果是，直接将value拷贝到buf，如果不是，则将value转换为小端字节序存储到buf中。
解码函数DecodeFixed32和DecodeFixed64是编码的逆过程，也很简单。

EncodeVarint:
为什么要把整型（int）编码成变长整型（varint）呢？是为了尽可能的节约存储空间。
varint是一种紧凑的表示数字的方法，它用一个或多个字节来表示一个数字，值越小的数字使用越少的字节数。
比如int32类型的数字，一般需要4个字节。但是采用Varint，对于很小的int32类型的数字，则可以用1个字节来表示。当然凡事都有好的也有不好的一面，采用varint表示法，大的数字则可能需要5个字节来表示。从统计的角度来说，一般不会所有消息中的数字都是大数，因此大多数情况下，采用varint后，可以用更小的字节数来表示数字信息。
varint中的每个字节的最高位（bit）有特殊含义，如果该位为1，表示后续的字节也是这个数字的一部分，如果该位为0，则结束。其他的7位（bit）都表示数字。7位能表示的最大数是127，因此小于128的数字都可以用一个字节表示。大于等于128的数字，比如说300，会用两个字节在内存中表示为：
低               高
1010 1100 0000 0010
实现过程如下：
300的二进制为100101100，取低7位也就是010 1100放在内存低字节中，由于第二个字节也是数字的一部分，因此内存低字节的最高位为1，则完整的内存低字节为1010 1100。300的高2位也就是10放到内存的高字节中，因为数字到该字节结束，因此该字节包括最高位的其他6位都用0填充，则完整的内存高字节为0000 0010。

https://blog.csdn.net/caoshangpa/article/details/78815940
```

- cache

```
Cache为接口类。ShardedLRUCache继承自Cache，实现了Cache中的缓存操作方法。
ShardedLRUCache封装了16个LRUCache缓存片，每次对缓存的读取、插入、查找、删除操作都是调用某个缓存片LRUCache中的相应方法完成。
LRUCache为一个循环双向链表，与标准实现一致。其“头结点”lru_的prev始终指向最新结点，next始终指向最久未用节点，其对象容器为HashTable（哈希表），用于为LRUCache提供快速的查找操作。
LRUHandle为结点类。其next_hash指针用于HashTable中的bucket单向链表 。next和prev指针用于LRUCache循环双向链表的后继和前驱。

LRUHandle为什么会被同时置于哈希表和双向链表之中？
1.注意看LookUp的实现，如果单纯使用链表，则仅能提供O(n)的查询效率，所以在查询时，利用了哈希表实现O(1)的查询。
2.如果单纯使用哈希表呢？虽然可以实现O(1)的查询，但却无法更新缓存节点的访问时间。这是因为链表可以按照固定的顺序被遍历，而哈希表中的节点无法提供固定的遍历顺序（考虑Resize前后）。

ShardedLRUCache：
// 取哈希值的高四位来定位使用那片LRUCache缓存
  static uint32_t Shard(uint32_t hash) {
    return hash >> (32 - kNumShardBits);
  }


https://blog.csdn.net/caoshangpa/article/details/78960999
```

- arena

```
 arena是LevelDB内部实现的内存池。

我们知道，对于一个高性能的服务器端程序来说，内存的使用非常重要。C++提供了new/delete来管理内存的申请和释放，但是对于小对象来说，直接使用new/delete代价比较大，要付出额外的空间和时间，性价比不高。另外，我们也要避免多次的申请和释放引起的内存碎片。一旦碎片到达一定程度，即使剩余内存总量够用，但由于缺乏足够的连续空闲空间，导致内存不够用的假象。

C++ STL为了避免内存碎片，实现一个复杂的内存池，LevelDB中则没有那么复杂，只是实现了一个"一次性"内存池arena。在leveldb里面，并不是所有的地方都使用了这个内存池，主要是memtable使用，主要是用于临时存放用户的更新数据，由于更新的数据可能很小，所以这里使用内存池就很合适。

为了避免小对象的频繁分配，需要减少对new的调用，最简单的做法就是申请大块的内存，多次分给客户。LevelDB用一个vector<char*>来保存所有的内存分配记录，默认每次申请4k的内存块，记录下当前内存块剩余指针和剩余内存字节数，每当有新的申请，如果当前剩余的字节能满足需要，则直接返回给用户。如果不能，对于超过1k的请求，直接new一个指定大小的内存块并返回，小于1K的请求，则申请一个新的4k内存块，从中分配一部分给用户。当内存池对象析构时，分配的内存均被释放，保证了内存不会泄漏。


https://blog.csdn.net/caoshangpa/article/details/78842671
```

- skiplist


```
下面来研究一下跳表的核心思想：
先从链表开始，如果是一个单链表，那么我们知道在链表中查找一个元素I的话，需要将整个链表遍历一次，时间复杂度为O(n)。

如果是说链表是排序的，并且结点中还存储了指向后面第二个结点的指针的话，那么在查找一个结点时，仅仅需要遍历N/2个结点即可。因此查找的时间复杂度为O(n/2)

其实，上面基本上就是跳跃表的思想，每一个结点不单单只包含指向下一个结点的指针，可能包含很多个指向后续结点的指针，这样就可以跳过一些不必要的结点，从而加快查找、删除等操作。对于一个链表内每一个结点包含多少个指向后续元素的指针，这个过程是通过一个随机函数生成器得到，这样子就构成了一个跳跃表。

如果一个结点存在k个指向后续元素指针的话，那么称该结点是一个k层结点。一个跳表的层MaxLevel义为跳表中所有结点中最大的层数。跳表的所有操作均从上向下逐层进行，越上层一次next操作的跨度越大。

跳表结构：
(1) 由多层结构组成
(2) 每一层都是一个有序的链表
(3) 最底层(Level 1)的链表包含所有元素
(4) 如果一个元素出现在 Level i 的链表中，则它在 Level i 之下的链表也都会出现。
(5) 每个节点包含两个指针，一个指向同一链表中的下一个元素，一个指向下面一层的元素。

https://blog.csdn.net/caoshangpa/article/details/78862339
```

- 整体架构

```
https://www.cnblogs.com/haippy/archive/2011/12/04/2276064.html
```

- version

```
https://zhuanlan.zhihu.com/p/81768877
```

- AtomicPointer

```
AtomicPointer 是 leveldb 提供的一个原子指针操作类，使用了基于原子操作（atomic operation）或者内存屏障（memory barrier）的同步访问机制，这比用锁和信号量的效率要高。


#if defined(LEVELDB_HAVE_MEMORY_BARRIER)
class AtomicPointer {
 private:
  void* rep_;
 public:
  AtomicPointer() { }
  explicit AtomicPointer(void* p) : rep_(p) {}
  inline void* NoBarrier_Load() const { return rep_; }
  inline void NoBarrier_Store(void* v) { rep_ = v; }
  inline void* Acquire_Load() const {
    void* result = rep_;
    MemoryBarrier();
    return result;
  }
  inline void Release_Store(void* v) {
    MemoryBarrier();
    rep_ = v;
  }
};
无锁编程具体使用和考虑到的技术方法包括：原子操作（atomic operation）、内存栅栏（memory barrier）、内存顺序冲突（memory order）、 指令序列一致性（sequential consistency）等等。

利用std::atomic实现AtomicPointer
利用内存屏障来实现AtomicPointer


//thread1
Object.var1 = a;
Object.var2 = b;
Object.var2 = c;
atomicpointer.Release_Store(p);
 
//thread2
user_pointer = atomicpointer.Acquire_Load();
get Object.var1
get Object.var2
get Object.var3
对于Store Barrier来说，在指令后插入Store Barrier，能让写入缓存中的最新数据更新写入主内存，让其他线程可见。
对于Load Barrier来说，在指令前插入Load Barrier，可以让高速缓存中的数据失效，强制从新从主内存加载数据。

https://blog.csdn.net/caoshangpa/article/details/78852255
```







