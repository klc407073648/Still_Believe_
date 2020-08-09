#include "CRegister.h"
#include "memory.h"
#include <string>
#include "json/json.h"

#include "Poco/RegularExpression.h"
#include "common.h"
#include "TEncode.h"

using namespace EncodeFun;
using Poco::RegularExpression;


CREATE_STATIC_LOGGERMANAGER(Register);

Response Register::reg(Request req)
{
	LogDebug("Register::reg() method : %s,  url: %s,  params: %s, token: %s",
        req.getMethod().c_str(),
        req.getUrl().c_str(),
        req.getParams().c_str(),
        req.getToken().c_str());
	Response res;
	string data;
	string method = req.getMethod();

	//存放user,passwd,token
	std::map<string, string> paramsMap=req.getParamsMap();

	if ("GET" == method)
	{
	   //注册类不应该有GET请求
	}
	else if ("POST" == method)
	{
		MySQL::ptr mysql= MySQL::getInstance();
		if (!mysql->connect())
		{
			LogError("mysql connect fail");
			res.setResData("mysql connect fail");
			return res;
		}

		string m_user = paramsMap["user"];
		string m_passwd = paramsMap["passwd"];

		RegularExpression reg_user(USER_REG);
		RegularExpression reg_passwd(PASSWD_REG);


		if(!reg_user.match(m_user))
		{
			//data = "<p> </p>";
			//data = "<p>用户名不符合规范:用户名长度6-20位，由数字，字母，下划线组成，且不能以数字开头，字母不区分大小写</p>";
			data = "user can't conform to the specification: the length is between 6-20 , \
			compose by numbers, letters and underline, and cannot start with a number, and the letters are not case sensitive";
			LogError("user match error: %s",m_user.c_str());
			res.setResData(data);
			return res;
		}

		if(!reg_passwd.match(m_passwd))
		{
			data = "Password can't conform to the specification: the length is between 8-20 ,\
			at least contains numbers and letters, and can have special characters.";
			//data = "<p>密码不符合规范:密码长度8-20位，至少包含数字跟字母，可以有特殊字符</p>";
			LogError("passwd match error: %s",m_passwd.c_str());
			res.setResData(data);
			return res;
		}
	
		m_passwd = TEncode::base64Encode(m_passwd);//加密存储

		LogInfo("m_passwd encode:%s",m_passwd.c_str());

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

 