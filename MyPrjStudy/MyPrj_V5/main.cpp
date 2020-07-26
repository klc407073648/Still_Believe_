#include <iostream>
#include "MySQL.h"
#include <map>
#include <string.h>
#include <iostream>
#include <sstream>
#include <string>
#include "Request.h"
#include "make_log.h"
#define HTML_PATH "/usr/local/nginx/test/"

#include <iostream>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <cstdio>
#include <map>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <mysql/mysql.h>
#include "MutexLock.h"

#ifndef lint
static const char rcsid[] = "$Id: echo.c,v 1.5 1999/07/28 00:29:37 roberts Exp $";
#endif /* not lint */

#include "fcgi_config.h"
#include "fcgi_stdio.h"


#ifdef _WIN32
#include <process.h>
#else
extern char **environ;
#endif


#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <functional>
#include <assert.h>


using namespace std;

typedef std::function<Response(Request)> handlerFunc;
std::map<string , handlerFunc>  eventMap;
std::map<string , handlerFunc>::iterator eventMapIt;

std::map<string, string> reqParams;


vector<string> mySplit(const string& str, string sp_string)  // split(), str 是要分割的string
{ 
    vector<string> vecString; 
    int sp_stringLen = sp_string.size(); 
    int lastPosition = 0; 
    int index = -1; 
	//如果要分割的串就是自己怎么写？
    while(-1 != (index=str.find(sp_string , lastPosition))) 
    { 
		if(str.substr(lastPosition, index - lastPosition) != string("")) //避免第一个字符串就是sp_string
		{
			vecString.push_back(str.substr(lastPosition, index - lastPosition));
		}
        lastPosition = index +sp_stringLen; 
    } 
    string lastStr = str.substr(lastPosition); 
    if ( !lastStr.empty() ) 
    { 
        vecString.push_back(lastStr); 
    } 
    return vecString; 
} 

void parseParam(string paramsStr)
{

	  std::vector<string> pairslist = mySplit(paramsStr,"&");

        for ( size_t i = 0; i < pairslist.size(); ++i )
        {
            string name = pairslist[i].substr( 0, pairslist[i].find("=") );
            string value = pairslist[i].substr( pairslist[i].find("=") + 1 );

            reqParams[name] = value;
        }
}

std::string loadFile(const std::string &filename)
{
    FILE *fp = fopen( filename.c_str(), "rb" );
    if ( fp == NULL ) return "";

    // read file size
    fseek( fp, 0, SEEK_END );
    int bufsize = ftell( fp );
    rewind( fp );

    char *buf = new char[bufsize+1];
    memset( buf, 0, bufsize+1 );
    fread( buf, 1, bufsize, fp );
    fclose( fp );
	
    return std::string( buf, bufsize );
}
class myEvent
{
	public:
	
	Response regTest(Request req)
	{
	   /*  LOG("cgi", "reg","method : %s,  url: %s,  params: %s, cookie: %s",
			req.getMethod().c_str(),
			req.getUrl().c_str(),
			req.getParams().c_str(),
			req.getToken().c_str()); */
		printf("222");
		parseParam(req.getParams());
		Response res;
		string data;
		printf("222");
		printf("222");
		printf("req.getParams():%s",req.getParams().c_str());
		//printf("reqParams:%d",reqParams.size());
		printf("req.getMethod():%s",req.getMethod().c_str());
	 /*   if("GET" == req.getMethod())
		{
			printf("222");
			if("klc123" == req.getToken())
			{
				//login
				data=loadFile(string(HTML_PATH) + "inputtest.html");
			}
			else
			{
				//not login
				data=loadFile(string(HTML_PATH) + "login.html");
			}
			//reqCookie.delCookie("password");
			res.setSetToken("klc123");
			res.setResData(data);

		}
		else if("POST" == req.getMethod())
		{
			// sql database
			printf("222111");
			std::map<std::string, std::string> params;
			//数据库的参数
			params["host"] = "localhost";
			params["user"] = "root";
			params["passwd"] = "klczxas789";
			params["dbname"] = "yourdb";
			   
			printf("mmmmm1%s",reqParams["user"].c_str());
			printf("mmmmm2%s",reqParams["passwd"].c_str());
			 MySQL::ptr mysql(new MySQL(params));
			if(!mysql->connect()) {
				//std::cout << "connect fail" << std::endl;
				return res;
			}
			
			string m_user = reqParams["user"];
			string m_passwd  = reqParams["passwd"];
			
			string sql_insert("");
				sql_insert+="INSERT INTO user(user, passwd) VALUES('";
				sql_insert+=m_user;
				sql_insert+="', '";
				sql_insert+=m_passwd;
				sql_insert+="')";
			ISQLData::ptr m_ptr1=mysql->query(sql_insert);
			
			printf("mmmmm"); //m_ptr1->getErrStr()==""
			printf("1111%s",(m_ptr1->getErrStr()).c_str());
			 if(1)
			{
				data=loadFile(string(HTML_PATH) + "login.html");
				res.setSetToken("klc123");
			}
			else
			{
				data=loadFile(string(HTML_PATH) + "judge.html");
			}   
			printf("mmmmm22");
			res.setResData(data);
		}   */
		return res;
	}
};


int main(int argc, char** argv) 
{
	char **initialEnv = environ;
    int count = 0;
	myEvent mEvent;
	eventMap.insert(make_pair("/register",std::bind(&myEvent::regTest, &mEvent, std::placeholders::_1)));
	//eventMap.insert(make_pair(event,std::move(func)));
	//eventMap.insert(make_pair(event,std::move(func)));
	//run();
	while(FCGI_Accept() >= 0)
	{
	char *contentLength = getenv("CONTENT_LENGTH");
        int len;
		
		  printf("Content-type: text/html\r\n"
	    "\r\n"
	    "<title>FastCGI echo</title>"
	    "<h1>FastCGI echo</h1>\n"); 

	    printf("register begin<p>\n");

        if (contentLength != NULL) 
		{
            len = strtol(contentLength, NULL, 10);
			printf("len=%d<p>\n",len);
        }
      
            char *file_buf = NULL; //==========> 开辟存放文件的 内存 <===========
			file_buf = (char *)malloc(len);
		  
			if (file_buf == NULL)
			{
			   //printf("malloc error! file size is to big!!!!\n");
			   return -1;
			}
		  
		    int ret = fread(file_buf, 1, len, stdin); //从标准输入(web服务器)读取内容
			if(ret == 0)
			{
				printf("fread error\n");
				return -1;
			}
			
			printf("%s\<p>\n",file_buf);
		
		
	//"GET" or "POST"
	string requestMethod =  getenv("REQUEST_METHOD");
	//"/login?key=value&name=pwd"
	printf("requestMethod: %s",requestMethod.c_str());
	string temp="?";
	string requestUri = getenv("REQUEST_URI") + temp + file_buf;
	 printf("requestUri: %s",requestUri.c_str());
	size_t pos = requestUri.find_first_of("?");
	string requestRoute = requestUri.substr(0,pos);
	printf("requestRoute: %s",requestRoute.c_str());
	string requestParam;
	printf("111");
	if("GET" == requestMethod)
	{
		if(-1 != pos)
		{
			requestParam = requestUri.substr(requestUri.length() - pos - 1);
		}
	}
	else if("POST" == requestMethod)
	{
		/* int ilen = atoi(getenv("CONTENT_LENGTH")) + 1;
		char *bufpost = (char* )malloc(ilen);
		memset(bufpost, 0, ilen);
		FCGI_fread(bufpost, ilen, 1, FCGI_stdin);
		requestParam = string(bufpost);
		free(bufpost); */
	} 

	printf("111");
	string temp1=file_buf;
	printf("temp1: %s",temp1.c_str());
	/* Request req;
	req.setMethod(requestMethod);
	req.setUrl(requestRoute);
	req.setParams(temp1);
	req.setToken("klc123");
    printf("222");
	//后续还未实现
	Response res = regTest(req);
	FCGI_printf(res.Out().c_str()); */
	eventMapIt = eventMap.find(requestRoute);
	if(eventMapIt == eventMap.end())
	{
		//DBG(L_ERROR, "not find route: %s ", requestUri.c_str());
		//LOG("cgi", "reg", "not find route: %s ", requestUri.c_str());
		printf("error");
	}
	else
	{
		handlerFunc func = eventMapIt->second;
		Request req;
		req.setMethod(requestMethod);
		req.setUrl(requestRoute);
		req.setParams(requestParam);
		req.setToken("klc123");

		Response res = func(req);
		printf("111");
		//FCGI_printf(res.Out().c_str());
	}  
	}
return 0;
}
