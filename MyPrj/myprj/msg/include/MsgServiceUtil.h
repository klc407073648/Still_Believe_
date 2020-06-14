/**
 * @file MsgServiceUtil.h
 * @brief  基础服务类定义
 * @author klc
 * @date 2020-06-14
 * @copyright Copyright (c) 2020年 klc
 */

#ifndef __STIBEL_MSGSERVICEUTIL_H__
#define __STIBEL_MSGSERVICEUTIL_H__

#include <iostream>
#include <string>

#include <fstream>
#include "json/json.h"

using namespace std;

namespace ServiceSpace
{
	/**
	* @brief 类型转换类
	*/
	class TypeUtil {
	public:
		static void jsonStrParse(string str);
	};
}

#endif //__STIBEL_MSGSERVICEUTIL_H__