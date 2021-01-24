#include "RegisterCgi.h"

Config* RegisterCgi::m_Config;
MySQL::ptr RegisterCgi::mysql=NULL;

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

int RegisterCgi::getRegInfo(const string& reg_buf,string& user, string& nickName, string& pwd, string& phone ,string& email)
{
    LogDebug("getRegInfo begin");

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
	   user = root["userName"].asString();
       nickName = root["nickName"].asString();
       pwd = root["firstPwd"].asString();
       phone = root["phone"].asString();
       email = root["email"].asString();
       
	   LogInfo("user=%s,nickName=%s,pwd=%s,phone=%s,email=%s",user.c_str(),nickName.c_str()\
       ,pwd.c_str(),phone.c_str(),email.c_str());
	   
		if( user =="" || nickName =="" || pwd =="" || phone =="" || email =="" )
		{
			LogError("user or nick_name or pwd or phone or email is null,please check!");
			return -1;
		}
	   
	   LogInfo("Reading Complete!");
	}
	else
	{
        LogError("parse error!");
		return -1;
	}

    LogDebug("getRegInfo end");
 
	return 0;
}

MySQL::ptr RegisterCgi::getMySQL()
{
    if(mysql == NULL)
    {
        std::map<std::string, std::string> params;

        params["host"] = m_ConfigMap["mysql_host"];
        params["user"] = m_ConfigMap["mysql_user"];
        params["passwd"] = m_ConfigMap["mysql_passwd"];
        params["dbname"] = m_ConfigMap["mysql_dbname"];

        mysql= MySQL::getInstance(params);
    }
    return  mysql;
}

int RegisterCgi::userReg(const string& reg_buf)
{
    LogDebug("userReg begin");

    int ret = 0;
    char sql_cmd[SQL_MAX_LEN] = {0};
    string sql_str;

	getMySQL();//获取mysql实例
    
	if (!mysql->connect())
	{
		LogError("mysql connect fail!");
		return -1;
	}

    ret = getRegInfo(reg_buf,mFileInfo.user,mFileInfo.nickName,mFileInfo.pwd,mFileInfo.phone,mFileInfo.email);

    if(ret != 0)
    {
        LogError("get register info error");
        return ret;
    }

	sprintf(sql_cmd, "select * from user where name = '%s'", mFileInfo.user.c_str());
    sql_str = sql_cmd;
    
    mysql->query(sql_str);
    int num = mysql->getAffectedRows();

	//查询数据库是否存在对应记录，存在则注册失败
	if (num > 0 || mysql->getErrno()!=0 )
	{
        LogError("user %s is already exist",mFileInfo.user.c_str());
		ret = -2;
        return ret;
	}

    LogInfo("sql_str:[%s] exec success,user %s is not exist",sql_str.c_str(),mFileInfo.user.c_str());
	
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
    sprintf(sql_cmd, "insert into user (name, nickname, password, phone, createtime, email) values \
    ('%s', '%s', '%s', '%s', '%s', '%s')", mFileInfo.user.c_str(), mFileInfo.nickName.c_str(),\
     mFileInfo.pwd.c_str(), mFileInfo.phone.c_str(), time_str ,mFileInfo.email.c_str());

    sql_str = sql_cmd;

    mysql->query(sql_str);
    num = mysql->getAffectedRows();

	//插入记录是否成功
	if ( num <= 0 || mysql->getErrno()!=0)
	{
        LogError("sql_str: %s,affectedRows is %d, error num is %d : info is %s",\
        sql_str.c_str(),num,mysql->getErrno(),mysql->getErrStr().c_str());
		ret = -1;
        return ret;
	}

    LogInfo("sql_str:[%s] exec success!",sql_str.c_str());

    LogDebug("userReg end");

	return ret;
}

void RegisterCgi::returnRegStatus(const string& statusNum)
{
	Json::Value root;
	
	//根节点属性
	root["code"] = Json::Value(statusNum);

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
            ret = userReg(buf);

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

