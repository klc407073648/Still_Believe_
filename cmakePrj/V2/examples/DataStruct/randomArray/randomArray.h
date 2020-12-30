/**
 * @file randomArray.h
 * @brief 随机数组生成器
 * @author klc
 * @date 2020-03-22
 * @copyright Copyright (c) 2020年 klc
 */

#ifndef _RANDOMARRAR_H_
#define _RANDOMARRAR_H_


#include<math.h>
#include <iostream>
#include <ctime>
#include<vector>

using namespace std;

namespace algorithm
{
	namespace  random
	{
		template<class T>

		class randomArray
		{
		public:
			randomArray(bool isTypeInt=false);
			randomArray(int size,T minValue,T maxValue,bool isTypeInt=false);
			~randomArray();

			void generateRandomArray();
		    const vector<T>& getResultArray();
		private:
			/// 数组大小
			int m_size ;
			///最小值
			T m_minValue ;
			///最大值
			T m_maxValue ;
			bool m_isTypeInt;
			///生成的数组结果
            vector<T> generateResult;
		};

	}
}

#include "randomArray_impl.h"

#endif // !_RANDOMARRAR_H_


