
# 数据结构
本章涉及的内容有：

* 随机数据生成器(用于生成数据，校验算法正确性，例如排序)
* 跳表概念以及使用学习
* 排序算法(总结各类排序算法)
* STL使用方法
---
## 随机数组
该测试程序用于生成随机数据，便于验证排序算法等。
```
template<class T> randomArray<T>::randomArray(int size,T minValue,T maxValue,bool isTypeInt) //isTypeInt默认值false

randomArray<int> *m_randomArray1 = new randomArray<int>(true);
randomArray<double> *m_randomArray2 = new randomArray<double>(20, 50, 100);
```
执行结果:
```
[root@192 DataStruct]# ./randomArray
m1 begin
13 23 27 35 13 6 4 12 17 24
m1 end
m2 begin
54.265 65.515 98.535 79.375 84.865 56.43 71.42 90.46 93.085 98.72 68.385 85.065 56.98 86.52 52.715 51.13 92.325 84.92 84.545 87.32
m2 end
```

## 跳表
Skiplist是一个用于有序元素序列快速搜索的数据结构，是一个“概率型”的数据结构，可以在很多应用场景中替代平衡树。Skip list算法与平衡树相比，有相似的渐进期望时间边界，但是它更简单，更快，使用更少的空间。

[数据结构学习——skiplist跳表](https://blog.csdn.net/baidu_15952103/article/details/109009130)
```
#include "SkipList.h"

int main()
{
	Skiplist* m_Skiplist = new Skiplist();
	
	m_Skiplist->insert(7);
    m_Skiplist->insert(14);
    m_Skiplist->insert(21);
    m_Skiplist->insert(32);
    m_Skiplist->insert(37);
    m_Skiplist->insert(71);
    m_Skiplist->insert(85);
	
	m_Skiplist->showSkiplist();

	return 0;
}
```
执行结果:
```
因为增加层数的方案用的是抛硬币，即根据随机数来确定是否增加“快线”
所以每次执行结果生成的level层数不一致，如下：

[root@192 skiplist1]# ./skipList
7 14 32
7 14 32 37
7 14 32 37 85
7 14 32 37 71 85
7 14 21 32 37 71 85
[root@192 skiplist1]# ./skipList
21
21
21
7 14 21 32 71
7 14 21 32 37 71 85
```

## 排序算法
将学习的冒泡排序、桶排序、堆排序、插入排序、归并排序、快速排序以及选择排序方法进行归纳，以模板类型方式实现各个排序算法，使得更加灵活。

```
可以循环选择执行的排序算法:
cout << "please choose your sort method" << endl;
cout << "1.bubbleSort" << endl;
cout << "2.bucketSort" << endl;
cout << "3.heapSort" << endl;
cout << "4.insertionSort" << endl;
cout << "5.mergeSort" << endl;
cout << "6.quickSort" << endl;
cout << "7.selectionSort" << endl;
```

执行结果:
```
除了桶排序不同，因为是将对应的值放在第i个桶里，必须进行取整。

please choose your sort method
1.bubbleSort
2.bucketSort
3.heapSort
4.insertionSort
5.mergeSort
6.quickSort
7.selectionSort
2
result of bucketSort(only int)
1 3 4 5 6 7
——————————————————————————————
orignl sort
7.5 1 5.8 4.6666 3.111 6.5544
please choose your sort method
1.bubbleSort
2.bucketSort
3.heapSort
4.insertionSort
5.mergeSort
6.quickSort
7.selectionSort
3
result of heapSort
1 3.111 4.6666 5.8 6.5544 7.5
——————————————————————————————
```

## STL使用
### map使用
学习map数据结构的使用方法。

[C++ map使用方法总结](https://blog.csdn.net/baidu_15952103/article/details/109407737)

```
#include <map>
#include <iostream>
#include <string>

using namespace std;

int main() {
    map<int, string> stuMap;
    
	cout<<"————————————————————插入操作—————————————————"<<endl;
	stuMap[1001]="Jason";
	stuMap.insert(pair<int, string>(1002,"Helen"));
	stuMap.insert(make_pair(1003,"Steve"));

    map<int, string>::const_iterator iter = stuMap.begin();
    for (; iter != stuMap.end(); ++iter)
	{
		cout <<"id:"<< iter->first <<" name:"<< iter->second << endl;
	}
	cout<<"————————————————————取值操作—————————————————"<<endl;
	
	cout <<"stuMap[1004]:"<<stuMap[1004]<<endl;
	
	//使用at会进行关键字检查，因此下面语句会报错
	//stuMap.at(1005) = "Bob";
	
	cout<<"————————————————————容量查询—————————————————"<<endl;
	
	cout<<"stuMap empty state is "<<boolalpha<<stuMap.empty()<<endl;
	
	cout<<"stuMap size is "<<boolalpha<<stuMap.size()<<endl;
	
	cout<<"stuMap.count(1008) is "<<boolalpha<<stuMap.count(1008)<<endl;
	
	cout<<"————————————————————删除操作—————————————————"<<endl;
	cout<<"before delete"<<endl;
	iter = stuMap.begin();
    for (; iter != stuMap.end(); ++iter)
	{
		cout <<"id:"<< iter->first <<" name:"<< iter->second << endl;
	}
	
	stuMap.erase(1004);
	
	iter = stuMap.begin();
    for (; iter != stuMap.end(); ++iter)
	{
		if(iter->second=="Helen")	
		{
			stuMap.erase(iter);
			break;
		}
	}
	
	cout<<"after delete"<<endl;
	iter = stuMap.begin();
    for (; iter != stuMap.end(); ++iter)
	{
		cout <<"id:"<< iter->first <<" name:"<< iter->second << endl;
	}
	
	cout<<"————————————————————查找操作—————————————————"<<endl;
	iter = stuMap.find(1001);
	if (iter != stuMap.end())
		cout <<"stuMap.find(1001) name:"<< iter->second << endl;
	
        
    return 0;
}
```
执行结果:
```
[root@192 DataStruct]# ./mapTest
————————————————————插入操作—————————————————
id:1001 name:Jason
id:1002 name:Helen
id:1003 name:Steve
————————————————————取值操作—————————————————
stuMap[1004]:
————————————————————容量查询—————————————————
stuMap empty state is false
stuMap size is 4
stuMap.count(1008) is 0
————————————————————删除操作—————————————————
before delete
id:1001 name:Jason
id:1002 name:Helen
id:1003 name:Steve
id:1004 name:
after delete
id:1001 name:Jason
id:1003 name:Steve
————————————————————查找操作—————————————————
stuMap.find(1001) name:Jason

```


