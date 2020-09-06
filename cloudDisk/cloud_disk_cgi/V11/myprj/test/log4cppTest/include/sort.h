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
		class Sort 
		{
		public:
			Sort();
			~Sort();

			
		private:
			DECLARE_STATIC_LOGGERMANAGER();
		};

	}
}
#endif // !ALGORITHM_SORT_H_