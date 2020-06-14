#ifndef _RANDOMARRAR_H_
#define _RANDOMARRAR_H_
/******************************************************************* 
简要描述:随机数生成器，用于产生测试用例
创建日期: 2019.12.15
作者: klc
版本：v1.0
修改日期: 
******************************************************************/


#include<math.h>
#include <iostream>
#include <ctime>
#include<vector>
#include"LoggerManager.h"

using namespace minilog;
using namespace std;

namespace algorithm
{
	namespace  klc
	{
		class randomArray
		{
		public:
			randomArray();
			~randomArray();

			vector<int> generateRandomArray(int size, int value);
			void rightMethod(vector<int> &a);
			int  myTest(int m_testCount = 20 ,int m_size = 20 , int m_maxValue = 100, bool m_succeed = true);
			int testCount;
			int size ;
			int maxValue ;
			bool succeed ;
			DECLARE_STATIC_LOGGERMANAGER();

		};

	}
}
#endif // !_RANDOMARRAR_H_


