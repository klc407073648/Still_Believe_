/*
 * echo.c --
 *
 *	Produce a page containing all FastCGI inputs
 *
 *
 * Copyright (c) 1996 Open Market, Inc.
 *
 * See the file "LICENSE.TERMS" for information on usage and redistribution
 * of this file, and for a DISCLAIMER OF ALL WARRANTIES.
 *
 */
#ifndef lint
static const char rcsid[] = "$Id: echo.c,v 1.5 1999/07/28 00:29:37 roberts Exp $";
#endif /* not lint */

#include "fcgi_config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif

#ifdef _WIN32
#include <process.h>
#else
extern char **environ;
#endif

#include "fcgi_stdio.h"


static void PrintEnv(char *label, char **envp)
{
    printf("%s:<br>\n<pre>\n", label);
    for ( ; *envp != NULL; envp++) {
        printf("%s\n", *envp);
    }
    printf("</pre><p>\n");
}

/* std::string execShellPipe(std::string cmd)
{
	char res[1024]={0},*p;
	 
	FILE *fp = popen(cmd.c_str(),"r");
	
	if( fp != NULL)
	{
		fgets( res, sizeof(res), fp ); //遇到\n终止复制
		if((p = strchr(res,'\n')) != NULL)
			*p = '\0';
		//fread( res, sizeof(char), sizeof(res), fp );
		pclose(fp);
	}
	
	return res;
} */

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

int main ()
{
    char **initialEnv = environ;
    int count = 0;

    while (FCGI_Accept() >= 0) {
        char *contentLength = getenv("CONTENT_LENGTH");
        int len;

	printf("Content-type: text/html\r\n"
	    "\r\n"
	    "<title>FastCGI echo</title>"
	    "<h1>FastCGI echo</h1>\n"
            "Request number %d,  Process ID: %d<p>\n", ++count, getpid());

        if (contentLength != NULL) {
            len = strtol(contentLength, NULL, 10);
        }
        else {
            len = 0;
        }

        if (len <= 0) {
	    printf("No data from standard input.<p>\n");
        }
       else
        {
            //===============> 得到上传文件  <============
          char *file_buf = NULL; //==========> 开辟存放文件的 内存 <===========
		  file_buf = (char *)malloc(len);
		  
		  if (file_buf == NULL)
		  {
			  printf("malloc error! file size is to big!!!!\n");
			  return -1;
		  }
		  
		  int ret2 = fread(file_buf, 1, len, stdin); //从标准输入(web服务器)读取内容
			if(ret2 == 0)
			{
				 printf("fread error\n");
				return -1;
			}
		  
		  
		  char *begin = NULL;
		  char *p, *q, *k;
		  char boundary[4096] = {0};
		  char content_text[4096] = {0};
		  
		  begin = file_buf;    //内存起点
		  p = begin;
		  
		  //printf("file_buf = %s\n",file_buf);
		  
		//get boundary 得到分界线, ------WebKitFormBoundary88asdgewtgewx
		p = strstr(begin, "\r\n");
		if (p == NULL)
		{
            printf("boundary error\n");
			return -1;
		}
		
		//拷贝分界线
		strncpy(boundary, begin, p-begin);
		boundary[p-begin] = '\0';   //字符串结束符
		
		 printf("boundary =[%s]\n",boundary);
		 
		  p += 2;//\r\n
		//已经处理了p-begin的长度
		len -= (p-begin);
			  //get content text head
		begin = p;

		//Content-Disposition: form-data; user="mike"; filename="xxx.jpg"; md5="xxxx"; size=10240\r\n
		p = strstr(begin, "\r\n");
		if(p == NULL)
		{
			printf("Content-Disposition error\n");
			return -1;
		}
		strncpy(content_text, begin, p-begin);
		content_text[p-begin] = '\0';
		
		printf("content_text =[%s]\n",content_text);
		
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
			printf("find boundary error\n");
			return -1;
		}
		else
		{
			p = p - 2;//\r\n
		}
		
		
		 int fd = 0;
		fd = open("test.png", O_CREAT|O_WRONLY, 0644);
		if (fd < 0)
		{
			printf("fd open error\n");
			return -1;
		}

		//ftruncate会将参数fd指定的文件大小改为参数length指定的大小
		ftruncate(fd, (p-begin));
		write(fd, begin, (p-begin));
		close(fd);
		
		
		}
		
		//string myStr=execShellPipe("fdfs_upload_file /etc/fdfs/client.conf ./klc.txt")
		
		//printf("myStr=%s\n",myStr.c_str());
		
		//string myStr1=execShellPipe("mysql -h localhost -u root -P 3306 -pklczxas789 -Dstudent -e 'insert into store_info'")
		
		
		
    } /* while */

    return 0;
}
