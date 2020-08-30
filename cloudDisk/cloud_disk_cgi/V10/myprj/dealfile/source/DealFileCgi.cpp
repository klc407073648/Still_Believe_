#include "DealFileCgi.h"

Config* DealFileCgi::m_Config;
//MySQL::ptr DealFileCgi::mysql;
//CMyRedis::ptr DealFileCgi::myRedis;

CREATE_STATIC_LOGGERMANAGER(DealFileCgi);

DealFileCgi::DealFileCgi()
{
	LogInfo("DealFileCgi Create!");
    init();
}

void DealFileCgi::init()
{
    LogDebug("init begin");

    m_Config = new Config(json_file);
    m_ConfigMap=m_Config->getConfigMap();

    LogDebug("init end");
}

DealFileCgi::~DealFileCgi()
{
	delete m_Config;
	LogInfo("DealFileCgi Finish!");
}


int DealFileCgi::getJsonInfo(const string& dealfile_buf, string& user, string& token ,string& md5, string& fileName)
{
    LogDebug("getJsonInfo begin");

	Json::Reader reader;
	Json::Value root;
 
     /*json数据如下
    {
    "user": "yoyo",
    "token": "xxxx",
    "md5": "xxx",
    "filename": "xxx"
    }
    */

	if (reader.parse(dealfile_buf, root))
	{
	   user = root["user"].asString();
       token = root["token"].asString();
       md5 = root["md5"].asString();
       fileName = root["filename"].asString();
       
	   LogInfo("json info : user=%s , token=%s , md5=%s , filename=%s", \
       user.c_str(),token.c_str(),md5.c_str(),fileName.c_str());
    
		if( user =="" || token =="" || md5 =="" || fileName ==""  )
		{
			LogError("user or token or md5 or filename is null,please check!");
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


int DealFileCgi::shareFile(const string& user,const string& md5,const string& fileName)
{
	LogDebug("shareFile enter");
     /*
    a)先判断此文件是否已经分享，判断集合有没有这个文件，如果有，说明别人已经分享此文件，中断操作(redis操作)
    b)如果集合没有此元素，可能因为redis中没有记录，再从mysql中查询，如果mysql也没有，说明真没有(mysql操作)
    c)如果mysql有记录，而redis没有记录，说明redis没有保存此文件，redis保存此文件信息后，再中断操作(redis操作)
    d)如果此文件没有被分享，mysql保存一份持久化操作(mysql操作)
    e)redis集合中增加一个元素(redis操作)
    f)redis对应的hash也需要变化 (redis操作)
    */
    int ret = 0;
    int ret2 = 0;
    int count = 0;
    char tmp[512] = {0};
    char sql_cmd[SQL_MAX_LEN] = {0};
    char fileid[1024] = {0};
    MYSQL *conn = NULL;

   CMyRedis::ptr myRedis = CMyRedis::getInstance();

   myRedis->Connect(m_ConfigMap["redis_ip"],atoi((m_ConfigMap["redis_port"]).c_str()));

   if(!myRedis->CheckConenct())
   {
       LogError("redis connect error ");
       return -1;
   }

	CMySQL::ptr mysql(new CMySQL());

    mysql->initDB(m_ConfigMap["mysql_host"],m_ConfigMap["mysql_user"],m_ConfigMap["mysql_passwd"],m_ConfigMap["mysql_dbname"]);
    
	if (!mysql->getConn())
	{
		LogError("mysql connect fail");
		return -1;
	}
	
     //文件标示，md5+文件名
    sprintf(fileid, "%s%s", md5.c_str(), fileName.c_str());
    string fileId = fileid;
	LogInfo("fileId = %s",fileId.c_str());

     //===1、先判断此文件是否已经分享，判断集合有没有这个文件，如果有，说明别人已经分享此文件，中断操作(redis操作)
    ret2 = myRedis->ZLexCount(FILE_PUBLIC_ZSET, fileId);
	
	LogInfo("myRedis->ZLexCount = %d",ret2);

    if( ret2 == 1 )//存在
    {
        LogInfo("shareFile case1:%s is already shared (redis record)",fileName.c_str());
        ret = -2;
        return ret;
    }
    else if(ret2 == 0) //不存在
    {//===2、如果集合没有此元素，可能因为redis中没有记录，再从mysql中查询，如果mysql也没有，说明真没有(mysql操作)
     //===3、如果mysql有记录，而redis没有记录，说明redis没有保存此文件，redis保存此文件信息后，再中断操作(redis操作)

        //查看此文件别人是否已经分享了
        sprintf(sql_cmd, "select * from share_file_list where md5 = '%s' and filename = '%s'", md5.c_str(),fileName.c_str());
        
        ret2 = mysql->exeSQL(sql_cmd,NULL);
        if(ret2 == 2) //说明有结果，别人已经分享此文件
        {
            //redis保存此文件信息
            myRedis->ZAdd(FILE_PUBLIC_ZSET, 0, fileId);

            LogInfo("shareFile case3:%s is already shared (mysql record)",fileName.c_str());
            ret = -2;
            return ret;
        }
    }
    else//出错
    {
        LogError("myRedis->ZLexCount error");
        ret = -1;
        return ret;
    }

     //===4、如果此文件没有被分享，mysql保存一份持久化操作(mysql操作)

    //sql语句, 更新共享标志字段
    sprintf(sql_cmd, "update user_file_list set shared_status = 1 where user = '%s' and md5 = '%s' and filename = '%s'" \
    , user.c_str() , md5.c_str(), fileName.c_str());

    if (mysql->query(sql_cmd) != 0)
    {
        LogError("mysqlStr[%s] exec fail , the error info is [%s]",sql_cmd,mysql->getErrStr().c_str());
        ret = -1;
        return ret;
    }
    else
    {
        LogInfo("shareFile case2:sql_cmd[%s] exec success ",sql_cmd);
        LogInfo("shareFile case2:mysql upate user_file_list record");
    }
    

    time_t now;;
    char create_time[TIME_STRING_LEN];
    //获取当前时间
    now = time(NULL);
    strftime(create_time, TIME_STRING_LEN-1, "%Y-%m-%d %H:%M:%S", localtime(&now));

    //分享文件的信息，额外保存在share_file_list保存列表
    /*
        -- user	文件所属用户
        -- md5 文件md5
        -- createtime 文件共享时间
        -- filename 文件名字
        -- pv 文件下载量，默认值为1，下载一次加1
    */
    sprintf(sql_cmd, "insert into share_file_list (user, md5, createtime, filename, pv) values ('%s', '%s', '%s', '%s', %d)" \
    , user.c_str(), md5.c_str(), create_time, fileName.c_str(), 0);
    
    if (mysql->query(sql_cmd) != 0)
    {
        LogError("mysqlStr[%s] exec fail , the error info is [%s]",sql_cmd,mysql->getErrStr().c_str());
        ret = -1;
        return ret;
    }
    else
    {
        LogInfo("shareFile case2:sql_cmd[%s] exec success ",sql_cmd);
        LogInfo("shareFile case2:mysql upate share_file_list record");
    }
    

    //查询共享文件数量
    sprintf(sql_cmd, "select count from user_file_count where user = '%s'", "xxx_share_xxx_file_xxx_list_xxx_count_xxx");
    
    ret2 = mysql->exeSQL(sql_cmd,tmp);
	LogInfo("current ret2=%d",ret2);
    if(ret2 == 1) //没有记录
    {
        //插入记录
        sprintf(sql_cmd, "insert into user_file_count (user, count) values('%s', %d)", "xxx_share_xxx_file_xxx_list_xxx_count_xxx", 1);
    }
    else if(ret2 == 0)
    {
        //更新用户文件数量count字段
        count = atoi(tmp);
        sprintf(sql_cmd, "update user_file_count set count = %d  where user = '%s'",count +1, "xxx_share_xxx_file_xxx_list_xxx_count_xxx");
    }
	
	LogInfo("current sql_cmd=%s",sql_cmd);

    if(mysql->query(sql_cmd) != 0)
    {
        LogError("mysqlStr[%s] exec fail , the error info is [%s]",sql_cmd,mysql->getErrStr().c_str());
        ret = -1;
        return ret;
    }
    else
    {
        LogInfo("shareFile case2:sql_cmd[%s] exec success ",sql_cmd);
        LogInfo("shareFile case2:mysql upate user_file_count record");
    }
    

    //===5、redis集合中增加一个元素(redis操作)
    myRedis->ZAdd(FILE_PUBLIC_ZSET, 0, fileId);

    //===6、redis对应的hash也需要变化 (redis操作)
    //     fileid ------>  filename
    myRedis->HSet(FILE_NAME_HASH, fileId, fileName);
	
	LogDebug("shareFile end");
    return ret;

}

int DealFileCgi::delFile(const string& user,const string& md5,const string& fileName)
{
     /*
    a)先判断此文件是否已经分享
    b)判断集合有没有这个文件，如果有，说明别人已经分享此文件(redis操作)
    c)如果集合没有此元素，可能因为redis中没有记录，再从mysql中查询，如果mysql也没有，说明真没有(mysql操作)
    d)如果mysql有记录，而redis没有记录，那么分享文件处理只需要处理mysql (mysql操作)
    e)如果redis有记录，mysql和redis都需要处理，删除相关记录
    */
    LogDebug("delFile begin");
    int ret = 0;
    char sql_cmd[SQL_MAX_LEN] = {0};
    char tmp[512] = {0};
    char fileid[1024] = {0};
    int ret2 = 0;
    int count = 0;
    int share = 0;  //共享状态
    int flag = 0; //标志redis是否有记录

    CMyRedis::ptr myRedis = CMyRedis::getInstance();
    myRedis->Connect(m_ConfigMap["redis_ip"],atoi((m_ConfigMap["redis_port"]).c_str()));

    if(!myRedis->CheckConenct())
    {
       LogError("redis connect error ");
       return -1;
    }

    CMySQL::ptr mysql(new CMySQL());

    mysql->initDB(m_ConfigMap["mysql_host"],m_ConfigMap["mysql_user"],m_ConfigMap["mysql_passwd"],m_ConfigMap["mysql_dbname"]);
    
	if (!mysql->getConn())
	{
		LogError("mysql connect fail");
		return -1;
	}
	
    //文件标示，md5+文件名
    sprintf(fileid, "%s%s", md5, fileName);
    string fileId = fileid;

     //===1、先判断此文件是否已经分享，判断集合有没有这个文件，如果有，说明别人已经分享此文件，中断操作(redis操作)
    ret2 = myRedis->ZLexCount(FILE_PUBLIC_ZSET, fileId);

    if( ret2 == 1 )//存在
    {
        LogInfo("delFile case1:%s is already shared",fileName.c_str());
        share = 1;  //共享标志
        flag = 1;   //redis有记录
    }
    else if(ret2 == 0) //不存在
    {//===2、如果集合没有此元素，可能因为redis中没有记录，再从mysql中查询，如果mysql也没有，说明真没有(mysql操作)
    
        //查看此文件别人是否已经分享了
        sprintf(sql_cmd, "select shared_status from user_file_list where user = '%s' and md5 = '%s' and filename = '%s'", user.c_str(), md5.c_str(), fileName.c_str());
       
         ret2 = mysql->exeSQL(sql_cmd, tmp); //执行sql语句
         if (ret2 == 0)
         {
           share = atoi(tmp); //shared_status字段
         }
         else if(ret2 == -1)//失败
         {
            LogError("mysqlStr[%s] exec fail , the error info is [%s]",sql_cmd,mysql->getErrStr().c_str());
            ret = -1;
            return ret;
         }
        
    }
    else//出错
    {
        ret = -1;
        return ret;
    }


    if( share == 1)
    {
        //===3、如果mysql有记录，删除相关分享记录 (mysql操作)
        //删除在共享列表的数据
        sprintf(sql_cmd, "delete from share_file_list where user = '%s' and md5 = '%s' and filename = '%s'",user.c_str(), md5.c_str(), fileName.c_str());

       if(mysql->query(sql_cmd) != 0)
        {
            LogError("mysqlStr[%s] exec fail , the error info is [%s]",sql_cmd,mysql->getErrStr().c_str());
            ret = -1;
            return ret;
        }
        else
        {
            LogInfo("delFile case3:sql_cmd[%s] exec success ",sql_cmd);
            LogInfo("delFile case3:mysql has record,delete it");
        }

        //共享文件的数量-1
        //查询共享文件数量
        sprintf(sql_cmd, "select count from user_file_count where user = '%s'", "xxx_share_xxx_file_xxx_list_xxx_count_xxx");

        ret2 = mysql->exeSQL(sql_cmd, tmp); //执行sql语句
         if ( ret2 != 0 )
        {
            LogError("mysqlStr[%s] exec fail , the error info is [%s]",sql_cmd,mysql->getErrStr().c_str());
            ret = -1;
            return ret;
        }
        else
        {
            LogInfo("delFile case3:sql_cmd[%s] exec success ",sql_cmd);
            LogInfo("delFile case3:mysql user_file_count search");
        }
        

        count = atoi(tmp);

        sprintf(sql_cmd, "update user_file_count set count = %d where user = '%s'",count -1, "xxx_share_xxx_file_xxx_list_xxx_count_xxx");
      
        if(mysql->query(sql_cmd) != 0)
        {
            LogError("mysqlStr[%s] exec fail , the error info is [%s]",sql_cmd,mysql->getErrStr().c_str());
            ret = -1;
            return ret;
        }
        else
        {
            LogInfo("delFile case3:sql_cmd[%s] exec success ",sql_cmd);
            LogInfo("delFile case3:mysql user_file_count -1");
        }
        

        //===4、如果redis有记录，redis需要处理，删除相关记录
        if(1 == flag)
        {
            //有序集合删除指定成员
            myRedis->ZRem( FILE_PUBLIC_ZSET, fileId);

            //从hash移除相应记录
            myRedis->HDel( FILE_NAME_HASH, fileId);

            LogInfo("delFile case3:redis clear");
        }
    }

     //用户文件数量-1
    //查询用户文件数量
    sprintf(sql_cmd, "select count from user_file_count where user = '%s'", user.c_str());
    
    ret2 = mysql->exeSQL(sql_cmd, tmp); //执行sql语句
    if ( ret2 != 0 )
    {
        LogError("mysqlStr[%s] exec fail , the error info is [%s]",sql_cmd,mysql->getErrStr().c_str());
        ret = -1;
        return ret;
    }
    else
    {
        LogInfo("mysql user_file_count user file -1");
    }
    
    count = atoi(tmp);

    if(count == 1)
    {
        //删除用户文件数量表对应的数据
        sprintf(sql_cmd, "delete from user_file_count where user = '%s'", user.c_str());
    }
    else
    {
        sprintf(sql_cmd, "update user_file_count set count = %d where user = '%s'", count-1, user.c_str());
    }

   if(mysql->query(sql_cmd) != 0)
    {
        LogError("mysqlStr[%s] exec fail , the error info is [%s]",sql_cmd,mysql->getErrStr().c_str());
        ret = -1;
        return ret;
    }

    //删除用户文件列表数据
    sprintf(sql_cmd, "delete from user_file_list where user = '%s' and md5 = '%s' and filename = '%s'", user.c_str(), md5.c_str(), fileName.c_str());

    if(mysql->query(sql_cmd) != 0)
    {
        LogError("mysqlStr[%s] exec fail , the error info is [%s]",sql_cmd,mysql->getErrStr().c_str());
        ret = -1;
        return ret;
    }
    else
    {
        LogInfo("delete user_file_list ");
    }
    

    //文件信息表(file_info)的文件引用计数count，减去1
    //查看该文件文件引用计数
    sprintf(sql_cmd, "select count from file_info where md5 = '%s'", md5.c_str());

    mysql->exeSQL(sql_cmd,tmp);
    if ( ret2 == 0 )
    {
        count = atoi(tmp); //count字段
    }
    else
    {
        LogError("mysqlStr[%s] exec fail , the error info is [%s]",sql_cmd,mysql->getErrStr().c_str());
        ret = -1;
        return ret;
    }

    count--; //减一
    sprintf(sql_cmd, "update file_info set count=%d where md5 = '%s'", count, md5.c_str());
    
    if(mysql->query(sql_cmd) != 0)
    {
        LogError("mysqlStr[%s] exec fail , the error info is [%s]",sql_cmd,mysql->getErrStr().c_str());
        ret = -1;
        return ret;
    }

   
    if(count == 0) //说明没有用户引用此文件，需要在storage删除此文件
    {
        //查询文件的id
        sprintf(sql_cmd, "select file_id from file_info where md5 = '%s'", md5.c_str());

        mysql->exeSQL(sql_cmd,tmp);
        if ( ret2 != 0 )
        {
            LogError("mysqlStr[%s] exec fail , the error info is [%s]",sql_cmd,mysql->getErrStr().c_str());
            ret = -1;
            return ret;
        }
  

        //删除文件信息表中该文件的信息
        sprintf(sql_cmd, "delete from file_info where md5 = '%s'", md5.c_str());

        if(mysql->query(sql_cmd) != 0)
        {
            LogError("mysqlStr[%s] exec fail , the error info is [%s]",sql_cmd,mysql->getErrStr().c_str());
            ret = -1;
            return ret;
        }
        else
        {
            LogInfo("delete file_info ");
        }
        

        //从storage服务器删除此文件，参数为为文件id
        ret2 = removeFileFromFastDfs(tmp);
        if(ret2 != 0)
        {
            LogError("removeFileFromFastDfs err");
            ret = -1;
            return ret;
        }
        else
        {
            LogInfo("removeFileFromFastDfs success ");
        }
        
    }
    LogDebug("delFile end");
    return ret;
}

int DealFileCgi::removeFileFromFastDfs(const string&  fileId)
{
	LogDebug("removeFileFromFastDfs begin");
    int ret = 0;

    //读取fdfs client 配置文件的路径
    string fdfs_cli_conf_path=m_ConfigMap["dfs_client_path"];
   
    string cmdStr="fdfs_delete_file " + fdfs_cli_conf_path + " " + fileId;
	LogInfo("cmdStr:%s",cmdStr.c_str());
    string retStr=ShellUtil::execShellPipe(cmdStr);
        
	LogInfo("fdfs_delete_file exec return %s",retStr.c_str());

	LogDebug("removeFileFromFastDfs end");
    return ret;
}

void DealFileCgi::returnDealFileStatus(string status_num)
{
	Json::Value root;
	
	//根节点属性
    root["code"] = Json::Value(status_num);

	string out = root.toStyledString();
	LogInfo("out = %s",out.c_str());
	printf("%s",out.c_str());

}

int DealFileCgi::verifyToken(const string& user, const string& token)
{
	LogDebug("verifyToken in");
	
	CMyRedis::ptr myRedis = CMyRedis::getInstance();

    myRedis->Connect(m_ConfigMap["redis_ip"],atoi((m_ConfigMap["redis_port"]).c_str()));
    
    if (!myRedis->CheckConenct())
    {
        LogError("redis connect fail");
    } 
	
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

}

int DealFileCgi::pvFile(const string& user,const string& md5,const string& fileName)
{
    LogDebug("pvFile begin");

    int ret = 0;
    int ret2 = 0;
    char sql_cmd[SQL_MAX_LEN] = {0};
    char tmp[512] = {0};
    int pv = 0;

    CMySQL::ptr mysql(new CMySQL());

    mysql->initDB(m_ConfigMap["mysql_host"],m_ConfigMap["mysql_user"],m_ConfigMap["mysql_passwd"],m_ConfigMap["mysql_dbname"]);
    
	if (!mysql->getConn())
	{
		LogError("mysql connect fail");
		return -1;
	}

    //sql语句
    //查看该文件的pv字段
    sprintf(sql_cmd, "select pv from user_file_list where user = '%s' and md5 = '%s' and filename = '%s'", user.c_str(), md5.c_str(), fileName.c_str());
    
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
    sprintf(sql_cmd, "update user_file_list set pv = %d where user = '%s' and md5 = '%s' and filename = '%s'", pv+1, user.c_str(), md5.c_str(), fileName.c_str());
    
    if(mysql->query(sql_cmd) != 0)
    {
        LogError("mysqlStr[%s] exec fail , the error info is [%s]",sql_cmd,mysql->getErrStr().c_str());
        ret = -1;
        return ret;
    }
    else
    {
        LogInfo("update user_file_list");
    }
    

    LogDebug("pvFile end");

    return ret;
}


int DealFileCgi::dealfileExec()
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
            //ret = fread(buf, 1, len, stdin); //从标准输入(web服务器)读取内容
			//printf("buf = %s\n",buf);
            if(ret == 0)
            {
                LogError("fread(buf, 1, len, stdin) err");
                continue;
            }
            LogInfo("buf = %s",buf);
			
			getJsonInfo(buf, mFileInfo.user, mFileInfo.token, mFileInfo.md5, mFileInfo.fileName);

           LogInfo("cmd = %s user = %s, token = %s, md5 = %s, filename = %s", \
           mFileInfo.cmd.c_str(), mFileInfo.user.c_str(), mFileInfo.token.c_str(), mFileInfo.md5.c_str(), mFileInfo.fileName.c_str());

            //验证登陆token，成功返回0，失败-1
            ret = verifyToken(mFileInfo.user, mFileInfo.token); //util_cgi.h
			
			int ret_type;
			string status_num;
            if ( ret ==0)
            {
                
                if( mFileInfo.cmd == "share") //分享文件
                {
                    ret_type = shareFile(mFileInfo.user, mFileInfo.md5, mFileInfo.fileName);

                    if(ret_type == 0)
                    {
                        status_num = "010";
                    }
                    else if(ret_type == -1)
                    {
                        status_num = "011"; 
                    }
                    else if(ret_type == -2)
                    {
                        status_num = "012";
                    }
                }
                else if( mFileInfo.cmd == "del")  //删除文件
                {
                    ret_type = delFile(mFileInfo.user, mFileInfo.md5, mFileInfo.fileName);
                    if(ret_type == 0)
                    {
                        status_num = "013";
                    }
                    else 
                    {
                        status_num = "014"; 
                    }
                }
                else if( mFileInfo.cmd == "pv")  //文件下载标志处理
                {
                     ret_type = pvFile(mFileInfo.user, mFileInfo.md5, mFileInfo.fileName);
                      if(ret_type == 0)
                      {
                          status_num = "016";
                      }
                      else 
                      {
                          status_num = "017"; 
                      }
                }
            }
            else
            {
                status_num = "111";
            }
            
            returnDealFileStatus(status_num);
         

        }

        }

     /* while */

    return 0;
}


