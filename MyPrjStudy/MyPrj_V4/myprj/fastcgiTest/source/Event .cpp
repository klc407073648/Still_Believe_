#include "Event.h"
#include "memory.h"
#include "Util.h"
#include <unistd.h>
#include <fcntl.h>

//#define THREAD_COUNT 20
//static int counts[THREAD_COUNT];


Event::Event(int threadNum):
    m_iThreadNum(threadNum)
{

}

Event::~Event()
{

}

void Event::addEvent(string requestEvent, handlerFunc func)
{
    m_EventMap.insert(EventMap::value_type(requestEvent, func));
}

char* Event::memstr(char* full_data, int full_data_len, char* substr) 
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

void Event::exec()
{
        while(FCGI_Accept() >= 0)
        {
             //"GET" or "POST"
            string requestMethod =  string(getenv("REQUEST_METHOD"));
            //"/login?key=value&name=pwd"
            string requestUri = string(getenv("REQUEST_URI"));
            size_t pos = requestUri.find_first_of("?");
            string requestEvent = requestUri.substr(0,pos);
			
            string requestParam;
            char*  requestCharParam;
            if("GET" == requestMethod)
            {
                if(-1 != pos)
                {
                    requestParam = requestUri.substr(pos+1);
                }
            }
            else if("POST" == requestMethod)
            {
                int len = atoi(getenv("CONTENT_LENGTH")) + 1;
                char *bufpost = (char* )malloc(len);
                memset(bufpost, 0, len);
                FCGI_fread(bufpost, len, 1, FCGI_stdin);
                if(requestUri != "/upload")
                {
                    requestParam = string(bufpost);
                }
                else
                {
                    requestCharParam=bufpost;
                }
                
                //free(bufpost);
				char *contentLength = getenv("CONTENT_LENGTH");
				//printf("requestParam = %s\n",requestParam.c_str());
				
				  printf("Content-type: text/html\r\n"
				"\r\n"
				"<title>FastCGI echo</title>"
				"<h1>FastCGI echo</h1>\n"); 
                
				printf("register begin<p>\n");
				printf("requestParam = %s\n",requestParam.c_str());		
				if (requestUri == "/upload")
				{
				    printf("register hello upload<p>\n");
				    char *  bufpost =requestCharParam;
                    char *begin = NULL;
                    char *p, *q, *k;
                    char boundary[4096] = {0};
                    char content_text[4096] = {0};

                    begin = requestCharParam; //内存起点
                    p = begin;

                    printf("file_buf = %s\n", requestCharParam);

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
					
					return ;
					//printf("retStr=%s\n",retStr.c_str());
				}
            }
		
            EventMap::iterator iterMap;
            iterMap = m_EventMap.find(requestEvent);
            if(iterMap == m_EventMap.end())
            {
                //DBG(L_ERROR, "not find Event: %s ", requestUri.c_str());
            }
            else
            {
                handlerFunc func = iterMap->second;
                Request req;
                req.setMethod(requestMethod);
                req.setUrl(requestEvent);
                req.setParams(requestParam);
                req.setToken("klc123");
                req.setCharParams("klc123");
                 
                Response res = func(req);
                FCGI_printf(res.Out().c_str()); 
            }
        }
}

void Event::processMessage(int threadID)
{
    
}

/* void Event::exec()
{
    if(m_iThreadNum > 1)
    {
        //多线程模式
        pthread_t id[m_iThreadNum];
        FCGX_Init();
        for (int i = 1; i < m_iThreadNum; i++)
        {
            threadStruct p;
            p.pthis = this;
            p.threadID = i;
            pthread_create(&id[i], NULL, pthreadTask, (void*)&p);
        }
        processMessage(0);
    }
    else
    {
        //单线程模式
        while(FCGI_Accept() >= 0)
        {
            //"GET" or "POST"
            string requestMethod =  string(getenv("REQUEST_METHOD"));
            //"/login?key=value&name=pwd"
            string requestUri = string(getenv("REQUEST_URI"));
            size_t pos = requestUri.find_first_of("?");
            string requestEvent = requestUri.left(pos);
            string requestParam;
            if("GET" == requestMethod)
            {
                if(-1 != pos)
                {
                    requestParam = requestUri.right(requestUri.length() - pos - 1);
                }
            }
            else if("POST" == requestMethod)
            {
                int ilen = atoi(getenv("CONTENT_LENGTH")) + 1;
                char *bufpost = (char* )malloc(ilen);
                memset(bufpost, 0, ilen);
                FCGI_fread(bufpost, ilen, 1, FCGI_stdin);
                requestParam = string(bufpost);
                free(bufpost);
            }

            EventMap::iterator iterMap;;
            iterMap = m_EventMap.find(requestEvent);
            if(iterMap == m_EventMap.end())
            {
                DBG(L_ERROR, "not find Event: %s ", requestUri.c_str());
            }
            else
            {
                handlerFunc func = iterMap->second;
                Request req;
                req.setMethod(requestMethod);
                req.setUrl(requestEvent);
                req.setParams(requestParam);
                req.setCookie(getenv("HTTP_COOKIE"));

                Response res = func(req);

                FCGI_printf(res.Out().c_str());
            }
        }
    }
}

void Event::processMessage(int threadID)
{
//    pid_t pid = getpid();
    int rc;
    FCGX_Request request;
    FCGX_InitRequest(&request, 0, 0);
    for(;;)
    {
        static pthread_mutex_t accept_mutex = PTHREAD_MUTEX_INITIALIZER;
//        static pthread_mutex_t counts_mutex = PTHREAD_MUTEX_INITIALIZER;

        pthread_mutex_lock(&accept_mutex);
        rc = FCGX_Accept_r(&request);
        pthread_mutex_unlock(&accept_mutex);
        if (rc < 0)
            break;

        std::string httpCookie;
        if(FCGX_GetParam("HTTP_COOKIE", request.envp))
            httpCookie = std::string(FCGX_GetParam("HTTP_COOKIE", request.envp));

        //"GET" or "POST"
        string requestMethod;
        if(FCGX_GetParam("REQUEST_METHOD", request.envp))
            requestMethod = string(FCGX_GetParam("REQUEST_METHOD", request.envp));

        //"/login?key=value&name=pwd"
        string requestUri;
        if(FCGX_GetParam("REQUEST_URI", request.envp))
            requestUri = string(FCGX_GetParam("REQUEST_URI", request.envp));

        size_t pos = requestUri.find_first_of("?");
        string requestEvent = requestUri.left(pos);
        string requestParam;
        if("GET" == requestMethod)
        {
            if(-1 != pos)
            {
                requestParam = requestUri.right(requestUri.length() - pos - 1);
            }
        }
        else if("POST" == requestMethod)
        {
            if(FCGX_GetParam("CONTENT_LENGTH", request.envp))
            {
                int ilen = atoi(FCGX_GetParam("CONTENT_LENGTH", request.envp)) + 1;
                char *bufpost = (char* )malloc(ilen);
                memset(bufpost, 0, ilen);
                FCGX_GetStr(bufpost,ilen,request.in);
                requestParam = string(bufpost);
                free(bufpost);
            }
        }

        EventMap::iterator iterMap;;
        iterMap = m_EventMap.find(requestEvent);
        if(iterMap == m_EventMap.end())
        {
            DBG(L_ERROR, "not find Event: %s ", requestUri.c_str());
        }
        else
        {
            handlerFunc func = iterMap->second;
            Request req;
            req.setMethod(requestMethod);
            req.setUrl(requestEvent);
            req.setParams(requestParam);
            req.setCookie(httpCookie);
            Response res = func(req);
            FCGX_PutS(res.Out().c_str(), request.out);

//            DBG(L_DEBUG, "res.Out: %s", res.Out().c_str());
        }
//        sleep(2);
//        pthread_mutex_lock(&counts_mutex);
//        DBG(L_DEBUG, "counts: %5d , PID: %d, threadID: %d" , ++counts[threadID], pid, threadID);
//        pthread_mutex_unlock(&counts_mutex);
        FCGX_Finish_r(&request);
    }
}
 */