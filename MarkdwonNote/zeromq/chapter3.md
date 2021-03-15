
# 函数学习
本章涉及的内容有：

* function和bind函数
* iovec结构体使用方法
* main函数命令行解析
* protobuf使用(网络通信、数据交换格式)
* 作用域函数::
* 智能指针share_ptr使用
* struct结构体使用(包含初始化函数)
* system函数学习(调用底层shell命令)
* typedef使用方法
* va_list函数(可变入参输入，动态处理)
---

## function & bind
 - bind函数: std::bind返回一个基于f的函数对象，其参数被绑定到args上:例如 auto fn_half = std::bind (my_divide,_1,2); 
 - 函数概述: std::function 是一种通用的多态函数包装器,可以存储、复制和调用任何可调用的目标——普通函数、lambda表达式、bind表达式或其他函数对象，以及指向成员函数的指针和指向数据成员的指针（统称为“可调用对象”）。存在意义：多个不同类型的可调用对象却拥有着相同的调用形式。例如 std::function<int(int,int)>

[C++11特性:bind和function函数使用](https://blog.csdn.net/baidu_15952103/article/details/110749611)

执行结果:
```
[root@192 function和bind函数学习]# ./bindTest
fn_five(): 5
fn_half(10): 5
fn_invert(10,2): 0.2
fn_rounding(10,3): 3
bound_member_fn():ten_two.multiply() 20
bound_member_data():ten_two.a 10
bound_member_fn_add(30,50): 80

[root@192 function和bind函数学习]# ./functionTest
fun1 add:15
fun2 sub():5
fun3 Lam:50
fun4 Math::div:2
fun5 Math::print_sum:15
```

## iovec学习
 - struct iovec定义了一个向量元素。通常，这个结构用作一个多元素的数组。对于每一个传输的元素，指针成员iov_base指向一个缓冲区，这个缓冲区是存放的是readv所接收的数据或是writev将要发送的数据。成员iov_len在各种情况下分别确定了接收的最大长度以及实际写入的长度。且iovec结构是用于scatter/gather IO的。readv和writev函数用于在一次函数调用中读、写多个非连续缓冲区。有时也将这两个函数称为散布读（scatter read）和聚集写（gather write）。

[struct iovec 结构体定义与使用](https://blog.csdn.net/baidu_15952103/article/details/109888362)

执行结果:
```
[root@192 mytest]# ./readv
12345HelloWorldIamJason
24 bytes read.
buf1: 12345
buf2: HelloWor
buf3: ldIamJason

[root@192 mytest]# ./writev
I am happy.

[root@192 mytest]# ./rw_test
rsize = 15
wsize = 15

```
## 命令行解析函数
在Linux下运行可执行程序时，一般需要带特定参数以指定运行的方式。通过是采用getopt()函数解析传入的参数内容，其头文件为<unistd.h>。

[getopt()函数分析以及命令行解析](https://blog.csdn.net/baidu_15952103/article/details/110734135)
```
#include <iostream>
#include <unistd.h>
#include <string>
 
using namespace std;
 
//解析ip, port, msg.
void testFun1(int argc, char *argv[])
{
    std::string ip = "127.0.0.1";
	int port = 8080;
	std::string msg = "Hello World";
	
	int opt;
    const char *str = "i:p:m:";
    while ((opt = getopt(argc, argv, str)) != -1)
    {
        switch (opt)
        {
        case 'i':
        {
            ip = optarg;
            break;
        }
        case 'p':
        {
            port = atoi(optarg);
            break;
        }
        case 'm':
        {
            msg = optarg;
            break;
        }
        default:
            break;
        }
    }
	cout<<"ip: "<<ip<<endl;
	cout<<"port: "<<port<<endl;
	cout<<"msg: "<<msg<<endl;
}
 
int main(int argc, char *argv[])
{
	testFun1(argc,argv);
	//testFun2(argc,argv);
    return 0;
}
 
```
执行结果:
```
[root@192 main函数命令行解析]# ./main
ip: 127.0.0.1
port: 8080
msg: Hello World
[root@192 main函数命令行解析]# ./main -i 10.43.33.115 -p 8888 -m Test
ip: 10.43.33.115
port: 8888
msg: Test
```

## protobuf使用
Protobuf是一种平台无关、语言无关、可扩展且轻便高效的序列化数据结构的协议，可以用于网络通信和数据存储。

使用步骤：
1.编号proto文件
2.protoc ./person.proto --cpp_out=./ 生成文件
3.g++ -g -o test_person test_person.cpp ./person.pb.cc -I. -lprotobuf -pthread -std=c++11
4../test_person 

[在C++中使用Protobuf](https://blog.csdn.net/baidu_15952103/article/details/110144131)

```
#include <iostream>
#include <fstream>
#include "person.pb.h"
#include "google/protobuf/io/zero_copy_stream_impl.h"
#include "google/protobuf/text_format.h"
 
 
using namespace test;
int main()
{
  Person p;
  p.set_name("test");
  p.set_id(100);
  p.set_email("a.iabc.com");
 
  //------------------将pb二进制信息保存到字符串
  std::string str;
  p.SerializeToString(&str);
  std::cout<<"str: ["<<str<<"]"<<std::endl;  
 
  //------------------将pb文本信息写入文件
  std::ofstream fw; 
  fw.open("./Person.txt", std::ios::out | std::ios::binary);
  google::protobuf::io::OstreamOutputStream *output = new google::protobuf::io::OstreamOutputStream(&fw);
  google::protobuf::TextFormat::Print(p, output);
 
  delete output;
  fw.close();
 
  //---------------------将pb文本信息保存到字符串
  std::string str1;
  google::protobuf::TextFormat::PrintToString(p, &str1);
  std::cout<<"str1: ["<<str1<<"]"<<std::endl;
 
  //---------------------反序列化
  Person p1;
  p1.ParseFromString(str);
  std::cout<<"name:"<<p1.name()<<",email:"<<p1.email()<<",id:"<<p1.id()<<std::endl;
 
  return 0;
}
```

执行结果:
```
[root@192 protobuf_test]# g++ -g -o test_person test_person.cpp ./person.pb.cc -I. -lprotobuf -pthread -std=c++11
[root@192 protobuf_test]# ./test_person
str: [
testd
a.iabc.com]
str1: [name: "test"
id: 100
email: "a.iabc.com"
]
name:test,email:a.iabc.com,id:100
[root@192 protobuf_test]# 

```

## 作用域函数
C++的::符合代表作用域运算符，例如MyClass的成员函数myFun，会写成MyClass::myFun,表示为MyClass中的myFun函数。但是还存在另外一种情况，就是前面没有作用域名，而是只有::加函数名的情况。这表示全局作用域，即所调用的函数是全局函数。

[C++ 作用域运算符::的使用](https://blog.csdn.net/baidu_15952103/article/details/105782001)

```
#include <iostream>
using namespace std;
 
// 全局函数
void myFun()
{
    cout<<"global function myFun"<<endl;
}
 
class MyClass
{
public:
    int printFun1()
    {
    	//调用的是全局函数
        ::myFun();
    }
 
     int printFun2()
    {
    	//调用的是成员函数
        myFun();
    }
    void myFun()
    {
        cout<<"MyClass function myFun"<<endl;
    }
};
 
int main()
{
    MyClass m;
    m.printFun1();
    cout<<"————————————————————"<<endl;
    m.printFun2();
    return 0;
}
 
```
执行结果:
```
global function myFun
————————————————————
MyClass function myFun
```

## share_ptr使用
[C++智能指针——shared_ptr的使用](https://blog.csdn.net/baidu_15952103/article/details/108893462)
```
#include <iostream>
#include <memory>
 
using namespace std;
 
class Test
{
public:
    Test(string s)
	:str(s)
    {
       cout<<"Test() create str: "<<str<<endl;
    }
    ~Test()
    {
        cout<<"~Test() delete str: "<<str<<endl;
    }
    string& getStr()
    {
        return str;
    }
    void setStr(const string& s)
    {
        str = s;
    }
    void print()
    {
        cout<<"str: "<<str<<endl;
    }
private:
    string str;
};
 
int main()
{
    shared_ptr<Test> ptest(new Test("123"));
    shared_ptr<Test> ptest2(new Test("456"));
	shared_ptr<Test> ptest3 = make_shared<Test>("789");
	
    cout<<"ptest2->getStr():"<<ptest2->getStr()<<endl;
    cout<<"ptest2.use_count():"<<ptest2.use_count()<<endl;
	
    ptest = ptest2;//“123”引用计数减1,计数为0自动销毁; "456"引用次数加1. (p=q的规则，p,q都为share_ptr)
    ptest->print();
	
    cout<<"ptest2.use_count():"<<ptest2.use_count()<<endl;//2
    cout<<"ptest.use_count():"<<ptest.use_count()<<endl;//2
	
	Test* mtest = ptest.get();
	mtest->print();
	
    ptest.reset();
    ptest2.reset();//此时“456”销毁
	
	cout<<"ptest3.unique():"<<boolalpha<<ptest3.unique()<<endl;
	shared_ptr<Test> ptest4(ptest3);
	cout<<"ptest3.unique():"<<boolalpha<<ptest3.unique()<<endl;
	ptest3.reset();
	ptest4.reset();
	
    cout<<"done !"<<endl;
    return 0;
}

```
执行结果:
```
[root@192 智能指针学习]# ./main
Test() create str: 123
Test() create str: 456
Test() create str: 789
ptest2->getStr():456
ptest2.use_count():1
~Test() delete str: 123
str: 456
ptest2.use_count():2
ptest.use_count():2
str: 456
~Test() delete str: 456
ptest3.unique():true
ptest3.unique():false
~Test() delete str: 789
done !
```
## struct结构体使用

```
#include<iostream>
#include<stdio.h>
#include<string>
using namespace std;

struct userInfo
{
    string userName;     
    int age;
	string address;
	
	userInfo():
	userName("null"),
	age(0),
	address("null")
	{}
	
	userInfo(const string& m_userName,int m_age,const string& m_address):
	userName(m_userName),
	age(m_age),
	address(m_address)
	{}
};

struct Day
{
   explicit Day(int d)
	       :val(d)
			{}
   int val;
};


int main()
{
	userInfo m_userInfo("jason",26,"Nanjing");
	Day m_Day(20.2);
	
	printf("userInfo userName:%s,age:%d,address:%s\n",m_userInfo.userName.c_str(),m_userInfo.age,m_userInfo.address.c_str());
	printf("m_Day:%d\n",m_Day.val);
	return 0;
}
 
```
执行结果:
```
[root@192 24]# ./main
userInfo userName:jason,age:26,address:Nanjing
m_Day:20
```

## 系统函数使用execShell

```
#include <iostream>
#include "util.h"
using namespace std;

int main()
{
   ShellUtil::execShell("touch klc.txt");
   ShellUtil::execShell("chmod 777 klc.txt");
   //string str=ShellUtil::execShellPipe("ls -al >> klc.txt");
   string str=ShellUtil::execShellPipe("ls -al ");
   cout<<ShellUtil::getParamValue("name","my.conf");
   cout<<ShellUtil::getParamValue("age","my.conf");
   ShellUtil::modParamValue("age","44","my.conf");
   cout<<ShellUtil::getParamValue("age","my.conf");
   //cout<<str<<endl;
    return 0;
}
```
执行结果:
```
待补充结果
为了更好的理解system()函数返回值，需要了解其执行过程，实际上system()函数执行了三步操作：

1.fork一个子进程；
2.在子进程中调用exec函数去执行command；
3.在父进程中调用wait去等待子进程结束。 对于fork失败，system()函数返回-1。 如果exec执行成功，也即command顺利执行完毕，则返回command通过exit或return返回的值。
```

## typeDef学习使用
[typedef 语句的使用](https://blog.csdn.net/baidu_15952103/article/details/108895171)
```
//一维数组
typedef int intArr[10];
typedef std::string stringArr[20];

//二维数组
typedef int matrix[5][5];
typedef char nameTable[3][3];

//对已有类型定义别名
typedef int inData;
typedef char chData;

intArr mIntArr = {1,2,3,4,5};
stringArr mStrArr = { "hello","world"};
matrix mMat ={{1,2,3,4,5},{6,7,8,9,10},{11,12,13}};
nameTable mNameTable ={{'a','b','c'},{'d','e','f'},{'g','h'}};
 
```
执行结果:
```
[root@localhost typedef]# make
g++ -o main main.cpp
[root@localhost typedef]# ./main
0th mIntArr is 1
1th mIntArr is 2
0th mStrArr is hello
1th mStrArr is world
————————————————————————————————————————
mMat[0][0]:1
mMat[0][1]:2
mMat[0][2]:3
mMat[0][3]:4
mMat[0][4]:5
mMat[1][0]:6
mMat[1][1]:7
mMat[1][2]:8
mMat[1][3]:9
mMat[1][4]:10
mMat[2][0]:11
mMat[2][1]:12
mMat[2][2]:13
mNameTable[0][0]:a
mNameTable[0][1]:b
mNameTable[0][2]:c
mNameTable[1][0]:d
mNameTable[1][1]:e
mNameTable[1][2]:f
mNameTable[2][0]:g
mNameTable[2][1]:h
mNameTable[2][2]:
————————————————————————————————————————
m_int is 100
m_char is Z

```

## va_list函数使用
使用场景描述：
当函数的入参个数不确定时，使用va_list函数进行动态处理，增加编程的灵活性。

[va_list函数学习（va_start，va_end, vasprintf）](https://blog.csdn.net/baidu_15952103/article/details/105886761)
```
#include <iostream>
#include <string.h>
#include <stdarg.h>
 
using namespace std;
 
/*
vasprintf()函数 :int vasprintf (char **buf, const char *format, va_list ap)
将格式化数据从可变参数列表写入缓冲区
*/
std::string  execute(const char* format, va_list ap);
 
void myFun1(const char* format, ...)
{
    va_list ap;
    va_start(ap, format);
    execute(format, ap);
    va_end(ap);
}
 
std::string  execute(const char* format, va_list ap)
{
	char* buf = nullptr;
    auto len = vasprintf(&buf, format, ap);
    if(len == -1) {
        return "";
    }
    std::string ret(buf, len);
	cout<<"ret:"<<ret<<endl;
    free(buf);
    return ret;
}
 
int myFun2(int count, ...)
{
    va_list ap; 
    va_start(ap, count);
    cout << "count: "<< count << endl;
    int sumNum = 0;
   
	for(int i=0 ; i< count ;i++)
	{
		sumNum+=va_arg(ap, int);
	}
 
    va_end(ap);
	
    return sumNum;
}
 
 
int main()
{
	int  num = 100;
	char character = 'A';
	string str="mystring";
    myFun1("Test va_list_Fun num:%d character:%c str:%s",num,character,str.c_str());
	
	int count = 5;
	int sumNum = myFun2(count,1,2,3,4,5);
	cout << "sumNum: "<< sumNum << endl;
    return 0;
}
 
```
执行结果:
```
[root@localhost funTest]# ./main
ret:Test va_list_Fun num:100 character:A str:mystring
count: 5
sumNum: 15
```


