/**
 * @file sort.h
 * @brief 排序算法封装
 * @author klc
 * @date 2020-03-21
 * @copyright Copyright (c) 2020年 klc
 */

#ifndef ALGORITHM_SORT_H_
#define ALGORITHM_SORT_H_

#include<vector>
#include<iostream>

using namespace std;

namespace algorithm
{
	namespace  sort
	{
		/**
		 * @brief 排序算法封装类
		*/
		template<class T>
		class Sort 
		{
		public:
			/**
			 * @brief 构造函数
			 */
			Sort();

			/**
			 * @brief 析构函数
			 */
			~Sort();

			/**
			 * @brief 冒泡排序
			 * @param[in] nums  传入传出参数，输入排序前序列，得到排序后序列
			 */
			void bubbleSort(vector<T> &nums);

			/**
			 * @brief 桶排序
			 * @param[in] nums  传入传出参数
			 */
			void bucketSort(vector<T> &nums);

			/**
			 * @brief 堆排序
			 * @param[in] nums  传入传出参数
			 */
			void heapSort(vector<T> &nums);

			/**
			 * @brief 插入排序
			 * @param[in] nums  传入传出参数
			 */
			void insertionSort(vector<T> &nums);

			/**
			 * @brief 归并排序
			 * @param[in] nums  传入传出参数
			 */
			void mergeSort(int left, int  right, vector<T> &nums);

			/**
			 * @brief 快速排序
			 * @param[in] nums  传入传出参数
			 */
			void quickSort(int  L, int  R, vector<T> &nums);

			/**
			 * @brief 选择排序
			 * @param[in] nums  传入传出参数
			 */
			void selectionSort(vector<T> &nums);

		private:
			/**
			 * @brief 返回a,b
			 * @param[in] a  交换参数a
			 * @param[in] b  交换参数b
			 */
			//T max(T a, T b);

			/**
			 * @brief 堆插入函数
			 * @param[in] nums  输入序列
			 * @param[in] index  位置
			 */
			void heapInsert(vector<T> &nums, int index);

			/**
			 * @brief 堆化
			 * @param[in] nums  输入序列
			 * @param[in] index  位置
			 * @param[in] index  数量
			 */
			void heapify(vector<T> &nums, int index, int size);

			/**
			 * @brief 归并辅助函数
			 * @param[in] 子序列的左右位置，以及 输入序列
			 */
			void merge(int  l1, int  r1, int  l2, int  r2, vector<T> &nums);
			int partition(int  L, int  R, vector<T> &nums);

		};

	}
}

#include "sort_impl.h"
#endif // !ALGORITHM_SORT_H_