#include "Request.h"




Request::Request():
    m_method(""),
    m_url(""),
    m_params(""),
    m_token("")
{

}

Request::~Request()
{

}

void Request::setMethod(string str)
{
    m_method = str;
}

void Request::setUrl(string str)
{
    m_url = str;
}

void Request::setParams(string str)
{
    m_params = str;
}

void Request::setToken(string str)
{
    m_token = str;
}

string Request::getMethod() const
{
    return m_method;
}


string Request::getUrl() const
{
    return m_url;
}

string Request::getParams() const
{
    return m_params;
}

string Request::getToken() const
{
    return m_token;
}


//////////////////////////////////////////////////////
Response::Response():
    m_contentType("text/html"),
    m_setToken(""),
    m_resData("")
{

}

Response::~Response()
{

}

void Response::setContentType(const string str)
{
    m_contentType = str;
}

void Response::setSetToken(const string token)
{
    m_setToken = token;
}

void Response::setResData(string str)
{
    m_resData = str;
}

string Response::Out()
{
    //string strSetToken = m_setToken.toSetTokenStr();

    string retStr;
    retStr += "Content-type: " + m_contentType + "\r\n";
    //retStr += strSettoken;
    retStr += "\r\n";

    //LOG(LOG_MODULE, LOG_PROC,  "%s", retStr.c_str());
    //DBG(L_DEBUG, "%s", retStr.c_str());
    retStr += m_resData;
    return retStr;

}
