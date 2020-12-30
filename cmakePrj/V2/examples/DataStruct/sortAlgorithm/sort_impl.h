/**
 * @file sort_impl.h
 * @brief 排序算法实现
 * @author klc
 * @date 2020-03-21
 * @copyright Copyright (c) 2020年 klc
 */

#ifndef ALGORITHM_SORT_IMPL_H_
#define ALGORITHM_SORT_IMPL_H_

#include "sort.h"
#include <stdio.h>
#include <stdlib.h>
#define INT_MAX ((int)(~0U>>1))
#define INT_MIN     (-INT_MAX - 1)

namespace algorithm
{
	namespace  sort
	{
		template<class T>
		Sort<T>::Sort()
		{
			cout<<"Sort()"<<endl;
		}
		
		template<class T>
		Sort<T>::~Sort()
		{
			cout<<"~Sort()"<<endl;
		} 

		//冒泡排序
		template<class T>
		void Sort<T>::bubbleSort(vector<T> &nums)
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

		//桶排序
		template<class T>
		void Sort<T>::bucketSort(vector<T> &nums)
		{
			if (nums.size() < 2) 
			{
				return;
			}
			
			int m_max = INT_MIN;
			int size = nums.size();
			for (int i = 0; i <size; i++) {
				m_max = (m_max>nums[i]) ? m_max : nums[i] ;
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

		//堆排序
		//堆就是完全二叉树，大根堆任何一棵子树的最大值都是头部，小根堆任何一棵子树的最小值都是头部
		//找N个数的中位数，两个堆，一个大根堆一个小根堆
		template<class T>
		void Sort<T>::heapSort(vector<T> &nums)
		{
			if (nums.size() < 2) {
				return;
			}
			for (int i = 0; i < nums.size(); i++) {
				heapInsert(nums, i);
			}
			int size = nums.size();
			swap(nums[0], nums[--size]);//将最大值弹出
			while (size > 0) {
				heapify(nums, 0, size);//[0,size)上调整nums[0]使其成为大根堆
				swap(nums[0], nums[--size]);
			}
		}

		//插入排序
		template<class T>
		void Sort<T>::insertionSort(vector<T> &nums)
		{
			if (nums.size() < 2)
				return;
			int size = nums.size() - 1;
			for (int i = 1, j; i <= size; i++)
			{
				T tmp = nums[i];
				for (j = i; j > 0 && tmp < nums[j - 1]; j--)
					nums[j] = nums[j - 1];
				nums[j] = tmp;
			}
		}

		//归并排序
		template<class T>
		void Sort<T>::mergeSort(int left, int right, vector<T> &nums)
		{
			if (left >= right)
				return;
			int mid = (left + right) >> 1;
			mergeSort(left, mid, nums);
			mergeSort(mid + 1, right, nums);
			merge(left, mid, mid + 1, right, nums);
		}

		//快速排序
		template<class T>
		void Sort<T>::quickSort(int L, int R, vector<T> &nums)
		{
			if (L >= R)
				return;
			int mid = partition(L, R, nums);
			quickSort(L, mid - 1, nums);
			quickSort(mid + 1, R, nums);
		}

		//选择排序
		template<class T>
		void Sort<T>::selectionSort(vector<T> &nums)
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

	
		//堆排序所用函数，用于构造大根堆
		template<class T>
		void Sort<T>::heapInsert(vector<T> &nums, int index) {
			while (nums[index] > nums[(index - 1) / 2]) {
				swap(nums[index], nums[(index - 1) / 2]);
				index = (index - 1) / 2;
			}
		}

		//堆排序所用函数，堆排序逻辑
		template<class T>
		void Sort<T>::heapify(vector<T> &nums, int index, int size) {
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


		//归并排序所用函数，归并排序逻辑
		template<class T>
		void Sort<T>::merge(int l1, int r1, int l2, int r2, vector<T> &nums)
		{
			int i = l1;
			int j = l2;
			int n = (r1 - l1 + 1) + (r2 - l2 + 1);
			int k = 0;
			vector<T> tmp(n);

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

		template<class T>
		int Sort<T>::partition(int L, int R, vector<T> &nums)
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


		

	
	}
}

#endif