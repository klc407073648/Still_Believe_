/////////////////////////////////////////////
///COPYRIGHT NOTICE
/// Copyright (c) 2018
/// All rights reserved.
///
/// @file Request.h
/// @brief 封装 Request/Response 类
///
/// @version 1.0
/// @author liukang
/// @date 2017.12.15
//////////////////////////////////////////////
#pragma once

#include <map>
#include <vector>
#include <iostream>
#include <functional>
#include <fcgi_stdio.h>
#include "fcgi_config.h"
#include <string>

using namespace std;

class  Request
{
public:
    Request();
    ~Request();

    void setMethod(string str);
    void setUrl(string str);
    void setParams(string str);
    void setToken(string str);
    void setCharParams(char *str);

    string getMethod() const;
    string getUrl() const;
    string getParams() const;
    string getToken() const;
    char* getCharParams() const;

private:

    string m_method;
    string m_url;
    string m_params;
    string m_token;
    char *m_charparam;

};


class  Response
{
public:
    Response();
    ~Response();

    void setContentType(const string str);
    void setSetToken(const string str);

    void setResData(string str);

    string Out();

private:

    //Content-type:
    string m_contentType;

    //Set-Token:
    string m_setToken;

    //html txt
    string m_resData;

};
