#include"randomArray.h"
//#include"sort.h"
#include <stdio.h>
#include <stdlib.h>

using namespace algorithm::klc;

CREATE_STATIC_LOGGERMANAGER(randomArray);
#define logError(msg)  mlogger->logError(msg)
#define logWarn(msg)   mlogger->logWarn(msg)
#define logInfo(msg)   mlogger->logInfo(msg)
#define logDebug(msg)  mlogger->logDebug(msg) 
randomArray::randomArray()
{
  logWarn("randomArray Test");
}
randomArray::~randomArray()
{

}

vector<int> randomArray::generateRandomArray(int size, int value)
{
	vector<int>  b;
	//srand((unsigned)time(int(rand()));
	int length = (int)((size)*((rand() % 100)*0.01));
	for (int i = 0; i < length+1; i++)
	{
		//b.push_back((int)(rand() % value)- (int)(rand() % value));
		b.push_back((int)(rand() % value));
	}
	return b;
}

//正确的排序算法
void randomArray::rightMethod(vector<int> &a)
{
	/* Sort mySort;
	mySort.quickSort(0,a.size()-1,a); */
}

//测试写的算法的正确性
int randomArray::myTest(int m_testCount, int m_size , int m_maxValue , bool m_succeed )
{
	/* testCount = m_testCount;//测试次数
	size = m_size;//数组大小
	maxValue = m_maxValue;//数组最大值
	succeed = m_succeed;//算法测试正确标志
	srand((unsigned)time(NULL));
	for (int i = 0; i < testCount; i++)
	{
		vector<int> array, array1, array2;//原数组，待排数组1，待排数组2
		array = generateRandomArray(size, maxValue);
		array1 = array;//
		array2 = array;
		Sort mySort;
		rightMethod(array1);//正确算法排序							
		mySort.bucketSort(array2);//桶排序
	
		cout << "Original sequence:        ";
		for (int i = 0; i<array.size(); i++)
		{
			cout << array[i] << " ";
		}
		cout << endl;
		cout << "rightMethod reslut:       ";
		for (int i = 0; i<array1.size(); i++)
		{
			cout << array1[i] << " ";
		}
		cout << endl;
		cout << "myMethod reslut:          ";

		for (int i = 0; i<array2.size(); i++)
		{
			cout << array2[i] << " ";
		}
		cout << endl;
		if (array1 != array2)//比较两种算法结果是否一致
		{
			succeed = false;
			cout << "error , Original sequence:        ";
			for (int i = 0; i<array.size(); i++)
			{
				cout << array[i] << " ";
			}
			break;
		}
	}
	succeed == true ? cout << "myMethod correct" << endl : cout << "myMethod error" << endl; */

	return 0;
}
