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
    parseParam(m_params, m_paramsMap);
}

void Request::setToken(string str)
{
	
    //CMyRedis myRedis;
	//myRedis.Connect("127.0.0.1",6379);
	CMyRedis::ptr myRedis = CMyRedis::getMyRedisInstance();
			
    string tok= m_paramsMap["user"]+"123456";
	int sec=120;
	
    myRedis->Setex(m_paramsMap["user"],sec,tok);
	m_paramsMap["token"]=tok;
    m_token = tok;
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
    return m_paramsMap["token"];
}

const std::map<string, string>& Request::getParamsMap() const
{
    return m_paramsMap;
}

void Request::parseParam(std::string paramsStr, std::map<std::string, std::string> &reqParams)
{

	std::vector<std::string> pairslist = StringUtil::mySplit(paramsStr, "&");

	for (size_t i = 0; i < pairslist.size(); ++i)
	{
		std::string name = pairslist[i].substr(0, pairslist[i].find("="));
		std::string value = pairslist[i].substr(pairslist[i].find("=") + 1);

		reqParams[name] = value;
	}
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
