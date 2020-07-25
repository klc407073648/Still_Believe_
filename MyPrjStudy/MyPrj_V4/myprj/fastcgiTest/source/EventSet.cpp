#include "EventSet.h"
#include "memory.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string>
#include <string.h>

void Login::parseParam(string paramsStr, std::map<string, string> &reqParams)
{

	std::vector<string> pairslist = StringUtil::mySplit(paramsStr, "&");

	for (size_t i = 0; i < pairslist.size(); ++i)
	{
		string name = pairslist[i].substr(0, pairslist[i].find("="));
		string value = pairslist[i].substr(pairslist[i].find("=") + 1);

		reqParams[name] = value;
	}
}

Response Login::login(Request req)
{
	Response res;
	string data;

	if ("GET" == req.getMethod())
	{
		ShellUtil::execShellPipe("echo \"login GET\" >> ./mylog.log");
		if ("klc123" == req.getToken())
		{
			//login
			ShellUtil::execShellPipe("echo \"login GET1111\" >> ./mylog.log");
			data = FileUtil::loadFile(string(HTML_PATH) + "inputtest.html");
		}
		else
		{
			//not login
			ShellUtil::execShellPipe("echo \"login GET2222\" >> ./mylog.log");
			data = FileUtil::loadFile(string(HTML_PATH) + "inputtest.html");
		}
		//reqCookie.delCookie("password");
		res.setSetToken("klc123");
		res.setResData(data);
	}
	else if ("POST" == req.getMethod())
	{
		// sql database
		ShellUtil::execShellPipe("echo \"login POST\" >> ./mylog.log");
		std::map<std::string, std::string> params;
		//数据库的参数
		params["host"] = "localhost";
		params["user"] = "root";
		params["passwd"] = "klczxas789";
		params["dbname"] = "yourdb";

		std::map<string, string> reqParams;
		parseParam(req.getParams(), reqParams);

		MySQL::ptr mysql(new MySQL(params));
		if (!mysql->connect())
		{
			//std::cout << "connect fail" << std::endl;
			return res;
		}

		string m_user = reqParams["user"];
		string m_passwd = reqParams["passwd"];

		string cmd11 = "echo " + m_user + " >> ./mylog.log";
		ShellUtil::execShellPipe(cmd11);
		string cmd22 = "echo " + m_passwd + " >> ./mylog.log";
		ShellUtil::execShellPipe(cmd22);

		/* string sql_select("");
			sql_select+="select * from  user where user='";
			sql_select+=m_user;
			sql_select+="' and passwd='";
			sql_select+=m_passwd;
			sql_select+="')"; */

		string sql_insert("");
		sql_insert += "INSERT INTO user(user, passwd) VALUES('";
		sql_insert += m_user;
		sql_insert += "', '";
		sql_insert += m_passwd;
		sql_insert += "')";
		mysql->query(sql_insert);

		int num = mysql->getErrno();
		string numStr = std::to_string(num);
		string cmd = "echo " + numStr + " >> ./mylog.log";
		ShellUtil::execShellPipe(cmd);

		//登陆成功,插入失败表示登陆成功
		if (mysql->getErrno() != 0)
		{
			data = FileUtil::loadFile(string(HTML_PATH) + "inputtest.html");
			res.setSetToken(req.getToken());
			ShellUtil::execShellPipe("echo \"POSTlogin1111\" >> ./mylog.log");
		}
		//登陆失败
		else
		{
			data = FileUtil::loadFile(string(HTML_PATH) + "judge.html");
			ShellUtil::execShellPipe("echo \"POSTlogin2222\" >> ./mylog.log");
		}

		res.setResData(data);
	}
	return res;
}

void Register::parseParam(string paramsStr, std::map<string, string> &reqParams)
{

	std::vector<string> pairslist = StringUtil::mySplit(paramsStr, "&");

	for (size_t i = 0; i < pairslist.size(); ++i)
	{
		string name = pairslist[i].substr(0, pairslist[i].find("="));
		string value = pairslist[i].substr(pairslist[i].find("=") + 1);

		reqParams[name] = value;
	}
}

Response Register::reg(Request req)
{
	Response res;
	string data;
	//ShellUtil::execShellPipe("echo \"GET\" >> ./mylog.log");
	if ("GET" == req.getMethod())
	{
		ShellUtil::execShellPipe("echo \"GET\" >> ./mylog.log");
		if ("klc123" == req.getToken())
		{
			//login
			ShellUtil::execShellPipe("echo \"GET1111\" >> ./mylog.log");
			data = FileUtil::loadFile(string(HTML_PATH) + "inputtest.html");
		}
		else
		{
			//not login
			ShellUtil::execShellPipe("echo \"GET2222\" >> ./mylog.log");
			data = FileUtil::loadFile(string(HTML_PATH) + "inputtest.html");
		}
		//reqCookie.delCookie("password");
		res.setSetToken("klc123");
		res.setResData(data);
	}
	else if ("POST" == req.getMethod())
	{
		// sql database
		ShellUtil::execShellPipe("echo \"POST\" >> ./mylog.log");
		std::map<std::string, std::string> params;
		//数据库的参数
		params["host"] = "localhost";
		params["user"] = "root";
		params["passwd"] = "klczxas789";
		params["dbname"] = "yourdb";

		std::map<string, string> reqParams;
		parseParam(req.getParams(), reqParams);

		MySQL::ptr mysql(new MySQL(params));
		if (!mysql->connect())
		{
			//std::cout << "connect fail" << std::endl;
			return res;
		}

		string m_user = reqParams["user"];
		string m_passwd = reqParams["passwd"];

		string sql_insert("");
		sql_insert += "INSERT INTO user(user, passwd) VALUES('";
		sql_insert += m_user;
		sql_insert += "', '";
		sql_insert += m_passwd;
		sql_insert += "')";
		ISQLData::ptr m_ptr = mysql->query(sql_insert);
		//mysql->getErrno();
		int num = mysql->getErrno();
		string numStr = std::to_string(num);
		string cmd = "echo " + numStr + " >> ./mylog.log";
		ShellUtil::execShellPipe(cmd);

		string errStr = mysql->getErrStr();
		string cmd1 = "echo " + errStr + " >> ./mylog.log";
		ShellUtil::execShellPipe(cmd1);

		//登陆成功
		if (mysql->getErrno() == 0)
		{
			data = FileUtil::loadFile(string(HTML_PATH) + "login.html");
			res.setSetToken(req.getToken());
			ShellUtil::execShellPipe("echo \"POST1111\" >> ./mylog.log");
		}
		//登陆失败
		else
		{
			data = FileUtil::loadFile(string(HTML_PATH) + "inputtest.html");
			ShellUtil::execShellPipe("echo \"POST2222\" >> ./mylog.log");
		}

		res.setResData(data);
	}
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
		ShellUtil::execShellPipe("echo \"GET download\" >> ./mylog.log");
		if ("klc123" == req.getToken())
		{
			//login
			ShellUtil::execShellPipe("echo \"GET download1111\" >> ./mylog.log");
			data = FileUtil::loadFile(string(HTML_PATH) + "inputtest.html");
		}
		else
		{
			//not login
			ShellUtil::execShellPipe("echo \"GET downloadT2222\" >> ./mylog.log");
			data = FileUtil::loadFile(string(HTML_PATH) + "inputtest.html");
		}
		//reqCookie.delCookie("password");
		res.setSetToken("klc123");
		res.setResData(data);
	}
	else if ("POST" == req.getMethod())
	{
		char *bufpost =req.getCharParams();
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
			string cmd111 = "echo " + postSuc + " >> ./mylog.log";
			ShellUtil::execShellPipe(cmd111);
			res.setResData(postSuc);
		}
		//登陆失败
		else
		{
			string postSuc = "post fail upload" + retStr;
			string cmd111 = "echo 111" + postSuc + " >> ./mylog.log";
			ShellUtil::execShellPipe(cmd111);
			res.setResData("post fail upload");
		}

		res.setResData(data);
	}
	return res;
}
