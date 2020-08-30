#include "DealShareFileCgi.h"

Config* DealShareFileCgi::m_Config;
//MySQL::ptr DealShareFileCgi::mysql;
//CMyRedis::ptr DealShareFileCgi::myRedis;

CREATE_STATIC_LOGGERMANAGER(DealShareFileCgi);

DealShareFileCgi::DealShareFileCgi()
{
	LogInfo("DealShareFileCgi Create!");
    init();
}

void DealShareFileCgi::init()
{
    LogDebug("init begin");

    m_Config = new Config(json_file);
    m_ConfigMap=m_Config->getConfigMap();

    LogDebug("init end");
}

DealShareFileCgi::~DealShareFileCgi()
{
	delete m_Config;
	LogInfo("DealShareFileCgi Finish!");
}


int DealShareFileCgi::getJsonInfo(const string& dealsharefile_buf , string& user ,string& md5 , string& fileName)
{
    LogDebug("getJsonInfo begin");

	Json::Reader reader;
	Json::Value root;
 
     /*json数据如下
        {
        "user": "yoyo",
        "md5": "xxx",
        "filename": "xxx"
        }
    */
	
	if (reader.parse(dealsharefile_buf, root))
	{
	   user = root["user"].asString();
       md5  = root["md5"].asString();
       fileName = root["filename"].asString();
       
	   LogInfo("json parse user=%s , md5=%s , filename=%s",user.c_str(),md5.c_str(),fileName.c_str());
    
		if( user =="" || md5 =="" || fileName =="" )
		{
			LogError("user or md5 or fileName  is null,please check!");
			return -1;
		}
	   
	   LogInfo("Reading Complete!");
	}
	else
	{
        LogError("parse error!");
		return -1;
	}
 
    LogDebug("getJsonInfo end");

	return 0;
}

int DealShareFileCgi::pvFile(const string& md5 ,const string& fileName)
{

    int ret = 0;
    char sql_cmd[SQL_MAX_LEN] = {0};
    char tmp[512] = {0};
    int ret2 = 0;
    MYSQL_RES *res_set = NULL;
    MYSQL_ROW row;
	int pv = 0;
	char fileid[1024];
    string fileId;

    //连接redis数据库
    CMyRedis::ptr myRedis = CMyRedis::getInstance();
    myRedis->Connect(m_ConfigMap["redis_ip"],atoi((m_ConfigMap["redis_port"]).c_str()));

    if(!myRedis->CheckConenct())
    {
       LogError("redis connect error ");
       ret = -1;
       return ret;
    }

    //连接mysql数据库
    CMySQL::ptr mysql(new CMySQL());

    mysql->initDB(m_ConfigMap["mysql_host"],m_ConfigMap["mysql_user"],m_ConfigMap["mysql_passwd"],m_ConfigMap["mysql_dbname"]);
    
	if (!mysql->getConn())
	{
		LogError("mysql connect fail");
		ret = -1;
        return ret;
	}

    //文件标示，md5+文件名
    sprintf(fileid, "%s%s", md5.c_str(), fileName.c_str());
    fileId = fileid;
    //===1、mysql的下载量+1(mysql操作)
    //sql语句
    //查看该共享文件的pv字段
    sprintf(sql_cmd, "select pv from share_file_list where md5 = '%s' and filename = '%s'",md5.c_str(), fileName.c_str());
    ret2 = mysql->exeSQL(sql_cmd, tmp); 

    if(ret2 == 0)
    {
        pv = atoi(tmp); //pv字段
    }
    else
    {
        LogError("mysqlStr[%s] exec fail , the error info is [%s]",sql_cmd,mysql->getErrStr().c_str());
        ret = -1;
        return ret;
    }

    //更新该文件pv字段，+1
    sprintf(sql_cmd, "update share_file_list set pv = %d where md5 = '%s' and filename = '%s'111", pv+1, md5.c_str(), fileName.c_str());

    if(mysql->query(sql_cmd) != 0)
    {
        LogError("mysqlStr[%s] exec fail , the error info is [%s]",sql_cmd,mysql->getErrStr().c_str());
        ret = -1;
        return ret;
    }

    //===2、判断元素是否在集合中(redis操作)
    ret2 = myRedis->ZLexCount( FILE_PUBLIC_ZSET, fileId);
    if(ret2 == 1) //存在
    {//===3、如果存在，有序集合score+1
        ret = myRedis->ZIncre( FILE_PUBLIC_ZSET, fileId);
        if(ret != 0)
        {
            LogError("myRedis->ZIncre exec fail");
        }
    }
    else if(ret2 == 0) //不存在
    {//===4、如果不存在，从mysql导入数据
        //===5、redis集合中增加一个元素(redis操作)
        myRedis->ZAdd(FILE_PUBLIC_ZSET, pv+1, fileId);

        //===6、redis对应的hash也需要变化 (redis操作)
        //     fileid ------>  filename
        myRedis->HSet(FILE_NAME_HASH, fileId, fileName);

    }
    else//出错
    {
        ret = -1;
        return ret;
    }

    return ret;

}

int DealShareFileCgi::cancelShareFile(const string& user,const string& md5 ,const string& fileName)
{
    LogDebug("cancelShareFile begin");

    int ret = 0;
    char sql_cmd[SQL_MAX_LEN] = {0};
    MYSQL_RES *res_set = NULL;
    MYSQL_ROW row;
    char fileid[1024] = {0};
    string fileId;
    int count = 0;
    char tmp[512] = {0};
    int ret2=0;

    //连接redis数据库
    CMyRedis::ptr myRedis = CMyRedis::getInstance();
    myRedis->Connect(m_ConfigMap["redis_ip"],atoi((m_ConfigMap["redis_port"]).c_str()));

    if(!myRedis->CheckConenct())
    {
       LogError("redis connect error ");
       ret = -1;
       return ret;
    }

    //连接mysql数据库
    CMySQL::ptr mysql(new CMySQL());

    mysql->initDB(m_ConfigMap["mysql_host"],m_ConfigMap["mysql_user"],m_ConfigMap["mysql_passwd"],m_ConfigMap["mysql_dbname"]);
    
	if (!mysql->getConn())
	{
		LogError("mysql connect fail");
		ret = -1;
        return ret;
	}

     //文件标示，md5+文件名
    sprintf(fileid, "%s%s", md5.c_str(), fileName.c_str());
    fileId = fileid;
    //===1、mysql记录操作
    //sql语句
    sprintf(sql_cmd, "update user_file_list set shared_status = 0 where user = '%s' and md5 = '%s' and filename = '%s'", user.c_str(), md5.c_str(), fileName.c_str());

    if(mysql->query(sql_cmd) != 0)
    {
        LogError("mysqlStr[%s] exec fail , the error info is [%s]",sql_cmd,mysql->getErrStr().c_str());
        ret = -1;
        return ret;
    }

    //查询共享文件数量
    sprintf(sql_cmd, "select count from user_file_count where user = '%s'", "xxx_share_xxx_file_xxx_list_xxx_count_xxx");
    
    ret2 = mysql->exeSQL(sql_cmd, tmp); 
    if(ret2 != 0)
    {
        LogError("mysqlStr[%s] exec fail , the error info is [%s]",sql_cmd,mysql->getErrStr().c_str());
        ret = -1;
        return ret;
    }


    //更新用户文件数量count字段
    count = atoi(tmp);
    if(count == 1)
    {
        //删除数据
        sprintf(sql_cmd, "delete from user_file_count where user = '%s'", "xxx_share_xxx_file_xxx_list_xxx_count_xxx");
    }
    else
    {
        sprintf(sql_cmd, "update user_file_count set count = %d where user = '%s'", count-1, "xxx_share_xxx_file_xxx_list_xxx_count_xxx");
    }

    if(mysql->query(sql_cmd) != 0)
    {
        LogError("mysqlStr[%s] exec fail , the error info is [%s]",sql_cmd,mysql->getErrStr().c_str());
        ret = -1;
        return ret;
    }

    //删除在共享列表的数据
    sprintf(sql_cmd, "delete from share_file_list where user = '%s' and md5 = '%s' and filename = '%s'",  user.c_str(), md5.c_str(), fileName.c_str());
    if(mysql->query(sql_cmd) != 0)
    {
        LogError("mysqlStr[%s] exec fail , the error info is [%s]",sql_cmd,mysql->getErrStr().c_str());
        ret = -1;
        return ret;
    }

    //===2、redis记录操作
    //有序集合删除指定成员 reply->integer成功返回1，reply->integer失败返回0
	int retVal = myRedis->ZRem(FILE_PUBLIC_ZSET, fileId);
	
    if(retVal != 1)
    {
        LogError("myRedis->ZRem exec fail");
		ret = -1;
        return ret;
    }

    //从hash移除相应记录
    retVal = myRedis->HDel(FILE_NAME_HASH, fileId);
    if(retVal != 1)
    {
        LogError("myRedis->HDel exec fail");
		ret = -1;
        return ret;
    }

    return ret;

}

void DealShareFileCgi::returnStatus(string status_num)
{
	Json::Value root;
	
	//根节点属性
    root["code"] = Json::Value(status_num);

	string out = root.toStyledString();
	LogInfo("out = %s",out.c_str());
	printf("%s",out.c_str());

}

int DealShareFileCgi::saveFile(const string& user,const string& md5 ,const string& fileName)
{
    int ret = 0;
    char sql_cmd[SQL_MAX_LEN] = {0};
    int ret2 = 0;
    char tmp[512] = {0};
    int count;

    //连接mysql数据库
    CMySQL::ptr mysql(new CMySQL());

    mysql->initDB(m_ConfigMap["mysql_host"],m_ConfigMap["mysql_user"],m_ConfigMap["mysql_passwd"],m_ConfigMap["mysql_dbname"]);
    
	if (!mysql->getConn())
	{
		LogError("mysql connect fail");
		ret = -1;
        return ret;
	}
    //查看此用户，文件名和md5是否存在，如果存在说明此文件存在
    sprintf(sql_cmd, "select * from user_file_list where user = '%s' and md5 = '%s' and filename = '%s'", user.c_str(), md5.c_str(), fileName.c_str());

    ret2 = mysql->exeSQL(sql_cmd, NULL); 
    if(ret2 == 2)
    {
        LogError("%s[filename:%s, md5:%s] already exist",user.c_str(), md5.c_str(), fileName.c_str());
        ret = -2;
        return ret;
    }

     //文件信息表，查找该文件的计数器
    sprintf(sql_cmd, "select count from file_info where md5 = '%s'", md5.c_str());
    ret2 = mysql->exeSQL(sql_cmd, tmp); 
    if(ret2 != 0)
    {
        LogError("mysqlStr[%s] exec fail , the error info is [%s]",sql_cmd,mysql->getErrStr().c_str());
        ret = -2;
        return ret;
    }

    count = atoi(tmp);

    //1、修改file_info中的count字段，+1 （count 文件引用计数）
    sprintf(sql_cmd, "update file_info set count = %d where md5 = '%s'", count+1, md5);
    if(mysql->query( sql_cmd) != 0)
    {
        LogError("mysqlStr[%s] exec fail , the error info is [%s]",sql_cmd,mysql->getErrStr().c_str());
        ret = -1;
        return ret;
    }

    //2、user_file_list插入一条数据
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
    sprintf(sql_cmd, "insert into user_file_list(user, md5, createtime, filename, shared_status, pv) values ('%s', '%s', '%s', '%s', %d, %d)", user.c_str(), md5.c_str(), time_str, fileName.c_str(), 0, 0);
    if(mysql->query(sql_cmd) != 0)
    {
        LogError("mysqlStr[%s] exec fail , the error info is [%s]",sql_cmd,mysql->getErrStr().c_str());
        ret = -1;
        return ret;
    }

    //3、查询用户文件数量，更新该字段
    sprintf(sql_cmd, "select count from user_file_count where user = '%s'", user.c_str());
    count = 0;
    //返回值： 0成功并保存记录集，1没有记录集，2有记录集但是没有保存，-1失败
    ret2 = mysql->exeSQL(sql_cmd, tmp);  //指向sql语句
    if(ret2 == 1) //没有记录
    {
        //插入记录
        sprintf(sql_cmd, " insert into user_file_count (user, count) values('%s', %d)", user.c_str(), 1);
    }
    else if(ret2 == 0)
    {
        //更新用户文件数量count字段
        count = atoi(tmp);
        sprintf(sql_cmd, "update user_file_count set count = %d where user = '%s'", count+1, user.c_str());
    }

    if(mysql->query(sql_cmd) != 0)
    {
        LogError("mysqlStr[%s] exec fail , the error info is [%s]",sql_cmd,mysql->getErrStr().c_str());
        ret = -1;
        return ret;
    }

    return ret;
}

int DealShareFileCgi::dealsharefileExec()
{
    while (FCGI_Accept() >= 0)
    {

		// 获取URL地址 "?" 后面的内容
    
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
			char *query = getenv("QUERY_STRING");

			string query_content = query;
			int pos=query_content.find("cmd=");
			mFileInfo.cmd = query_content.substr(pos+4);
			
            char buf[4*1024] = {0};
            int ret = 0;
			ret = FCGI_fread(buf, len, 1, FCGI_stdin);
           
            if(ret == 0)
            {
                LogError("fread(buf, 1, len, stdin) err");
                continue;
            }

            LogInfo("buf = %s",buf);
			
            getJsonInfo( buf, mFileInfo.user, mFileInfo.md5, mFileInfo.fileName);

            LogInfo(" cmd = %s ,user = %s ,md5 = %s,filename = %s", mFileInfo.cmd.c_str(), mFileInfo.user.c_str(), mFileInfo.md5.c_str(), mFileInfo.fileName.c_str());
		
		    int ret_type;
            string status_num;

            if ( mFileInfo.cmd == "pv")
            {
               ret_type = pvFile(mFileInfo.md5,mFileInfo.fileName);

               if(ret_type == 0)
               {
                   status_num="016";
               }
               else
               {
                   status_num="017";
               }
               
            }
            else if ( mFileInfo.cmd == "cancel")
            {
                ret_type = cancelShareFile(mFileInfo.user,mFileInfo.md5,mFileInfo.fileName);
                if(ret_type == 0)
               {
                   status_num="018";
               }
               else
               {
                   status_num="019";
               }
            }

            else if ( mFileInfo.cmd == "save")
            {
                ret_type = saveFile(mFileInfo.user,mFileInfo.md5,mFileInfo.fileName);

               if(ret_type == 0)
               {
                   status_num="020";
               }
               else if(ret_type == -1)
               {
                   status_num="022";
               }
               else if(ret_type == -2)
               {
                   status_num="021";
               }
            }
            
             returnStatus(status_num);
         

        }

    }

     /* while */

    return 0;
}


