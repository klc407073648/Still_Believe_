#include "sort.h"

using namespace algorithm::sort;

int main()
{
	Sort<double> mySort;
	double a[] = { 7.5, 1, 5.8, 4.6666, 3.111, 6.5544 };
	vector<double> nums(a,a+6);
	while (1)
	{
		vector<double> nums(a,a+6);
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
			mySort.bubbleSort(nums);
			cout << "result of bubbleSort" << endl;
			break;
		case 2:
			mySort.bucketSort(nums);
			cout << "result of bucketSort(only int)" << endl;
			break;
		case 3:
			mySort.heapSort(nums); 
			cout << "result of heapSort" << endl;
			break;
		case 4:
			mySort.insertionSort(nums); 
			cout << "result of insertionSort" << endl;
			break;
		case 5:
			mySort.mergeSort(0, nums.size() - 1, nums);
			cout << "result of mergeSort" << endl;
			break;
		case 6:
			mySort.quickSort(0, nums.size() - 1, nums); 
			cout << "result of quickSort" << endl;
			break;
		case 7:
			mySort.selectionSort(nums); 
			cout << "result of selectionSort" << endl;
			break;
		}
		for (int i = 0; i < nums.size(); i++)
		{
			cout << nums[i] << " ";
		}
		cout << endl;
		cout <<"——————————————————————————————"<<endl;
	}
}