/**
 * @file Request.h
 * @brief 事件请求和事件响应的封装类
 * @author klc
 * @date 2020-08-03
 * @copyright Copyright (c) 2020年 klc
 */

#ifndef __STIBEL_REQUEST_H__
#define __STIBEL_REQUEST_H__


#include <map>
#include <vector>
#include <iostream>
#include <functional>
#include <fcgi_stdio.h>
#include "fcgi_config.h"
#include <string>
#include "CMyRedis.h"
#include "Util.h"


using namespace minilog;

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

    string getMethod() const;
    string getUrl() const;
	string getParams() const;
    string getToken() const;
    const std::map<string, string>& getParamsMap() const;

private:
    void parseParam(std::string paramsStr, std::map<std::string, std::string> &reqParams);
private:
    string m_method;
    string m_url;
    string m_params;
    string m_token;
    std::map<string, string> m_paramsMap;

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

#endif //__STIBEL_REQUEST_H__
