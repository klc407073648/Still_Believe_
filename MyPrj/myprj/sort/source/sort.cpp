#include"sort.h"
#include<iostream>

//Linux�¼��ݲ���
//linux����ʹ�ú���srand��rand������������ַ���
#include <stdio.h>
#include <stdlib.h>
#define INT_MAX ((int)(~0U>>1))
#define INT_MIN     (-INT_MAX - 1)
//Linux�¼��ݲ���


using namespace std;
using namespace algorithm::sort;

//LoggerManager*  Sort::mlogger = new LoggerManager();
//Sort::mlogger->getInstance("SORT");

CREATE_STATIC_LOGGERMANAGER(Sort);

#define logError(msg)  mlogger->logError(msg)
#define logWarn(msg)   mlogger->logWarn(msg)
#define logInfo(msg)   mlogger->logInfo(msg)
#define logDebug(msg)  mlogger->logDebug(msg) 



Sort::Sort()
{
	//mlogger->getInstance("Sort");
	//mlogger->logWarn("Sort Test");
	logError("Sort Test");
	//logError(LoggerManager::getLogger("SORT"),"Sort Func Test");
}
Sort::~Sort()
{
	
}

//ð������
void Sort::bubbleSort(vector<int> &nums)
{
	if (nums.size() <2)
		return;
	int size = nums.size() - 1;
	for (int i = size, j; i>0; i--)
	{
		for (j = 0; j < i; j++)
			if (nums[j] > nums[j + 1])
				swap(nums[j], nums[j + 1]);
	}
}

//Ͱ����
void Sort::bucketSort(vector<int> &nums)
{
	if (nums.size() < 2) {
		return;
	}
	int m_max = INT_MIN;
	int size = nums.size();
	for (int i = 0; i <size; i++) {
		m_max = max(m_max, nums[i]);
	}
	vector<int> bucket(m_max + 1);
	for (int i = 0; i < size; i++) {
		bucket[nums[i]]++;
	}
	int i = 0;
	for (int j = 0; j < bucket.size(); j++) {
		while (bucket[j]-- > 0) {
			nums[i++] = j;
		}
	}
}

//������
//�Ѿ�����ȫ��������������κ�һ�����������ֵ����ͷ����С�����κ�һ����������Сֵ����ͷ��
//��N��������λ���������ѣ�һ�������һ��С����
void Sort::heapSort(vector<int> &nums)
{
	if (nums.size() < 2) {
		return;
	}
	for (int i = 0; i < nums.size(); i++) {
		heapInsert(nums, i);
	}
	int size = nums.size();
	swap(nums[0], nums[--size]);//�����ֵ����
	while (size > 0) {
		heapify(nums, 0, size);//[0,size)�ϵ���nums[0]ʹ���Ϊ�����
		swap(nums[0], nums[--size]);
	}
}

//��������
void Sort::insertionSort(vector<int> &nums)
{
	if (nums.size() < 2)
		return;
	int size = nums.size() - 1;
	for (int i = 1, j; i <= size; i++)
	{
		int tmp = nums[i];
		for (j = i; j > 0 && tmp < nums[j - 1]; j--)
			nums[j] = nums[j - 1];
		nums[j] = tmp;
	}
}

//�鲢����
void Sort::mergeSort(int left, int right, vector<int> &nums)
{
	if (left >= right)
		return;
	int mid = (left + right) >> 1;
	mergeSort(left, mid, nums);
	mergeSort(mid + 1, right, nums);
	merge(left, mid, mid + 1, right, nums);
}

//��������
void Sort::quickSort(int L, int R, vector<int> &nums)
{
	if (L >= R)
		return;
	int mid = partition(L, R, nums);
	quickSort(L, mid - 1, nums);
	quickSort(mid + 1, R, nums);
}

//ѡ������
void Sort::selectionSort(vector<int> &nums)
{
	if (nums.size() < 2)
		return;
	int size = nums.size() - 1;
	for (int i = 0, j, least; i <= size; i++)
	{
		for (j = i + 1, least = i; j <= size; j++)
		{
			if (nums[j] < nums[least])
				least = j;
		}
		if (i != least)
			swap(nums[i], nums[least]);
	}
}

//����������
void Sort::swap(int &a, int &b)
{
	int tmp = a;
	a = b;
	b = tmp;
}

//�����ֵ
int Sort::max(int a, int b)
{
	return a > b ? a : b;
}

//���������ú��������ڹ�������

void Sort::heapInsert(vector<int> &nums, int index) {
	while (nums[index] > nums[(index - 1) / 2]) {
		swap(nums[index], nums[(index - 1) / 2]);
		index = (index - 1) / 2;
	}
}

//���������ú������������߼�
void Sort::heapify(vector<int> &nums, int index, int size) {
	int left = index * 2 + 1;
	while (left < size) {
		int largest = left + 1 < size && nums[left + 1] > nums[left] ? left + 1 : left;
		largest = nums[largest] > nums[index] ? largest : index;
		if (largest == index) {
			break;
		}
		swap(nums[largest], nums[index]);
		index = largest;
		left = index * 2 + 1;
	}
}


//�鲢�������ú������鲢�����߼�
void Sort::merge(int l1, int r1, int l2, int r2, vector<int> &nums)
{
	int i = l1;
	int j = l2;
	int n = (r1 - l1 + 1) + (r2 - l2 + 1);
	int k = 0;
	vector<int> tmp(n);

	while (i <= r1&&j <= r2)
	{
		tmp[k++] = nums[i] < nums[j] ? nums[i++] : nums[j++];
	}
	while (i <= r1)
		tmp[k++] = nums[i++];
	while (j <= r2)
		tmp[k++] = nums[j++];
	for (int i = 0; i < n; i++)
		nums[l1 + i] = tmp[i];
}

int Sort::partition(int L, int R, vector<int> &nums)
{
	swap(nums[L + (int)(rand() % (R - L + 1))], nums[L]);
	int less = L + 1;
	int more = R;
	while (L<more)
	{
		if (nums[less] <= nums[L])
			swap(nums[less++], nums[L++]);
		else
			swap(nums[less], nums[more--]);
	}

	return L;
}


int Sort::sortDemo()
{
	Sort mySort;
	int a[] = { 7, 1, 5, 4, 3, 6 };
	vector<int> nums(a,a+6);
	while (1)
	{
		vector<int> nums(a,a+6);
		cout << "orignl sort" << endl;
		for (int i = 0; i < nums.size(); i++)
		{
			cout << nums[i] << " ";
		}
		cout << endl;
		cout << "please choose your sort method" << endl;
		cout << "1.bubbleSort" << endl;
		cout << "2.bucketSort" << endl;
		cout << "3.heapSort" << endl;
		cout << "4.insertionSort" << endl;
		cout << "5.mergeSort" << endl;
		cout << "6.quickSort" << endl;
		cout << "7.selectionSort" << endl;
		int choose;
		cin >> choose;
		switch (choose)
		{
		case 1:
			mySort.bubbleSort(nums); break;
		case 2:
			mySort.bucketSort(nums); break;
		case 3:
			mySort.heapSort(nums); break;
		case 4:
			mySort.insertionSort(nums); break;
		case 5:
			mySort.mergeSort(1, nums.size() - 1, nums); break;
		case 6:
			mySort.quickSort(3, nums.size() - 1, nums); break;
		case 7:
			mySort.selectionSort(nums); break;
		}
		cout << "result of sort" << endl;
		for (int i = 0; i < nums.size(); i++)
		{
			cout << nums[i] << " ";
		}
		cout << endl;
	}
}