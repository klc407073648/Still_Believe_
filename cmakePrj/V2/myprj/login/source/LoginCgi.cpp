#include "LoginCgi.h"
Config* LoginCgi::m_Config;
MySQL::ptr LoginCgi::mysql=NULL;

CREATE_STATIC_LOGGERMANAGER(LoginCgi);

LoginCgi::LoginCgi()
{
    m_Config = new Config(json_file);
    m_ConfigMap=m_Config->getConfigMap();

	LogInfo("LoginCgi Create!");
}

LoginCgi::~LoginCgi()
{
    delete m_Config;
	LogInfo("LoginCgi Finish!");
}

int LoginCgi::getLoginInfo(const string& login_buf,string& user,string& pwd)
{
    LogDebug("getLoginInfo begin");

	Json::Reader reader;
	Json::Value root;
 
    /*
    buf = {
    "pwd": "e10adc3949ba59abbe56e057f20f883e",
    "user": "kevin"
    }
    */

	if (reader.parse(login_buf, root))
	{
        user = root["user"].asString();
	    pwd = root["pwd"].asString();
       
	    LogInfo("user=%s,pwd=%s",user.c_str(),pwd.c_str());
    
		if( user=="" || pwd =="" )
		{
			LogError("user or pwd is null,please check!");
			return -1;
		}
	   
	   LogInfo("Reading Complete!");
	}
	else
	{
        LogError("parse error!");
		return -1;
	}
 
    LogDebug("getLoginInfo end");
	return 0;
}

MySQL::ptr LoginCgi::getMySQL()
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

int LoginCgi::checkUserAndPwd(const string& user,const string& pwd )
{
    LogDebug("checkUserAndPwd begin");

    char sql_cmd[SQL_MAX_LEN] = {0};
    string sql_str;
    int ret = 0;

    getMySQL();//获取mysql实例
  
	if (!mysql->connect())
	{
		LogError("mysql connect fail!");
		return -1;
	}

	sprintf(sql_cmd, "select password from user where name = '%s'", user.c_str());
    sql_str = sql_cmd;
	ISQLData::ptr m_ptr=mysql->query(sql_str);
	
    int num = mysql->getAffectedRows();
	
	//查询数据库是否存在对应记录，如果存在，校验用户成功
	if ( num > 0 )
	{
        m_ptr->next();
        if(m_ptr->getString(0) == pwd)
        {
            ret = 0;
        }
		else
        {
            ret = -1;	
        }
        LogInfo("sql_str:[%s] exec success!",sql_str.c_str());
        LogInfo("m_ptr->getString(0): %s ,pwd : %s",m_ptr->getString(0).c_str(),pwd.c_str());
        
	}
	//登陆失败
	else
	{	
        LogInfo("sql_str: %s,affectedRows is %d, error num is %d : info is %s",\
        sql_str.c_str(),num,mysql->getErrno(),mysql->getErrStr().c_str());	
        ret = -1;
	}

    LogDebug("checkUserAndPwd end");
	return ret;
}

int LoginCgi::setToken(const string& user, string& token)
{
    LogDebug("setToken begin");
	string redis_ip = m_ConfigMap["redis_ip"];
	int  redis_port = atoi((m_ConfigMap["redis_port"]).c_str());
	CMyRedis::ptr myRedis = CMyRedis::getInstance(redis_ip,redis_port);

	int ret = 0;
	if(!myRedis->CheckConenct())
    {
       LogError("redis connect error!");
       ret = -1;
       return -1;
    }
	
	LogInfo("redis connect success, ip = %s , port =%d",redis_ip.c_str(),redis_port);


    //产生4个1000以内的随机数
    int rand_num[4] = {0};
    int i = 0;

    //设置随机种子
    srand((unsigned int)time(NULL));
    for(i = 0; i < 4; ++i)
    {
        rand_num[i] = rand()%1000;//随机数
    }

    char tmp[1024] = {0};
	
    sprintf(tmp, "%s%d%d%d%d", user, rand_num[0], rand_num[1], rand_num[2], rand_num[3]);
    LogInfo("tmp = %s", tmp);
	
    //加密
    char enc_tmp[1024*2] = {0};
    int enc_len = 0;
    ret = DesEncode::DesEnc((unsigned char *)tmp, strlen(tmp), (unsigned char *)enc_tmp, &enc_len);
    if(ret != 0)
    {
        LogError( "DesEnc error");
		ret = -1;
        return ret;
    }

    //to base64
    char base64[1024*3] = {0};
    BASE64::base64Encode((const unsigned char *)enc_tmp, enc_len, base64); //base64编码

	LogInfo("base64 = %s", base64);

    //to md5
    MD5_CTX md5;
    MD5::MD5Init(&md5);
    unsigned char decrypt[16];
    MD5::MD5Update(&md5, (unsigned char *)base64, strlen(base64) );
    MD5::MD5Final(&md5, decrypt);


    char str[100] = { 0 };
    for (i = 0; i < 16; i++)
    {
        sprintf(str, "%02x", decrypt[i]);
		token = token +str;
		//strcat(token, str);
    }

	int expire_time =atoi((m_ConfigMap["log4cpp_expire_time"]).c_str());
    LogInfo("expire_time = %d", expire_time);
    // redis保存此字符串，用户名：token, 有效时间为24小时
	
	myRedis->Setex(user,expire_time,token);
	
    LogDebug("setToken end");
	
	return ret;
}
void LoginCgi::returnLoginStatus(const string& statusNum, const string& token)
{
    LogDebug("returnLoginStatus begin");

	Json::Value root;
	
	//根节点属性
	root["code"] = Json::Value(statusNum);
	root["token"] = Json::Value(token);

	string out = root.toStyledString();
	LogInfo("out = %s",out.c_str());
	printf("%s",out.c_str());

    LogDebug("returnLoginStatus end");
}

int LoginCgi::loginExec()
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

            //获取登陆用户的信息
            getLoginInfo(buf,mFileInfo.user,mFileInfo.pwd);
            LogInfo("user = %s, pwd = %s", mFileInfo.user.c_str(), mFileInfo.pwd.c_str());
            
            //登陆判断，成功返回0，失败返回-1
            ret = checkUserAndPwd( mFileInfo.user, mFileInfo.pwd );
            if (ret == 0) //登陆成功,生成token
            {
                //生成token字符串
                ret = setToken(mFileInfo.user, mFileInfo.token);
                LogInfo("token = %s", mFileInfo.token.c_str());
            }

            if(ret == 0)
            {
                //返回前端登陆情况， 000代表成功
                returnLoginStatus("000", mFileInfo.token);
            }
            else
            {
                //返回前端登陆情况， 001代表失败
                returnLoginStatus("001", "fail");
            } 
        }
    }
}

