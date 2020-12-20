#include "UploadCgi.h"

Config* UploadCgi::m_Config;
MySQL::ptr UploadCgi::mysql=NULL;

CREATE_STATIC_LOGGERMANAGER(UploadCgi);

UploadCgi::UploadCgi()
{
	m_Config = new Config(json_file);
    m_ConfigMap=m_Config->getConfigMap();
	LogInfo("UploadCgi Create!");
}

UploadCgi::~UploadCgi()
{
	delete m_Config;
	LogInfo("UploadCgi Finish!");
}

template <class Type> 
Type stringToNum(const string& str){ 
    istringstream iss(str); 
    Type num; 
    iss >> num; 
    return num;     
} 
 
string UploadCgi::parseContent(string src, string target)
 {
     string result;
    int begin = src.find(target);
    if ( begin != string::npos )
    {
        begin = begin+target.size()+1;
        int end = src.find_first_of('"',begin);
        result = src.substr(begin,end-begin);
		
        result.erase(0,result.find_first_not_of(" "));  
        result.erase(result.find_last_not_of(" ") + 1); 

        return result;
    }

    return "";
 }


/**
 * @brief  在字符串full_data中查找字符串substr第一次出现的位置
 *
 * @param full_data 	源字符串首地址
 * @param full_data_len 源字符串长度
 * @param substr        匹配字符串首地址
 *
 * @returns   
 *      成功: 匹配字符串首地址
 *      失败：NULL
 */
char* memstr(char* full_data, int full_data_len, char* substr) 
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


int UploadCgi::recSaveFile2(long len)
{
        LogDebug("recSaveFile2 begin");

          char *file_buf = NULL; //==========> 开辟存放文件的 内存 <===========
		  file_buf = (char *)malloc(len);
		  
		  if (file_buf == NULL)
		  {
			  LogInfo("malloc error! file size is to big!!!!\n");
			  return -1;
		  }
		  
		  int ret2 = fread(file_buf, 1, len, stdin); //从标准输入(web服务器)读取内容
			if(ret2 == 0)
			{
				LogInfo("fread error\n");
				return -1;
			}
		  
		  
		  char *begin = NULL;
		  char *p, *q, *k;
		  char boundary[4096] = {0};
		  char content_text[4096] = {0};
		  
		  begin = file_buf;    //内存起点
		  p = begin;
		  
		  //LogInfo("file_buf = %s\n",file_buf);
		  
		//get boundary 得到分界线, ------WebKitFormBoundary88asdgewtgewx
		p = strstr(begin, "\r\n");
		if (p == NULL)
		{
            LogInfo("boundary error\n");
			return -1;
		}
		
		//拷贝分界线
		strncpy(boundary, begin, p-begin);
		boundary[p-begin] = '\0';   //字符串结束符
		
		// LogInfo("boundary =[%s]\n",boundary);
		 
		  p += 2;//\r\n
		//已经处理了p-begin的长度
		len -= (p-begin);
			  //get content text head
		begin = p;

		//Content-Disposition: form-data; user="mike"; filename="xxx.jpg"; md5="xxxx"; size=10240\r\n
		p = strstr(begin, "\r\n");
		if(p == NULL)
		{
			LogInfo("Content-Disposition error\n");
			return -1;
		}
		strncpy(content_text, begin, p-begin);
		content_text[p-begin] = '\0';
		
		LogInfo("content_text =[%s]",content_text);
		
		
		//从content_text中解析出filename
		string mystr=content_text;
		string user = parseContent(mystr,"user=");
		string fileName = parseContent(mystr,"filename=");
		string md5 = parseContent(mystr,"md5=");
		
		string tmp ="size=";
		int sizeBegin = mystr.find_last_of("=");
		string m_size = mystr.substr(sizeBegin+1);
		long p_size = stringToNum<long>(m_size); 
        		
		p += 2;//\r\n
        len -= (p-begin);
		
		begin = p;
		p = strstr(begin, "\r\n");
		p += 4;//\r\n\r\n
		len -= (p-begin);
		
		begin = p;
		
		 p = memstr(begin, len, boundary);//util_cgi.h， 找文件结尾
		if (p == NULL)
		{
			LogInfo("find boundary error");
			return -1;
		}
		else
		{
			p = p - 2;//\r\n
		}
		
		
		 int fd = 0;
		fd = open(fileName.c_str(), O_CREAT|O_WRONLY, 0644);
		if (fd < 0)
		{
			LogInfo("fd open error");
			return -1;
		}

		//ftruncate会将参数fd指定的文件大小改为参数length指定的大小
		ftruncate(fd, (p-begin));
		write(fd, begin, (p-begin));
		close(fd);
		
		mFileInfo.user = user;
		mFileInfo.fileName = fileName;
		mFileInfo.md5 = md5;
		mFileInfo.size = p_size;
		
		LogInfo("user: [%s] , fileName: [%s] , md5: [%s] , size: [%d]", \
        mFileInfo.user.c_str(), mFileInfo.fileName.c_str(), mFileInfo.md5.c_str(), mFileInfo.size);
		
		LogDebug("recSaveFile2 end");
		
		return 0;
}


int UploadCgi::recSaveFile(long len, string& user, string& fileName, string& md5, long& p_size)
{
	LogDebug("recSaveFile begin");
    int ret = 0;
    char *file_buf = NULL;

    string content_text; //文件头部信息
    string boundary;     //分界线信息

    //==========> 开辟存放文件的 内存 <===========
    file_buf = (char *)malloc(len);
    if (file_buf == NULL)
    {
        LogError("malloc error! file size is to big!");
        return -1;
    }

    int ret2 = fread(file_buf, 1, len, stdin); //从标准输入(web服务器)读取内容

    if( ret2 == 0  )
    {
        LogError("fread(file_buf, 1, len, stdin) err");
        return -1;
    }

    //===========> 开始处理前端发送过来的post数据格式 <============
  
    /*
       ------WebKitFormBoundary88asdgewtgewx\r\n
       Content-Disposition: form-data; user="mike"; filename="xxx.jpg"; md5="xxxx"; size=10240\r\n
       Content-Type: application/octet-stream\r\n
       \r\n
       真正的文件内容\r\n
       ------WebKitFormBoundary88asdgewtgewx
    */
    string begin = file_buf;
	LogInfo("file_buf: [%s]", begin.c_str());
    //get boundary 得到分界线, ------WebKitFormBoundary88asdgewtgewx
    int pos = begin.find_first_of("\r\n");
    if ( pos == -1 )
    {
        LogError("wrong no boundary!");
        return -1;
    }

    //拷贝分界线
    boundary = begin.substr(0,pos);
    LogInfo("boundary: [%s]", boundary.c_str());

    begin = begin.substr(pos+2);// 跳过\r\n
   
    len = len - (pos+2);  //已经处理了pos+2的长度

    //获取内容描述: Content-Disposition: form-data; user="mike"; filename="xxx.jpg"; md5="xxxx"; size=10240\r\n
    int pos1 = begin.find_first_of("\r\n");

    if ( pos1 == -1 )
    {
        LogError("get context text error, no filename?");
        return -1;
    }

    content_text = begin.substr(0,pos1);
    LogInfo("content_text: [%s]", content_text.c_str());
  
    begin = begin.substr(pos1+2);// 跳过\r\n
   
    len = len - (pos1+2); //已经处理了pos1+2的长度
    
    //========================================获取文件上传者,解析 user,filename ,md5 ,size
    //Content-Disposition: form-data; user="mike"; filename="xxx.jpg"; md5="xxxx"; size=10240\r\n
    //                                ↑
    
    user = parseContent(content_text,"user=");
    fileName = parseContent(content_text,"filename=");
    md5 = parseContent(content_text,"md5=");
    
    string tmp ="size=";
    int sizeBegin = content_text.find_last_of("=");
    string m_size = content_text.substr(sizeBegin+1);
    p_size = stringToNum<long>(m_size);
    
    //跳过 \r\n\r\n
    int pos2 = begin.find_first_of("\r\n");

    if ( pos2 == -1 )
    {
        LogError("no content");
        return -1;
    }

    begin = begin.substr(pos2+4);
    len = len - (pos2+4);

    //下面才是文件的真正内容

    /*
       ------WebKitFormBoundary88asdgewtgewx\r\n
       Content-Disposition: form-data; user="mike"; filename="xxx.jpg"; md5="xxxx"; size=10240\r\n
       Content-Type: application/octet-stream\r\n
       \r\n
       真正的文件内容\r\n
       ------WebKitFormBoundary88asdgewtgewx
       */
	LogInfo("content: [%s]", begin.c_str());   
    int pos3 = begin.find_first_of(boundary);

    if ( pos3 == -1 )
    {
        LogError("find boundary end error");
		return -1;
    }
    else
    {
        begin = begin.substr(0,pos3-2);
    }

    int file_len = pos3-2;

    char* myfile= begin.c_str();

    //=====> 此时begin-->p两个指针的区间就是post的文件二进制数据
    //======>将数据写入文件中,其中文件名也是从post数据解析得来  <===========

    int fd = 0;
    fd = open(fileName.c_str(), O_CREAT|O_WRONLY, 0644);
    if (fd < 0)
    {
        LogError("open %s error", fileName.c_str());
        return -1;
    }

    ftruncate(fd, begin.length());
    write(fd, myfile, begin.length());
    close(fd);
	
	LogInfo("user: [%s]", mFileInfo.user.c_str());
	LogInfo("fileName: [%s]", mFileInfo.fileName.c_str());
	LogInfo("md5: [%s]", mFileInfo.md5.c_str());
	LogInfo("size: [%d]", mFileInfo.size);

	LogDebug("recSaveFile end");
    return ret;
}

int UploadCgi::uploadToFastDfs(const string& fileName, string& fileId)
{
	LogDebug("uploadToFastDfs begin");
    int ret = 0;

    //读取fdfs client 配置文件的路径
    string fdfs_cli_conf_path=m_ConfigMap["dfs_client_path"];
   
    string cmdStr="fdfs_upload_file " + fdfs_cli_conf_path + " " + fileName;
	LogInfo("cmdStr:%s",cmdStr.c_str());
    string retStr=ShellUtil::execShellPipe(cmdStr);
        
	LogInfo("fdfs_upload_file exec return [%s]",retStr.c_str());
	
	StringUtil::ClearHeadTailSpace(retStr);
	fileId = retStr;

	LogDebug("uploadToFastDfs end");
    return ret;
}

string&  replace_all(string&  str, const  string&  old_value, const  string&  new_value)
{
    while(true)
   {
        string::size_type   pos(0);
        if(   (pos=str.find(old_value)) != string::npos   )
         {
      str.replace(pos,old_value.length(),new_value);
 }
        else  { break; }
    }
    return   str;
}

int UploadCgi::makeFileUrl(const string& fileId, string& fdfsFileUrl)
{
	LogDebug("makeFileUrl begin");
	
    int ret = 0;
	string fdfs_file_host_name;
	string storage_web_server_port;

     //读取fdfs client 配置文件的路径
    string fdfs_cli_conf_path=m_ConfigMap["dfs_client_path"];
	
	//string m_temp = "group1/M00/";
	//fileId.replace(0,m_temp.size(),"/usr/fastdfs/fastdfs0/data/00/00/");
	//string m_temp = fileId;
	//m_temp = replace_all(m_temp,"group1/M00/","/usr/fastdfs/fastdfs0/data/");
	
    /*fileId : group1/M00/00/01/wKgC5l9LQk6AU1LcAABfQI4wr0E14547.o*/
	LogInfo("fileId:%s",fileId.c_str());
   
    string cmdStr="fdfs_file_info " + fdfs_cli_conf_path + " " + fileId;
	LogInfo("cmdStr:%s",cmdStr.c_str());
	
    string retStr=ShellUtil::execShellPipe(cmdStr);
        
	LogInfo("fdfs_file_info exec return %s",retStr.c_str());
	

    //拼接上传文件的完整url地址--->http://host_name/group1/M00/00/00/D12313123232312.png

	//从retStr获取fdfs_file_host_name 暂不解决，且不加端口23000可以下载
   /*  string temp = "address:";
    int pos1 = retStr.find_first_of(temp);
    int pos2 = retStr.find_first_of("\n",pos1);
	LogInfo("fdfs_file_info pos1 %d",pos1);
	LogInfo("fdfs_file_info pos2 %d",pos2);
	fdfs_file_host_name= retStr.substr(pos1,pos2-pos1);
	
	LogInfo("fdfs_file_host_name %s",fdfs_file_host_name); */

    //读取storage_web_server服务器的端口

    storage_web_server_port=m_ConfigMap["storage_web_server_port"];

    fdfsFileUrl = "http://";
    //fdfsFileUrl += fdfs_file_host_name;
	fdfsFileUrl += "192.168.2.230";
    //fdfsFileUrl += ":";
    //fdfsFileUrl += storage_web_server_port;
    fdfsFileUrl += "/";
    fdfsFileUrl += fileId;

    /*file url is: http://192.168.2.230/group1/M00/00/01/wKgC5l9LQk6AU1LcAABfQI4wr0E14547.o*/
    LogInfo("file url is: %s", fdfsFileUrl.c_str());
    
    LogDebug("makeFileUrl end");
    return ret;
}

MySQL::ptr UploadCgi::getMySQL()
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

int UploadCgi::storeFileInfoToMysql(const string& user,const string& fileName,const string& md5, const long& size,const string& fileId, const string& fdfsFileUrl)
{
	LogDebug("storeFileInfoToMysql begin");
    int ret = 0;

    time_t now;;
    char create_time[TIME_STRING_LEN];
    char sql_cmd[SQL_MAX_LEN] = {0};
    string sql_str;
    

	getMySQL();//获取mysql实例
    
	if (!mysql->connect())
	{
		LogError("mysql connect fail!");
		return -1;
	}

    //得到文件后缀字符串 如果非法文件后缀,返回"null"
    string suffix=StringUtil::getFileSuffix(fileName); //mp4, jpg, png

    //sql 语句
    /*
       -- =============================================== 文件信息表
       -- md5 文件md5
       -- file_id 文件id
       -- url 文件url
       -- size 文件大小, 以字节为单位
       -- type 文件类型： png, zip, mp4……
       -- count 文件引用计数， 默认为1， 每增加一个用户拥有此文件，此计数器+1
       */
    sprintf(sql_cmd, "insert into file_info (md5, file_id, url, size, type, count) values ('%s', '%s', '%s', '%ld', '%s', %d)", \
            md5.c_str(), fileId.c_str(), fdfsFileUrl.c_str(), size, suffix.c_str(), 1);

    sql_str = sql_cmd;
    mysql->query(sql_str);

    int num = mysql->getAffectedRows();

	//插入成功 num > 0
	if ( num <= 0 || mysql->getErrno()!=0 )
	{
        LogError("sql_str: %s,affectedRows is %d, error num is %d : info is %s",\
		sql_str.c_str(),num,mysql->getErrno(),mysql->getErrStr().c_str());
        return -1;
	}
	
	LogInfo("sql_str:[%s] exec success!",sql_str.c_str());

    //获取当前时间
    now = time(NULL);
    strftime(create_time, TIME_STRING_LEN-1, "%Y-%m-%d %H:%M:%S", localtime(&now));

    /*
       -- =============================================== 用户文件列表
       -- user 文件所属用户
       -- md5 文件md5
       -- createtime 文件创建时间
       -- filename 文件名字
       -- shared_status 共享状态, 0为没有共享， 1为共享
       -- pv 文件下载量，默认值为0，下载一次加1
       */
    sprintf(sql_cmd, "insert into user_file_list(user, md5, createtime, filename, shared_status, pv) values ('%s', '%s', '%s', '%s', %d, %d)", \
     user.c_str(), md5.c_str(), create_time, fileName.c_str(), 0, 0);
    
    sql_str = sql_cmd;
    mysql->query(sql_str);
    num = mysql->getAffectedRows();

	//插入成功 num > 0
	if (num <= 0 || mysql->getErrno()!=0 )
	{
       LogError("sql_str: %s,affectedRows is %d, error num is %d : info is %s",\
		sql_str.c_str(),num,mysql->getErrno(),mysql->getErrStr().c_str());
        return -1;
	}
	
	LogInfo("sql_str:[%s] exec success!",sql_str.c_str());

    //查询用户文件数量
    sprintf(sql_cmd, "select count from user_file_count where user = '%s'", user.c_str());

    sql_str = sql_cmd;
    mysql->query(sql_str);
    num = mysql->getAffectedRows();
	
	if (num < 0 || mysql->getErrno()!=0 )
	{
       LogError("sql_str: %s,affectedRows is %d, error num is %d : info is %s",\
		sql_str.c_str(),num,mysql->getErrno(),mysql->getErrStr().c_str());
        return -1;
	}
	
	LogInfo("sql_str:[%s] exec success!",sql_str.c_str());
	
    //查询用户文件数量
    if( num == 0 ) //没有记录
    {
        //插入记录
        sprintf(sql_cmd, " insert into user_file_count (user, count) values('%s', %d)", user.c_str(), 1);
    }
    else if ( num > 0 )
    {
        //更新用户文件数量count字段,自增1
        sprintf(sql_cmd, "update user_file_count set count = count +1 where user = '%s'",user.c_str());
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
	
	LogDebug("storeFileInfoToMysql end");
    return ret;
}

void UploadCgi::returnUploadStatus(const string& statusNum)
{
	Json::Value root;
	
	//根节点属性
	root["code"] = Json::Value(statusNum);

	string out = root.toStyledString();
	LogInfo("out = %s",out.c_str());
	printf("%s",out.c_str());

}

int UploadCgi::uploadExec()
{
    while (FCGI_Accept() >= 0)
    {
        char *contentLength = getenv("CONTENT_LENGTH");
        long len;
        int ret = 0;

        printf("Content-type: text/html\r\n\r\n");

        if (contentLength != NULL)
        {
            len = strtol(contentLength, NULL, 10); //字符串转long， 或者atol
        }
        else
        {
            len = 0;
        }

        if (len <= 0)
        {
            printf("No data from standard input\n");
            LogError("len = 0, No data from standard input");
        }
        else
        {
            //===============> 得到上传文件  <============
            if (recSaveFile2(len) < 0) //解决了图像上传娶不到bou
            {
                LogError("recSaveFile fail");
                ret = -1;
                goto END;
            } 
			
			 
			/*  if (recSaveFile(len, mFileInfo.user, mFileInfo.fileName, mFileInfo.md5, mFileInfo.size) < 0)
            {
                LogError("recSaveFile fail");
                return -1;
            }  */

            //===============> 将该文件存入fastDFS中,并得到文件的file_id <============
            if (uploadToFastDfs(mFileInfo.fileName, mFileInfo.fileId) < 0)
            {
                LogError("uploadToFastDfs fail");
                ret = -1;
                goto END;
            }
			
			LogInfo("%s upload success [%s, size:%ld , md5:%s] to local", mFileInfo.user.c_str(),  mFileInfo.fileName.c_str(), mFileInfo.size, mFileInfo.md5.c_str());
			
            //================> 删除本地临时存放的上传文件 <===============
            unlink((mFileInfo.fileName).c_str());

            //================> 得到文件所存放storage的host_name <=================
            if (makeFileUrl(mFileInfo.fileId, mFileInfo.fdfsFileUrl) < 0)
            {
                LogError("makeFileUrl fail");
                ret = -1;
                goto END;
            }

            //===============> 将该文件的FastDFS相关信息存入mysql中 <======
            if (storeFileInfoToMysql(mFileInfo.user, mFileInfo.fileName, mFileInfo.md5, mFileInfo.size, mFileInfo.fileId, mFileInfo.fdfsFileUrl) < 0)
            {
                LogError("storeFileInfoToMysql fail");
                ret = -1;
                goto END;
            }
            //给前端返回，上传情况
            /*
               上传文件：
               成功：{"code":"008"}
               失败：{"code":"009"}
               */
END:
            if(ret == 0) //成功上传
            {
                returnUploadStatus("008");//common.h
            }
            else//上传失败
            {
               returnUploadStatus("009");//common.h
            }

            

        }

    } /* while */

    return 0;
}


