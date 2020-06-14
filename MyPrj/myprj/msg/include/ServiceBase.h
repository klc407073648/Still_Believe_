/**
 * @file ServiceBase.h
 * @brief  基础服务类定义
 * @author klc
 * @date 2020-06-14
 * @copyright Copyright (c) 2020年 klc
 */

#ifndef __STIBEL_SERVICEBASE_H__
#define __STIBEL_SERVICEBASE_H__

#include <iostream>
#include <string>

using namespace std;

namespace ServiceSpace
{
	class ServiceBase
	{	
		public:
			typedef std::function<void()> handlerFunc;

			ServiceBase(){};
			virtual ~ServiceBase(){};

			virtual void serviceInit() = 0;
			
			virtual void listenEventRegist(int eventID,handlerFunc&& func) = 0;

			virtual void listenEventRemove(int eventID) = 0;

			virtual int serviceHandler() = 0;
			
			virtual void serviceStop() = 0;
	};
}

#endif //__STIBEL_SERVICEBASE_H__