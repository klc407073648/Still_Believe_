#include "util.h"
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <string.h>
#include <iostream>

const int MAX_BUFF = 4096;
ssize_t readn(int fd, void *buff, size_t n)
{
    size_t nleft = n;
    ssize_t nread = 0;
    ssize_t readSum = 0;
    char *ptr = (char*)buff;
    while (nleft > 0)
    {
        if ((nread = read(fd, ptr, nleft)) < 0)
        {
            if (errno == EINTR)
                nread = 0;
            else if (errno == EAGAIN)
            {
                return readSum;
            }
            else
            {
                return -1;
            }  
        }
        else if (nread == 0)
            break;
        readSum += nread;
        nleft -= nread;
        ptr += nread;
    }
    return readSum;
}

ssize_t readn(int fd, std::string &inBuffer)
{
    ssize_t nread = 0;
    ssize_t readSum = 0;
    while (true)
    {
        char buff[MAX_BUFF];
        if ((nread = read(fd, buff, MAX_BUFF)) < 0)
        {
            if (errno == EINTR)
                continue;
            else if (errno == EAGAIN)
            {
                
                return readSum;
            }  
            else
            {
                perror("read error");
                return -1;
            }
        }
        else if (nread == 0)
            break;
        //printf("before inBuffer.size() = %d\n", inBuffer.size());
        //printf("nread = %d\n", nread);
        readSum += nread;
        //buff += nread;
        inBuffer += std::string(buff, buff + nread);
        //printf("after inBuffer.size() = %d\n", inBuffer.size());
    }
    return readSum;
}

ssize_t writen(int fd, void *buff, size_t n)
{
    size_t nleft = n;
    ssize_t nwritten = 0;
    ssize_t writeSum = 0;
    char *ptr = (char*)buff;
    while (nleft > 0)
    {
        if ((nwritten = write(fd, ptr, nleft)) <= 0)
        {
            if (nwritten < 0)
            {
                if (errno == EINTR)
                {
                    nwritten = 0;
                    continue;
                }
                else if (errno == EAGAIN)
                {
                    return writeSum;
                }
                else
                    return -1;
            }
        }
        writeSum += nwritten;
        nleft -= nwritten;
        ptr += nwritten;
    }
    return writeSum;
}

ssize_t writen(int fd, std::string &sbuff)
{
    size_t nleft = sbuff.size();
    ssize_t nwritten = 0;
    ssize_t writeSum = 0;
    const char *ptr = sbuff.c_str();
    while (nleft > 0)
    {
        if ((nwritten = write(fd, ptr, nleft)) <= 0)
        {
            if (nwritten < 0)
            {
                if (errno == EINTR)
                {
                    nwritten = 0;
                    continue;
                }
                else if (errno == EAGAIN)
                    break;
                else
                    return -1;
            }
        }
        writeSum += nwritten;
        nleft -= nwritten;
        ptr += nwritten;
    }
    if (writeSum == sbuff.size())
        sbuff.clear();
    else
        sbuff = sbuff.substr(writeSum);
    return writeSum;
}

void handle_for_sigpipe()
{
    struct sigaction sa;
    memset(&sa, '\0', sizeof(sa));
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    if(sigaction(SIGPIPE, &sa, NULL))
        return;
}

int setSocketNonBlocking(int fd)
{
    int flag = fcntl(fd, F_GETFL, 0);
    if(flag == -1)
        return -1;

    flag |= O_NONBLOCK;
    if(fcntl(fd, F_SETFL, flag) == -1)
        return -1;
    return 0;
}

std::vector<std::string> StringUtil::mySplit(const std::string& str, std::string sp_string)  // split(), str 是要分割的string
{ 
    std::vector<std::string> vecString; 
    int sp_stringLen = sp_string.size(); 
    int lastPosition = 0; 
    int index = -1; 
	//如果要分割的串就是自己怎么写？
    while(-1 != (index=str.find(sp_string , lastPosition))) 
    { 
		if(str.substr(lastPosition, index - lastPosition) != std::string("")) //避免第一个字符串就是sp_string
		{
			vecString.push_back(str.substr(lastPosition, index - lastPosition));
		}
        lastPosition = index +sp_stringLen; 
    } 
    std::string lastStr = str.substr(lastPosition); 
    if ( !lastStr.empty() ) 
    { 
        vecString.push_back(lastStr); 
    } 
    return vecString; 
}

void StringUtil::parseParam(std::string paramsStr, std::map<std::string, std::string> &reqParams)
{

	std::vector<std::string> pairslist = StringUtil::mySplit(paramsStr, "&");

	for (size_t i = 0; i < pairslist.size(); ++i)
	{
		std::string name = pairslist[i].substr(0, pairslist[i].find("="));
		std::string value = pairslist[i].substr(pairslist[i].find("=") + 1);

		reqParams[name] = value;
	}
}

std::string StringUtil::replace_all(std::string& str, const std::string& old_value, const std::string& new_value)     
{     
    while(true)   
    {     
        std::string::size_type   pos(0);     
	    if( (pos=str.find(old_value)) != std::string::npos)     
        { 
			str.replace(pos,old_value.length(),new_value);  
		}   
        else  
		{ 
			break;
		}
    }     
    
	return str;
}  

std::string ShellUtil::execShellPipe(std::string cmd)
{
	char res[1024]={0};
	 
	FILE *fp = popen(cmd.c_str(),"r");
	
	if( fp != NULL)
	{
		//fgets( res, sizeof(res), fp ); //遇到\n终止复制
		fread( res, sizeof(char), sizeof(res), fp );
		pclose(fp);
	}
	
	return res;
}