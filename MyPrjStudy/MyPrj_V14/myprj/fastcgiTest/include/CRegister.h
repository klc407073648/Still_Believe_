/**
 * @file CRegister.h
 * @brief 注册类
 * @author klc
 * @date 2020-08-09
 * @copyright Copyright (c) 2020年 klc
 */

#ifndef __STIBEL_CREGISTER_H__
#define __STIBEL_CREGISTER_H__


#include "Request.h"
#include "MySQL.h"
#include "Util.h"
#include "LoggerManager.h"
using namespace minilog;

class Register
{

public:
        Response reg(Request req);

private:
        DECLARE_STATIC_LOGGERMANAGER();

};



#endif //__STIBEL_CREGISTER_H__
