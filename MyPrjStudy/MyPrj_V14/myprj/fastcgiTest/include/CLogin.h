/**
 * @file CLogin.h
 * @brief ???
 * @author klc
 * @date 2020-08-09
 * @copyright Copyright (c) 2020年 klc
 */

#ifndef __STIBEL_CLOGIN_H__
#define __STIBEL_CLOGIN_H__

#include "Request.h"
#include "MySQL.h"
#include "Util.h"
#include "LoggerManager.h"
using namespace minilog;

class Login
{

public:
        Response login(Request req);
        Response logout(Request req);

private:
        DECLARE_STATIC_LOGGERMANAGER();
};

#endif //__STIBEL_CLOGIN_H__
