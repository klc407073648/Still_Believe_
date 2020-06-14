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

//��ȷ�������㷨
void randomArray::rightMethod(vector<int> &a)
{
	/* Sort mySort;
	mySort.quickSort(0,a.size()-1,a); */
}

//����д���㷨����ȷ��
int randomArray::myTest(int m_testCount, int m_size , int m_maxValue , bool m_succeed )
{
	/* testCount = m_testCount;//���Դ���
	size = m_size;//�����С
	maxValue = m_maxValue;//�������ֵ
	succeed = m_succeed;//�㷨������ȷ��־
	srand((unsigned)time(NULL));
	for (int i = 0; i < testCount; i++)
	{
		vector<int> array, array1, array2;//ԭ���飬��������1����������2
		array = generateRandomArray(size, maxValue);
		array1 = array;//
		array2 = array;
		Sort mySort;
		rightMethod(array1);//��ȷ�㷨����							
		mySort.bucketSort(array2);//Ͱ����
	
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
		if (array1 != array2)//�Ƚ������㷨����Ƿ�һ��
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
