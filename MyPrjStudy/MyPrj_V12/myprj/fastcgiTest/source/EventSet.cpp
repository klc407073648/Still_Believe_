#include "EventSet.h"
#include "memory.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string>
#include <string.h>

CREATE_STATIC_LOGGERMANAGER(Login);
CREATE_STATIC_LOGGERMANAGER(Register);
CREATE_STATIC_LOGGERMANAGER(Upload);

Response Login::login(Request req)
{
	 LogDebug("Login::login() method : %s,  url: %s,  params: %s, token: %s",
        req.getMethod().c_str(),
        req.getUrl().c_str(),
        req.getParams().c_str(),
        req.getToken().c_str());

	Response res;
	string data;

	if ("GET" == req.getMethod())
	{
		if ("klc123" == req.getToken())
		{
			//login
			LogDebug("login GETin");
			data = FileUtil::loadFile(string(HTML_PATH) + "loginError.html");
		}
		else
		{
			//not login
			data = FileUtil::loadFile(string(HTML_PATH) + "juade.html");
		}
		//reqCookie.delCookie("password");
		res.setSetToken("klc123");
		res.setResData(data);
		LogDebug("login GETout");
	}
	else if ("POST" == req.getMethod())
	{
		
		MySQL::ptr mysql= Event::getMySQLInstance();
		if (!mysql->connect())
		{
			LogError("mysql connect fail");
			return res;
		}
		
		string m_user = (req.getParamsMap())["user"];
		string m_passwd = (req.getParamsMap())["passwd"];
	
		
		string sql_insert("");
		sql_insert += "select * from user where user='";
		sql_insert += m_user;
		sql_insert += "' AND passwd='";
		sql_insert += m_passwd;
		sql_insert += "';";
		mysql->query(sql_insert);
		
        LogError("sql_insert=%s",sql_insert.c_str());
		LogError("mysql->getAffectedRows(): %d ,mysql->getErrStr(): %s",mysql->getAffectedRows(),mysql->getErrStr().c_str());
		
		//登陆成功,插入失败表示登陆成功
		if (mysql->getAffectedRows() != 0)
		{
			data = FileUtil::loadFile(string(HTML_PATH) + "index.html");
			res.setSetToken(req.getToken());
		}
		//登陆失败
		else
		{
			data = FileUtil::loadFile(string(HTML_PATH) + "loginError.html");			
		}

		res.setResData(data);
	}
	LogDebug("login end");
	return res;
}

Response Login::logout(Request req)
{
	LogDebug("logout begin");
    Response res;
	string data;
    LogDebug("Method is %s",req.getMethod().c_str());
	if ("GET" == req.getMethod())
	{
		 data = FileUtil::loadFile(string(HTML_PATH) + "login.html");			
		 res.setResData(data);
	}
	else if ("POST" == req.getMethod())
	{			
	    data = FileUtil::loadFile(string(HTML_PATH) + "judge.html");			
		res.setResData(data);
	}
	LogDebug("logout end");
	return res;
}



Response Register::reg(Request req)
{
	LogDebug("Register::reg() method : %s,  url: %s,  params: %s, token: %s",
        req.getMethod().c_str(),
        req.getUrl().c_str(),
        req.getParams().c_str(),
        req.getToken().c_str());
	Response res;
	string data;
	LogDebug("Method is %s",req.getMethod().c_str());

	if ("GET" == req.getMethod())
	{
		if ("klc123" == req.getToken())
		{
			//login
			data = FileUtil::loadFile(string(HTML_PATH) + "login.html");
		}
		else
		{
			//not login
			data = FileUtil::loadFile(string(HTML_PATH) + "inputtest.html");
		}
		//reqCookie.delCookie("password");
		res.setSetToken("klc123");
		res.setResData(data);
	}
	else if ("POST" == req.getMethod())
	{

		MySQL::ptr mysql= Event::getMySQLInstance();
		if (!mysql->connect())
		{
			LogError("mysql connect fail");
			return res;
		}

		string m_user = (req.getParamsMap())["user"];
		string m_passwd = (req.getParamsMap())["passwd"];

		string sql_insert("");
		sql_insert += "INSERT INTO user(user, passwd) VALUES('";
		sql_insert += m_user;
		sql_insert += "', '";
		sql_insert += m_passwd;
		sql_insert += "')";

		mysql->query(sql_insert);
		
		LogError("mysql->getErrno(): %d ,mysql->getErrStr(): %s",mysql->getErrno(),mysql->getErrStr().c_str());
		//注册成功，进入登陆界面
		if (mysql->getErrno() == 0)
		{
			data = FileUtil::loadFile(string(HTML_PATH) + "login.html");
			res.setSetToken(req.getToken());
		}
		//注册失败，返回已经注册了
		else
		{
			data = FileUtil::loadFile(string(HTML_PATH) + "registerError.html");
		}

		res.setResData(data);
	}
	LogDebug("reg end");
	return res;
}

 char* Upload::memstr(char* full_data, int full_data_len, char* substr) 
{ 
	//异常处理
    if (full_data == NULL || full_data_len <= 0 || substr == NULL) 
	{ 
        return NULL; 
    } 

    if (*substr == '\0')
	{ 
        return NULL; 
    } 

	//匹配子串的长度
    int sublen = strlen(substr); 

    int i; 
    char* cur = full_data; 
    int last_possible = full_data_len - sublen + 1; //减去匹配子串后的长度
    for (i = 0; i < last_possible; i++) 
	{ 
        if (*cur == *substr) 
		{ 
            if (memcmp(cur, substr, sublen) == 0) 
			{ 
                //found  
                return cur; 
            } 
        }
		
        cur++; 
    } 

    return NULL; 
}  

Response Upload::upload(Request req)
{
	Response res;
	string data;
	std::string mystr;

	if ("GET" == req.getMethod())
	{
		if ("klc123" == req.getToken())
		{
			//login
			data = FileUtil::loadFile(string(HTML_PATH) + "inputtest.html");
		}
		else
		{
			//not login
			data = FileUtil::loadFile(string(HTML_PATH) + "inputtest.html");
		}
		//reqCookie.delCookie("password");
		res.setSetToken("klc123");
		res.setResData(data);
	}
	else if ("POST" == req.getMethod())
	{
		char *bufpost =NULL;
		int len =strlen(bufpost);
		char *begin = NULL;
		char *p, *q, *k;
		char boundary[4096] = {0};
		char content_text[4096] = {0};

		begin = bufpost; //内存起点
		p = begin;

		printf("file_buf = %s\n", bufpost);

		//get boundary 得到分界线, ------WebKitFormBoundary88asdgewtgewx
		p = strstr(begin, "\r\n");
		if (p == NULL)
		{
			printf("boundary error\n");
			//return -1;
		}

		//拷贝分界线
		strncpy(boundary, begin, p - begin);
		boundary[p - begin] = '\0'; //字符串结束符

		//printf("boundary =[%s]\n",boundary);

		p += 2; //\r\n
		//已经处理了p-begin的长度
		len -= (p - begin);
		//get content text head
		begin = p;

		//Content-Disposition: form-data; user="mike"; filename="xxx.jpg"; md5="xxxx"; size=10240\r\n
		p = strstr(begin, "\r\n");
		if (p == NULL)
		{
			//printf("Content-Disposition error\n");
			//return -1;
		}
		strncpy(content_text, begin, p - begin);
		content_text[p - begin] = '\0';

		printf("content_text =[%s]\n", content_text);
		string mystr;
		/* std::string mystr=content_text;
					int pos1=mystr.find_last_of("=");
					int pos2=mystr.find_last_of('"');
					mystr = mystr.substr(pos1+2,pos2-(pos1+2));
					printf("mystr= %s\n",mystr.c_str()); */

		//从content_text中解析出filename
		mystr = content_text;
		int pos1 = mystr.find("filename=");
		int pos2 = mystr.find_last_of('"');
		mystr = mystr.substr(pos1 + 10, pos2 - (pos1 + 10));
		printf("mystr= %s\n", mystr.c_str());

		p += 2; //\r\n
		len -= (p - begin);

		begin = p;
		p = strstr(begin, "\r\n");
		p += 4; //\r\n\r\n
		len -= (p - begin);

		begin = p;

		p = memstr(begin, len, boundary); //util_cgi.h， 找文件结尾
		if (p == NULL)
		{
			//printf("find boundary error\n");
			//return -1;
		}
		else
		{
			p = p - 2; //\r\n
		}

		int fd = 0;
		fd = open(mystr.c_str(), O_CREAT | O_WRONLY, 0644);
		if (fd < 0)
		{
			//printf("fd open error\n");
			//return -1;
		}

		//ftruncate会将参数fd指定的文件大小改为参数length指定的大小
		ftruncate(fd, (p - begin));
		write(fd, begin, (p - begin));
		close(fd);

		std::string cmdstr = "fdfs_upload_file /etc/fdfs/client.conf ./" + mystr;
		std::string retStr = ShellUtil::execShellPipe(cmdstr);

		//登陆成功
		if (retStr != "")
		{
			string postSuc = "post success upload" + retStr;
			res.setResData(postSuc);
		}
		//登陆失败
		else
		{
			res.setResData("post fail upload");
		}

		res.setResData(data);
	}
	return res;
}