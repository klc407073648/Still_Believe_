#include "RegisterCgi.h"

Config* RegisterCgi::m_Config;

CREATE_STATIC_LOGGERMANAGER(RegisterCgi);

RegisterCgi::RegisterCgi()
{
	m_Config = new Config(json_file);
    m_ConfigMap=m_Config->getConfigMap();
	LogInfo("RegisterCgi Create!");
}

RegisterCgi::~RegisterCgi()
{
	delete m_Config;
	LogInfo("RegisterCgi Finish!");
}

int RegisterCgi::getRegInfo(const string& reg_buf, string& user, string& nick_name, string& pwd, string& tel, string& email)
{
	Json::Reader reader;
	Json::Value root;
 
     /*json数据如下
    {
        userName:xxxx,
        nickName:xxx,
        firstPwd:xxx,
        phone:xxx,
        email:xxx
    }
    */

	if (reader.parse(reg_buf, root))
	{
	   string m_userName = root["userName"].asString();
       string m_nickName = root["nickName"].asString();
       string m_firstPwd = root["firstPwd"].asString();
       string m_phone = root["phone"].asString();
       string m_email = root["email"].asString();
       

	   LogInfo("user=%s,nick_name=%s,pwd=%s,tel=%s,email=%s",m_userName.c_str(),m_nickName.c_str()\
       ,m_firstPwd.c_str(),m_phone.c_str(),m_email.c_str());
	   
       user = m_userName;
	   nick_name = m_nickName;
       pwd = m_firstPwd;
	   tel = m_phone;
       email = m_email;
    
		if( user =="" || nick_name =="" || pwd =="" || tel =="" || email =="" )
		{
			LogError("user or nick_name or pwd or tel or email is null,please check!");
			return -1;
		}
	   
	   LogInfo("Reading Complete!");
	}
	else
	{
        LogError("parse error!");
		return -1;
	}
 
	return 0;
}

int RegisterCgi::userReg(string& reg_buf)
{
	std::map<std::string, std::string> params;

    params["host"] = m_ConfigMap["mysql_host"];
    params["user"] = m_ConfigMap["mysql_user"];
    params["passwd"] = m_ConfigMap["mysql_passwd"];
    params["dbname"] = m_ConfigMap["mysql_dbname"];

	MySQL::ptr mysql= MySQL::getInstance(params);
    
	if (!mysql->connect())
	{
		LogError("mysql connect fail");
		return -1;
	}

	int ret = 0;

    string user;
	string nick_name;
    string pwd;
	string tel;
    string email;

    ret = getRegInfo(reg_buf, user, nick_name, pwd, tel, email);

    if(ret != 0)
    {
        LogError("get register info error");
        return ret;
    }

	string sql_query("");
	sql_query += "select * from user where name='";
	sql_query += user;
	sql_query += "';";

	mysql->query(sql_query);
		
    LogDebug("sql_query=%s",sql_query.c_str());
	LogError("mysql->getAffectedRows(): %d ,mysql->getErrStr(): %s",mysql->getAffectedRows(),mysql->getErrStr().c_str());
		
	//查询注册用户是否存在，若存在返回 -2
	if (mysql->getAffectedRows() != 0)
	{
        LogError("user %s is already exist",user.c_str());
		ret = -2;
        return ret;
	}
	
     //当前时间戳
    struct timeval tv;
    struct tm* ptm;
    char time_str[128];

    //使用函数gettimeofday()函数来得到时间。它的精度可以达到微妙
    gettimeofday(&tv, NULL);
    ptm = localtime(&tv.tv_sec);//把从1970-1-1零点零分到当前时间系统所偏移的秒数时间转换为本地时间
    //strftime() 函数根据区域设置格式化本地时间/日期，函数的功能将时间格式化，或者说格式化一个时间字符串
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", ptm);

    //sql 语句, 插入注册信息
    string sql_insert("");
	sql_insert += "insert into user (name, nickname, password, phone, createtime, email) values ('";
	sql_insert += user;
	sql_insert += "' , '";
	sql_insert += nick_name;
    sql_insert += "' , '";
    sql_insert += pwd;
	sql_insert += "' , '";
	sql_insert += tel;
	sql_insert += "' , '";
	sql_insert += time_str;
    sql_insert += "' , '";
    sql_insert += email;
	sql_insert += "');";

    mysql->query(sql_insert);

    LogDebug("sql_insert=%s",sql_insert.c_str());
	LogError("mysql->getAffectedRows(): %d ,mysql->getErrStr(): %s",mysql->getAffectedRows(),mysql->getErrStr().c_str());
		
	//查询注册用户是否存在，若存在返回 -2
	if (mysql->getAffectedRows() == 0)
	{
        LogError("sql_insert exec fail");
		ret = -1;
        return ret;
	}

	return ret;
}

void RegisterCgi::returnRegStatus(string status_num)
{
	Json::Value root;
	
	//根节点属性
	root["code"] = Json::Value(status_num);

	string out = root.toStyledString();
	LogInfo("out = %s",out.c_str());
	printf("%s",out.c_str());

}


int RegisterCgi::regExec()
{
	    //阻塞等待用户连接
    while (FCGI_Accept() >= 0)
    {
        char *contentLength = getenv("CONTENT_LENGTH");
        int len;
        

        printf("Content-type: text/html\r\n\r\n");

        if( contentLength == NULL )
        {
            len = 0;
        }
        else
        {
            len = atoi(contentLength); //字符串转整型
        }

        if (len <= 0)//没有登陆用户信息
        {
            printf("No data from standard input.<p>\n");
            LogError("len = 0, No data from standard input");
        }
        else //获取登陆用户信息
        {
            char buf[4*1024] = {0};
            int ret = 0;
			ret = FCGI_fread(buf, len, 1, FCGI_stdin);
            //ret = fread(buf, 1, len, stdin); //从标准输入(web服务器)读取内容
			//printf("buf = %s\n",buf);
             if(ret == 0)
            {
                LogError("fread(buf, 1, len, stdin) err");
                continue;
            }
            LogInfo("buf = %s",buf);

             //注册用户，成功返回0，失败返回-1, 该用户已存在返回-2
            /*
            注册：
                成功：{"code":"002"}
                该用户已存在：{"code":"003"}
                失败：{"code":"004"}
            */ 
			string m_buf = buf;
            ret = userReg(m_buf);

            if (ret == 0) //登陆成功
            {
                //返回前端注册情况， 002代表成功
                returnRegStatus("002");
            }
            else if(ret == -1)
            {
                //返回前端注册情况， 004代表失败
                returnRegStatus("004");
            }
            else if(ret == -2)
            {
                //返回前端注册情况， 003代表用户已存在
                returnRegStatus("003");
            }

        }
    }
}

