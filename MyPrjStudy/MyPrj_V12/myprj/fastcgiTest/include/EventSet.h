/**
 * @file EventSet.h
 * @brief ???????
 *        ?????Login???Register???Upload??????????
 * @author klc
 * @date 2020-08-03
 * @copyright Copyright (c) 2020? klc
 */

#ifndef __STIBEL_EVENTSET_H__
#define __STIBEL_EVENTSET_H__

#include "Request.h"
#include "MySQL.h"
#include "Util.h"
#include "Event.h"
#include "LoggerManager.h"
#include "common.h"

using namespace minilog;

class Login
{

public:
        Response login(Request req);
        Response logout(Request req);

private:
        DECLARE_STATIC_LOGGERMANAGER();

};

class Register
{

public:
        Response reg(Request req);

private:
        DECLARE_STATIC_LOGGERMANAGER();

};

class Upload
{

public:
        char *memstr(char *full_data, int full_data_len, char *substr);
        Response upload(Request req);

private:
        DECLARE_STATIC_LOGGERMANAGER();

};

#endif //__STIBEL_EVENTSET_H__
