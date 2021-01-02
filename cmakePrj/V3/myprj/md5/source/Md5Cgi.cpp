#include "Md5Cgi.h"

Config* Md5Cgi::m_Config;
MySQL::ptr Md5Cgi::mysql=NULL;
//MySQL::ptr Md5Cgi::mysql;
//CMyRedis::ptr Md5Cgi::myRedis;

CREATE_STATIC_LOGGERMANAGER(Md5Cgi);

Md5Cgi::Md5Cgi()
{
	LogInfo("Md5Cgi Create!");
    init();
}

void Md5Cgi::init()
{
    LogDebug("init begin");

    m_Config = new Config(json_file);
    m_ConfigMap=m_Config->getConfigMap();

/*     std::map<std::string, std::string> params;

    params["host"] = m_ConfigMap["mysql_host"];
    params["user"] = m_ConfigMap["mysql_user"];
    params["passwd"] = m_ConfigMap["mysql_passwd"];
    params["dbname"] = m_ConfigMap["mysql_dbname"];

	MySQL::ptr mysql= MySQL::getInstance(params);
    
	if (!mysql->connect())
	{
		LogError("mysql connect fail");
	}

    CMyRedis::ptr myRedis = CMyRedis::getInstance();

    myRedis->Connect(m_ConfigMap["redis_ip"],atoi((m_ConfigMap["redis_port"]).c_str()));
    
    if (!myRedis->CheckConenct())
    {
        LogError("redis connect fail");
    } */

    LogDebug("init end");
}

Md5Cgi::~Md5Cgi()
{
	delete m_Config;
	LogInfo("Md5Cgi Finish!");
}


int Md5Cgi::getMd5Info(const string& md5_buf, string& user, string& token, string& md5, string& fileName)
{
    LogDebug("getMd5Info begin");

	Json::Reader reader;
	Json::Value root;
 
    //解析json中信息
    /*
     * {
        user:xxxx,
        token: xxxx,
        md5:xxx,
        fileName: xxx
       }
     */

	if (reader.parse(md5_buf, root))
	{
	   user = root["user"].asString();
       token = root["token"].asString();
       md5 = root["md5"].asString();
       fileName = root["fileName"].asString();
       
	   LogInfo("user=%s , token=%s , md5=%s , fileName=%s",user.c_str(),token.c_str()\
       ,md5.c_str(),fileName.c_str());
    
		if( user =="" || token =="" || md5 =="" || fileName =="" )
		{
			LogError("user or token or md5 or fileName is null,please check!");
			return -1;
		}
	   
	   LogInfo("Reading Complete!");
	}
	else
	{
        LogError("parse error!");
		return -1;
	}
 
    LogDebug("getMd5Info end");

	return 0;
}
MySQL::ptr Md5Cgi::getMySQL()
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

int Md5Cgi::dealMd5(const string& user , const string& md5, const string& fileName)
{
    LogDebug("dealMd5 begin");

    char sql_cmd[SQL_MAX_LEN] = {0};
    string sql_str;

    //如果有
    //1、修改file_info中的count字段，+1 （count 文件引用计数）
    //   update file_info set count = 2 where md5 = "bae488ee63cef72efb6a3f1f311b3743";
    //2、user_file_list插入一条数据

	getMySQL();//获取mysql实例
    
	if (!mysql->connect())
	{
		LogError("mysql connect fail");
        return -1;
	}
	
    //查看数据库file_info，对应md5码的文件是否存在
    sprintf(sql_cmd, "select count from file_info where md5 = '%s'", md5.c_str());
    sql_str = sql_cmd;
    mysql->query(sql_str);
    int num = mysql->getAffectedRows();

    if( num > 0) //有结果，说明服务器已经有此文件
    {
        //查看此用户是否已经有此文件，如果存在说明此文件已上传，无需再上传
        sprintf(sql_cmd, "select * from user_file_list where user = '%s' and md5 = '%s' and filename = '%s'", \
        user.c_str(), md5.c_str(), fileName.c_str());

        sql_str = sql_cmd;
        mysql->query(sql_str); 
        num = mysql->getAffectedRows();
        if( num > 0 ) //如果有结果，说明此用户已经保存此文件
        {
            LogError("%s[filename:%s, md5:%s] already exist", user.c_str(), fileName.c_str(), md5.c_str());
            return -2; //0秒传成功，-1出错，-2此用户已拥有此文件， -3秒传失败
        }

        LogInfo("sql_str:[%s] exec success!",sql_str.c_str());

        //1、修改file_info中的count字段，+1 （count 文件引用计数）
        sprintf(sql_cmd, "update file_info set count = count + 1 where md5 = '%s'" , md5.c_str());//前置++

        sql_str = sql_cmd;
        mysql->query(sql_str); 
        num = mysql->getAffectedRows();

         if ( num <= 0 || mysql->getErrno()!=0 )
	    {
            LogError("sql_str: %s,affectedRows is %d, error num is %d : info is %s",\
            sql_str.c_str(),num,mysql->getErrno(),mysql->getErrStr().c_str());
            return -1;
	    } 

        LogInfo("sql_str:[%s] exec success!",sql_str.c_str());
       
        //2、user_file_list, 用户文件列表插入一条数据
        //当前时间戳
        struct timeval tv;
        struct tm* ptm;
        char time_str[128];

        //使用函数gettimeofday()函数来得到时间。它的精度可以达到微妙
        gettimeofday(&tv, NULL);
        ptm = localtime(&tv.tv_sec);//把从1970-1-1零点零分到当前时间系统所偏移的秒数时间转换为本地时间
        //strftime() 函数根据区域设置格式化本地时间/日期，函数的功能将时间格式化，或者说格式化一个时间字符串
        strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", ptm);

        //sql语句
        /*
        -- =============================================== 用户文件列表
        -- user	文件所属用户
        -- md5 文件md5
        -- createtime 文件创建时间
        -- filename 文件名字
        -- shared_status 共享状态, 0为没有共享， 1为共享
        -- pv 文件下载量，默认值为0，下载一次加1
        */
        sprintf(sql_cmd, "insert into user_file_list(user, md5, createtime, filename, shared_status, pv) values ('%s', '%s', '%s', '%s', %d, %d)", \
        user.c_str(), md5.c_str(), time_str, fileName.c_str(), 0, 0);

        sql_str = sql_cmd;
        mysql->query(sql_str);
        num = mysql->getAffectedRows();

        if ( num <= 0 || mysql->getErrno()!=0 )
	    {
            LogError("sql_str: %s,affectedRows is %d, error num is %d : info is %s",\
            sql_str.c_str(),num,mysql->getErrno(),mysql->getErrStr().c_str());
            return -1;
	    } 

        LogInfo("sql_str:[%s] exec success!",sql_str.c_str());
       

        //查询用户文件数量
        sprintf(sql_cmd, "select count from user_file_count where user = '%s'", user);

        sql_str = sql_cmd;
        mysql->query(sql_str);
        num = mysql->getAffectedRows();

        if ( num < 0 || mysql->getErrno()!=0 )
	    {
            LogError("sql_str: %s,affectedRows is %d, error num is %d : info is %s",\
            sql_str.c_str(),num,mysql->getErrno(),mysql->getErrStr().c_str());
            return -1;
	    } 

        LogInfo("sql_str:[%s] exec success!",sql_str.c_str());

        //返回值： 0成功并保存记录集，1没有记录集，2有记录集但是没有保存，-1失败
  
        if( num == 0 ) //没有记录
        {
            //数据库插入此记录
            sprintf(sql_cmd, " insert into user_file_count (user, count) values('%s', %d)", user, 1);
        }
        else 
        {
            //更新用户文件数量count字段
            sprintf(sql_cmd, "update user_file_count set count = count +1 where user = '%s'", user);
        }

        sql_str = sql_cmd;
        mysql->query(sql_str);
        num = mysql->getAffectedRows();

        if ( num <= 0 || mysql->getErrno()!=0 )
	    {
            LogError("sql_str: %s,affectedRows is %d, error num is %d : info is %s",\
            sql_str.c_str(),num,mysql->getErrno(),mysql->getErrStr().c_str());
            return -1;
	    } 

        LogInfo("sql_str:[%s] exec success!",sql_str.c_str());

    }
    else //没有结果，秒传失败
    {
       return -3;//0秒传成功，-1出错，-2此用户已拥有此文件， -3秒传失败
    }
    LogDebug("dealMd5 end");

    return 0;
}
/*
    秒传文件：
        文件已存在：{"code":"005"}
        秒传成功：  {"code":"006"}
        秒传失败：  {"code":"007"}

*/
void Md5Cgi::returnMd5Status(const string& statusNum)
{
	Json::Value root;
	
	//根节点属性
	root["code"] = Json::Value(statusNum);

	string out = root.toStyledString();
	LogInfo("out = %s",out.c_str());
	printf("%s",out.c_str());

}

int Md5Cgi::verifyToken(const string& user, const string& token)
{
    LogDebug("verifyToken begin");

	string redis_ip = m_ConfigMap["redis_ip"];
	int  redis_port = atoi((m_ConfigMap["redis_port"]).c_str());
	CMyRedis::ptr myRedis = CMyRedis::getInstance(redis_ip,redis_port);

	int ret = 0;
	if(!myRedis->CheckConenct())
    {
       LogError("redis connect error!");
       ret = -1;
       return ret;
    }
	
	LogInfo("redis connect success, ip = %s , port =%d",redis_ip.c_str(),redis_port);
	
    string retStr = myRedis->Get(user);
	
	LogInfo("retStr = %s",retStr.c_str());
    if( retStr == token )
    {
        return 0;
    }
    else
    {
        return -1;
    }

	LogDebug("verifyToken end");

    return 0;
}

int Md5Cgi::md5Exec()
{
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

        if (len <= 0)//没有数据
        {
            printf("No data from standard input.<p>\n");
            LogError("len = 0, No data from standard input");
        }
        else //获取md5信息
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
			
			ret = getMd5Info(buf, mFileInfo.user, mFileInfo.token, mFileInfo.md5, mFileInfo.fileName);//解析json中信息
			if ( ret < 0 )
            {
                LogError("getMd5Info fail");
                continue;
            }
		
			LogInfo("getMd5Info success [user = %s, token = %s, md5 = %s, filename = %s]", \
            mFileInfo.user.c_str(),  mFileInfo.token.c_str(), mFileInfo.md5.c_str(), mFileInfo.fileName.c_str());
			
            //验证登陆token，成功返回0，失败-1
            ret = verifyToken(mFileInfo.user, mFileInfo.token); 

            LogInfo("verifyToken return is %d",ret);

            string status_num;
            if ( ret == 0 )
            {
                int ret_type =dealMd5(mFileInfo.user.c_str(), mFileInfo.md5.c_str(), mFileInfo.fileName.c_str());

                  if(ret_type == 0)
                  {
                    status_num= "006"; 
                  }
                  else if(ret_type == -2)
                  {
                    status_num= "005";
                  }
                  else
                  {
                    status_num= "007";
                  }
            }
            else
            {
                status_num = "111"; //token验证失败错误码
            }

            returnMd5Status(status_num);           

        }

    } /* while */

    return 0;
}


