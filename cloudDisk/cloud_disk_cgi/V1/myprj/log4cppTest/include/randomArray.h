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


namespace algorithm
{
	namespace  klc
	{
		class randomArray
		{
		public:
			randomArray();
			~randomArray();

		private:
			DECLARE_STATIC_LOGGERMANAGER();

		};

	}
}
#endif // !_RANDOMARRAR_H_


