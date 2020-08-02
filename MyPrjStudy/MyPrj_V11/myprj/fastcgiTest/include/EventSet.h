/////////////////////////////////////////////
///COPYRIGHT NOTICE
/// Copyright (c) 2018
/// All rights reserved.
///
/// @file Event.h
/// @brief http请求路由
///
/// @version 1.0
/// @author liukang
/// @date 2017.12.15
//////////////////////////////////////////////
#pragma once

#include "Request.h"
#include "MySQL.h"
#include "Util.h"
#include "Event.h"
#include <mysql/mysql.h>
#include <map>
#include"LoggerManager.h"

using namespace minilog;

#define HTML_PATH "/usr/local/nginx/test/"

class  Login
{
	public:
      Response login(Request req);
      Response logout(Request req);
	private:
        DECLARE_STATIC_LOGGERMANAGER();
};

class  Register
{
public:
    Response reg(Request req);
private:
        DECLARE_STATIC_LOGGERMANAGER();    
};

class  Upload
{
public:
    char* memstr(char* full_data, int full_data_len, char* substr);
    Response upload(Request req);
private:
        DECLARE_STATIC_LOGGERMANAGER();
};
