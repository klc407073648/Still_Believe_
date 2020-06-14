#ifndef ALGORITHM_SORT_H_
#define ALGORITHM_SORT_H_

/******************************************************************* 
简要描述: 提供基础的排序算法实现
创建日期: 2019.12.15
作者: klc
版本：v1.0
修改日期: 
******************************************************************/

#include<vector>
#include"LoggerManager.h"

using namespace minilog;

using namespace std;


namespace algorithm
{
	namespace  sort
	{
		class Sort //:public  LoggerManager
		{
		public:
			Sort();
			~Sort();

			void bubbleSort(vector<int> &nums);
			void bucketSort(vector<int> &nums);
			void heapSort(vector<int> &nums);
			void insertionSort(vector<int> &nums);
			void mergeSort(int left, int right, vector<int> &nums);
			void quickSort(int L, int R, vector<int> &nums);
			void selectionSort(vector<int> &nums);
		private:
			DECLARE_STATIC_LOGGERMANAGER();
			//static minilog::LoggerManager*  mlogger;
			void swap(int &a, int &b);
			int max(int a, int b);
			void heapInsert(vector<int> &nums, int index);
			void heapify(vector<int> &nums, int index, int size);
			void merge(int l1, int r1, int l2, int r2, vector<int> &nums);
			int partition(int L, int R, vector<int> &nums);
			int sortDemo();

		};

	}
}
#endif // !ALGORITHM_SORT_H_