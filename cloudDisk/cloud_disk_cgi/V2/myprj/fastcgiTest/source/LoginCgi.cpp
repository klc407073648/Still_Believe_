#include "LoginCgi.h"

map<string,string> LoginCgi::m_ConfigMap;

CREATE_STATIC_LOGGERMANAGER(LoginCgi);

LoginCgi::LoginCgi(map<string,string> cfg)
{
	m_ConfigMap =  cfg;
	LogInfo("mysql_host=%s",(m_ConfigMap["mysql_host"]).c_str());
	LogInfo("redis_ip=%s",(m_ConfigMap["redis_ip"]).c_str());
}

int LoginCgi::getLoginInfo(const string& buf, string& user, string& pwd)
{
	Json::Reader reader;
	Json::Value root;
 
	if (reader.parse(buf, root))
	{
		string m_pwd = root["pwd"].asString();
       string m_user = root["user"].asString();
       //string m_pwd = root["pwd"].asString();
	   LogInfo("user=%s,pwd=%s",m_user.c_str(),m_pwd.c_str());
	   user = m_user;
	   pwd = m_pwd;
    
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
 
	return 0;
}

int LoginCgi::checkUserAndPwd(string& user, string& pwd )
{
	MySQL::ptr mysql= MySQL::getInstance();
	if (!mysql->connect())
	{
		LogError("mysql connect fail");
		return -1;
	}
	int ret = 0;

	string sql_query("");
	sql_query += "select * from user where name='";
	sql_query += user;
	sql_query += "' AND password='";
	sql_query += pwd;
	sql_query += "';";

	mysql->query(sql_query);
		
    LogDebug("sql_query=%s",sql_query.c_str());
	LogError("mysql->getAffectedRows(): %d ,mysql->getErrStr(): %s",mysql->getAffectedRows(),mysql->getErrStr().c_str());
		
	//查询数据库是否存在对应记录，如果存在，校验用户成功
	if (mysql->getAffectedRows() != 0)
	{
		ret = 0;
	}
	//登陆失败
	else
	{
		ret = -1;		
	}
	return ret;
}

int LoginCgi::setToken(string& user, string& token)
{
	CMyRedis::ptr myRedis = CMyRedis::getInstance();

    int ret = 0;
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
    LogInfo("tmp = %s\n", tmp);
	
    //加密
    char enc_tmp[1024*2] = {0};
    int enc_len = 0;
    ret = DesEncode::DesEnc((unsigned char *)tmp, strlen(tmp), (unsigned char *)enc_tmp, &enc_len);
    if(ret != 0)
    {
        LogError( "DesEnc error\n");
        return -1;
    }

    //to base64
    char base64[1024*3] = {0};
    BASE64::base64Encode((const unsigned char *)enc_tmp, enc_len, base64); //base64编码

	LogInfo("base64 = %s\n", base64);

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

    // redis保存此字符串，用户名：token, 有效时间为24小时
	myRedis->Setex(user,86400,token);
 
}
void LoginCgi::returnLoginStatus(string status_num, string token)
{
	Json::Value root;
	
	//根节点属性
	root["code"] = Json::Value(status_num);
	root["token"] = Json::Value(token);

	string out = root.toStyledString();
	LogInfo("out = %s",out.c_str());
	printf("%s",out.c_str());

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
            LogInfo("buf = %s\n",buf);

            //获取登陆用户的信息
            string user;
            string pwd;
            getLoginInfo(buf, user, pwd);
            LogInfo("user = %s, pwd = %s", user.c_str(), pwd.c_str());
            
			string token;
            //登陆判断，成功返回0，失败返回-1
            ret = checkUserAndPwd( user, pwd );
            if (ret == 0) //登陆成功
            {
                //生成token字符串
                //memset(token, 0, sizeof(token));
                ret = setToken(user, token);
                LogInfo("token = %s\n", token.c_str());

            }

            if(ret == 0)
            {
                //返回前端登陆情况， 000代表成功
                returnLoginStatus("000", token);
            }
            else
            {
                //返回前端登陆情况， 001代表失败
                returnLoginStatus("001", "fail");
            } 
        }
    }
}

