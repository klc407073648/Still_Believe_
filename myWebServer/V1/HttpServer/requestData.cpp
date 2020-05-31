#include "requestData.h"
#include "util.h"
#include "epoll.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <queue>
#include <cstdlib>
/*
#include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace cv;
*/
//test
#include <iostream>

const std::string rootPath = "./webContent/";
using namespace std;

pthread_once_t MimeType::once_control = PTHREAD_ONCE_INIT;
std::unordered_map<std::string, std::string> MimeType::mime;
std::string RequestData::user;
std::string RequestData::password;

char favicon[555] = {
    '\x89', 'P',    'N',    'G',    '\xD',  '\xA',  '\x1A', '\xA',  '\x0',
    '\x0',  '\x0',  '\xD',  'I',    'H',    'D',    'R',    '\x0',  '\x0',
    '\x0',  '\x10', '\x0',  '\x0',  '\x0',  '\x10', '\x8',  '\x6',  '\x0',
    '\x0',  '\x0',  '\x1F', '\xF3', '\xFF', 'a',    '\x0',  '\x0',  '\x0',
    '\x19', 't',    'E',    'X',    't',    'S',    'o',    'f',    't',
    'w',    'a',    'r',    'e',    '\x0',  'A',    'd',    'o',    'b',
    'e',    '\x20', 'I',    'm',    'a',    'g',    'e',    'R',    'e',
    'a',    'd',    'y',    'q',    '\xC9', 'e',    '\x3C', '\x0',  '\x0',
    '\x1',  '\xCD', 'I',    'D',    'A',    'T',    'x',    '\xDA', '\x94',
    '\x93', '9',    'H',    '\x3',  'A',    '\x14', '\x86', '\xFF', '\x5D',
    'b',    '\xA7', '\x4',  'R',    '\xC4', 'm',    '\x22', '\x1E', '\xA0',
    'F',    '\x24', '\x8',  '\x16', '\x16', 'v',    '\xA',  '6',    '\xBA',
    'J',    '\x9A', '\x80', '\x8',  'A',    '\xB4', 'q',    '\x85', 'X',
    '\x89', 'G',    '\xB0', 'I',    '\xA9', 'Q',    '\x24', '\xCD', '\xA6',
    '\x8',  '\xA4', 'H',    'c',    '\x91', 'B',    '\xB',  '\xAF', 'V',
    '\xC1', 'F',    '\xB4', '\x15', '\xCF', '\x22', 'X',    '\x98', '\xB',
    'T',    'H',    '\x8A', 'd',    '\x93', '\x8D', '\xFB', 'F',    'g',
    '\xC9', '\x1A', '\x14', '\x7D', '\xF0', 'f',    'v',    'f',    '\xDF',
    '\x7C', '\xEF', '\xE7', 'g',    'F',    '\xA8', '\xD5', 'j',    'H',
    '\x24', '\x12', '\x2A', '\x0',  '\x5',  '\xBF', 'G',    '\xD4', '\xEF',
    '\xF7', '\x2F', '6',    '\xEC', '\x12', '\x20', '\x1E', '\x8F', '\xD7',
    '\xAA', '\xD5', '\xEA', '\xAF', 'I',    '5',    'F',    '\xAA', 'T',
    '\x5F', '\x9F', '\x22', 'A',    '\x2A', '\x95', '\xA',  '\x83', '\xE5',
    'r',    '9',    'd',    '\xB3', 'Y',    '\x96', '\x99', 'L',    '\x6',
    '\xE9', 't',    '\x9A', '\x25', '\x85', '\x2C', '\xCB', 'T',    '\xA7',
    '\xC4', 'b',    '1',    '\xB5', '\x5E', '\x0',  '\x3',  'h',    '\x9A',
    '\xC6', '\x16', '\x82', '\x20', 'X',    'R',    '\x14', 'E',    '6',
    'S',    '\x94', '\xCB', 'e',    'x',    '\xBD', '\x5E', '\xAA', 'U',
    'T',    '\x23', 'L',    '\xC0', '\xE0', '\xE2', '\xC1', '\x8F', '\x0',
    '\x9E', '\xBC', '\x9',  'A',    '\x7C', '\x3E', '\x1F', '\x83', 'D',
    '\x22', '\x11', '\xD5', 'T',    '\x40', '\x3F', '8',    '\x80', 'w',
    '\xE5', '3',    '\x7',  '\xB8', '\x5C', '\x2E', 'H',    '\x92', '\x4',
    '\x87', '\xC3', '\x81', '\x40', '\x20', '\x40', 'g',    '\x98', '\xE9',
    '6',    '\x1A', '\xA6', 'g',    '\x15', '\x4',  '\xE3', '\xD7', '\xC8',
    '\xBD', '\x15', '\xE1', 'i',    '\xB7', 'C',    '\xAB', '\xEA', 'x',
    '\x2F', 'j',    'X',    '\x92', '\xBB', '\x18', '\x20', '\x9F', '\xCF',
    '3',    '\xC3', '\xB8', '\xE9', 'N',    '\xA7', '\xD3', 'l',    'J',
    '\x0',  'i',    '6',    '\x7C', '\x8E', '\xE1', '\xFE', 'V',    '\x84',
    '\xE7', '\x3C', '\x9F', 'r',    '\x2B', '\x3A', 'B',    '\x7B', '7',
    'f',    'w',    '\xAE', '\x8E', '\xE',  '\xF3', '\xBD', 'R',    '\xA9',
    'd',    '\x2',  'B',    '\xAF', '\x85', '2',    'f',    'F',    '\xBA',
    '\xC',  '\xD9', '\x9F', '\x1D', '\x9A', 'l',    '\x22', '\xE6', '\xC7',
    '\x3A', '\x2C', '\x80', '\xEF', '\xC1', '\x15', '\x90', '\x7',  '\x93',
    '\xA2', '\x28', '\xA0', 'S',    'j',    '\xB1', '\xB8', '\xDF', '\x29',
    '5',    'C',    '\xE',  '\x3F', 'X',    '\xFC', '\x98', '\xDA', 'y',
    'j',    'P',    '\x40', '\x0',  '\x87', '\xAE', '\x1B', '\x17', 'B',
    '\xB4', '\x3A', '\x3F', '\xBE', 'y',    '\xC7', '\xA',  '\x26', '\xB6',
    '\xEE', '\xD9', '\x9A', '\x60', '\x14', '\x93', '\xDB', '\x8F', '\xD',
    '\xA',  '\x2E', '\xE9', '\x23', '\x95', '\x29', 'X',    '\x0',  '\x27',
    '\xEB', 'n',    'V',    'p',    '\xBC', '\xD6', '\xCB', '\xD6', 'G',
    '\xAB', '\x3D', 'l',    '\x7D', '\xB8', '\xD2', '\xDD', '\xA0', '\x60',
    '\x83', '\xBA', '\xEF', '\x5F', '\xA4', '\xEA', '\xCC', '\x2',  'N',
    '\xAE', '\x5E', 'p',    '\x1A', '\xEC', '\xB3', '\x40', '9',    '\xAC',
    '\xFE', '\xF2', '\x91', '\x89', 'g',    '\x91', '\x85', '\x21', '\xA8',
    '\x87', '\xB7', 'X',    '\x7E', '\x7E', '\x85', '\xBB', '\xCD', 'N',
    'N',    'b',    't',    '\x40', '\xFA', '\x93', '\x89', '\xEC', '\x1E',
    '\xEC', '\x86', '\x2',  'H',    '\x26', '\x93', '\xD0', 'u',    '\x1D',
    '\x7F', '\x9',  '2',    '\x95', '\xBF', '\x1F', '\xDB', '\xD7', 'c',
    '\x8A', '\x1A', '\xF7', '\x5C', '\xC1', '\xFF', '\x22', 'J',    '\xC3',
    '\x87', '\x0',  '\x3',  '\x0',  'K',    '\xBB', '\xF8', '\xD6', '\x2A',
    'v',    '\x98', 'I',    '\x0',  '\x0',  '\x0',  '\x0',  'I',    'E',
    'N',    'D',    '\xAE', 'B',    '\x60', '\x82',
};

void MimeType::init()
{
    mime[".html"] = "text/html";
    mime[".avi"] = "video/x-msvideo";
    mime[".bmp"] = "image/bmp";
    mime[".c"] = "text/plain";
    mime[".doc"] = "application/msword";
    mime[".gif"] = "image/gif";
    mime[".gz"] = "application/x-gzip";
    mime[".htm"] = "text/html";
    mime[".ico"] = "application/x-ico";
    mime[".jpg"] = "image/jpeg";
    mime[".png"] = "image/png";
    mime[".txt"] = "text/plain";
    mime[".mp3"] = "audio/mp3";
	//mime[".cgi"] = "exec/cgi";
    mime["default"] = "text/html";
}

std::string MimeType::getMime(const std::string &suffix)
{
    pthread_once(&once_control, MimeType::init);
    if (mime.find(suffix) == mime.end())
        return mime["default"];
    else
        return mime[suffix];
}

RequestData::RequestData(): 
    now_read_pos(0), 
    state(STATE_PARSE_URI), 
    h_state(H_START), 
    keep_alive(false), 
    isAbleRead(true),
    isAbleWrite(false),
    events(0),
    error(false)
{
    cout << "RequestData()" << endl;
}

RequestData::RequestData(int _epollfd, int _fd, std::string _path):
    now_read_pos(0), 
    state(STATE_PARSE_URI), 
    h_state(H_START), 
    keep_alive(false), 
    path(_path), 
    fd(_fd), 
    epollfd(_epollfd),
    isAbleRead(true),
    isAbleWrite(false),
    events(0),
    error(false)
{
    cout << "RequestData()" << endl;
}

RequestData::~RequestData()
{
    cout << "~RequestData()" << endl;
    close(fd);
}

void RequestData::linkTimer(shared_ptr<TimerNode> mtimer)
{
    // shared_ptr重载了bool, 但weak_ptr没有
    //if (!timer.lock())
    timer = mtimer;
}

int RequestData::getFd()
{
    return fd;
}
void RequestData::setFd(int _fd)
{
    fd = _fd;
}

std::string RequestData::getPath()
{
    return path;
}

void RequestData::setPath(std::string _path)
{
    path = _path;
}


void RequestData::reset()
{
    inBuffer.clear();
    file_name.clear();
    //path.clear();
    realFile.clear();
	//user.clear();
	//password.clear();
    now_read_pos = 0;
    state = STATE_PARSE_URI;
    h_state = H_START;
    headers.clear();
    //keep_alive = false;
    if (timer.lock())
    {
        shared_ptr<TimerNode> my_timer(timer.lock());
        my_timer->clearReq();
        timer.reset();
    }
}

void RequestData::seperateTimer()
{
    //cout << "seperateTimer" << endl;
    if (timer.lock())
    {
        shared_ptr<TimerNode> my_timer(timer.lock());
        my_timer->clearReq();
        timer.reset();
    }
}

void RequestData::handleRead()
{
    do
    {
        int read_num = readn(fd, inBuffer);
        //printf("read_num=%d\n", read_num);
		cout<<"inBuffer:"<<inBuffer<<endl;
		cout<<"---------------------------"<<endl;
        if (read_num < 0)
        {
            perror("1");
            error = true;
            handleError(fd, 400, "Bad Request");
            break;
        }
        else if (read_num == 0)
        {
            // 有请求出现但是读不到数据，可能是Request Aborted，或者来自网络的数据没有达到等原因
            // 最可能是对端已经关闭了，统一按照对端已经关闭处理
            error = true;
            break; 
        }

        if (state == STATE_PARSE_URI)
        {
            URIState flag = this->parseURI();
            if (flag == PARSE_URI_AGAIN)
                break;
            else if (flag == PARSE_URI_ERROR)
            {
                perror("2");
                error = true;
                handleError(fd, 400, "Bad Request");
                break;
            }
            else
                state = STATE_PARSE_HEADERS;
        }

        if (state == STATE_PARSE_HEADERS)
        {
            HeaderState flag = this->parseHeaders();
            if (flag == PARSE_HEADER_AGAIN)
                break;
            else if (flag == PARSE_HEADER_ERROR)
            {
                perror("3");
                error = true;
                handleError(fd, 400, "Bad Request");
                break;
            }

            if(method == METHOD_POST)
            {
                // POST方法准备
                state = STATE_RECV_BODY;
            }
            else 
            {
                state = STATE_ANALYSIS;
            }
        }

        if (state == STATE_RECV_BODY)
        {
            int content_length = -1;
            if (headers.find("Content-Length") != headers.end())
            {
                content_length = stoi(headers["Content-Length"]);
            }
            else
            {
                error = true;
                handleError(fd, 400, "Bad Request: Lack of argument (Content-Length)");
                break;
            }

            if (inBuffer.size() < content_length)
                break;

            state = STATE_ANALYSIS;
        }
        if (state == STATE_ANALYSIS)
        {
            AnalysisState flag = this->analysisRequest();
            if (flag == ANALYSIS_SUCCESS)
            {
                state = STATE_FINISH;
                break;
            }
            else
            {
                error = true;
                break;
            }
        }
    } while (false);

    if (!error)
    {
        if (outBuffer.size() > 0)
            events |= EPOLLOUT;

        if (state == STATE_FINISH)
        {
            cout << "keep_alive=" << keep_alive << endl;
            if (keep_alive)
            {
                this->reset();
                events |= EPOLLIN;
            }
            else
                return;
        }
        else
            events |= EPOLLIN;
    }
}

void RequestData::handleWrite()
{
    if (!error)
    {
        if (writen(fd, outBuffer) < 0)
        {
            perror("writen");
            events = 0;
            error = true;
        }
        else if (outBuffer.size() > 0)
            events |= EPOLLOUT;
    }
}

void RequestData::handleConn()
{
    if (!error)
    {
        if (events != 0)
        {
            // 一定要先加时间信息，否则可能会出现刚加进去，下个in触发来了，然后分离失败后，又加入队列，最后超时被删，然后正在线程中进行的任务出错，double free错误。
            // 新增时间信息
            int timeout = 2000;
            if (keep_alive)
                timeout = 5 * 60 * 1000;
            isAbleRead = false;
            isAbleWrite = false;
            Epoll::add_timer(shared_from_this(), timeout);

            if ((events & EPOLLIN) && (events & EPOLLOUT))
            {
                events = __uint32_t(0);
                events |= EPOLLOUT;
            }
            events |= (EPOLLET | EPOLLONESHOT);
            __uint32_t _events = events;
            events = 0;
            if (Epoll::epoll_mod(fd, shared_from_this(), _events) < 0)
            {
                printf("Epoll::epoll_mod error\n");
            }
        }
        else if (keep_alive)
        {
            events |= (EPOLLIN | EPOLLET | EPOLLONESHOT);
            int timeout = 5 * 60 * 1000;
            isAbleRead = false;
            isAbleWrite = false;
            Epoll::add_timer(shared_from_this(), timeout);
            __uint32_t _events = events;
            events = 0;
            if (Epoll::epoll_mod(fd, shared_from_this(), _events) < 0)
            {
                printf("Epoll::epoll_mod error\n");
            }
        }
    }
}


URIState RequestData::parseURI()
{
    string &str = inBuffer;
    // 读到完整的请求行再开始解析请求
    int pos = str.find('\r', now_read_pos);
    if (pos < 0)
    {
        return PARSE_URI_AGAIN;
    }
    // 去掉请求行所占的空间，节省空间
    string request_line = str.substr(0, pos);
    if (str.size() > pos + 1)
        str = str.substr(pos + 1);
    else 
        str.clear();

    // Method
    int posGet = request_line.find("GET");
    int posPost = request_line.find("POST");
    int posHead = request_line.find("HEAD");

    if (posGet >= 0) 
    {
        pos = posGet;
        method = METHOD_GET;
    } 
    else if (posPost >= 0) 
    {
        pos = posPost;
        method = METHOD_POST;
		cout<<"POST REQUEST"<<endl;
    } 
    else if (posHead >= 0) 
    {
        pos = posHead;
        method = METHOD_HEAD;
    } 
    else 
    {
        return PARSE_URI_ERROR;
    }


    pos = request_line.find("/", pos);
    if (pos < 0)
        return PARSE_URI_ERROR;
    else
    {
        int _pos = request_line.find(' ', pos);
        if (_pos < 0)
            return PARSE_URI_ERROR;
        else
        {
            if (_pos - pos > 1)
            {
                file_name = request_line.substr(pos + 1, _pos - pos - 1);
                int __pos = file_name.find('?');
                if (__pos >= 0)
                {
                    file_name = file_name.substr(0, __pos);
                }
            }
                
            else
                file_name = "judge.html";
        }
        pos = _pos;
    }
    cout << "file_name: " << file_name << endl;
	
    // HTTP 版本号
    pos = request_line.find("/", pos);
    if (pos < 0)
        return PARSE_URI_ERROR;
    else
    {
        if (request_line.size() - pos <= 3)
            return PARSE_URI_ERROR;
        else
        {
            string ver = request_line.substr(pos + 1, 3);
            if (ver == "1.0")
                httpVersion = HTTP_10;
            else if (ver == "1.1")
                httpVersion = HTTP_11;
            else
                return PARSE_URI_ERROR;
        }
    }
    return PARSE_URI_SUCCESS;
}

HeaderState RequestData::parseHeaders()
{
  string &str = inBuffer;
  int key_start = -1, key_end = -1, value_start = -1, value_end = -1;
  int now_read_line_begin = 0;
  bool notFinish = true;
  size_t i = 0;
  for (; i < str.size() && notFinish; ++i) {
    switch (h_state) {
      case H_START: {
        if (str[i] == '\n' || str[i] == '\r') break;
        h_state = H_KEY;
        key_start = i;
        now_read_line_begin = i;
        break;
      }
      case H_KEY: {
        if (str[i] == ':') {
          key_end = i;
          if (key_end - key_start <= 0) return PARSE_HEADER_ERROR;
          h_state = H_COLON;
        } else if (str[i] == '\n' || str[i] == '\r')
          return PARSE_HEADER_ERROR;
        break;
      }
      case H_COLON: {
        if (str[i] == ' ') {
          h_state = H_SPACES_AFTER_COLON;
        } else
          return PARSE_HEADER_ERROR;
        break;
      }
      case H_SPACES_AFTER_COLON: {
        h_state = H_VALUE;
        value_start = i;
        break;
      }
      case H_VALUE: {
        if (str[i] == '\r') {
          h_state = H_CR;
          value_end = i;
          if (value_end - value_start <= 0) return PARSE_HEADER_ERROR;
        } else if (i - value_start > 255)
          return PARSE_HEADER_ERROR;
        break;
      }
      case H_CR: {
        if (str[i] == '\n') {
          h_state = H_LF;
          string key(str.begin() + key_start, str.begin() + key_end);
          string value(str.begin() + value_start, str.begin() + value_end);
          headers[key] = value;
          now_read_line_begin = i;
        } else
          return PARSE_HEADER_ERROR;
        break;
      }
      case H_LF: {
        if (str[i] == '\r') {
          h_state = H_END_CR;
        } else {
          key_start = i;
          h_state = H_KEY;
        }
        break;
      }
      case H_END_CR: {
        if (str[i] == '\n') {
          h_state = H_END_LF;
        } else
          return PARSE_HEADER_ERROR;
        break;
      }
      case H_END_LF: {
        notFinish = false;
        key_start = i;
        now_read_line_begin = i;
        break;
      }
    }
  }
  if (h_state == H_END_LF) {
    str = str.substr(now_read_line_begin);
    return PARSE_HEADER_SUCCESS;
  }
  str = str.substr(now_read_line_begin);
  return PARSE_HEADER_AGAIN;
}

std::string RequestData::execShellPipe(std::string cmd)
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

AnalysisState RequestData::analysisPostNumberRequest(char number)
{	
    switch (number)
    {
    case'0':
        realFile = rootPath + "register.html";
        break;
    case'1':
        realFile = rootPath + "log.html";
        break;
    case'5':
        realFile = rootPath + "picture.html";
        break;
    case'6':
        realFile = rootPath + "video.html";
        break;
    case'7':
        realFile = rootPath + "fans.html";
        break;
    default:
        break;
    }
    string header;
    header += string("HTTP/1.1 200 OK\r\n");
    if(headers.find("Connection") != headers.end() && headers["Connection"] == "Keep-Alive")
    {
        keep_alive = true;
        header += string("Connection: Keep-alive\r\n") + "Keep-Alive: timeout=" + to_string(5 * 60 * 1000) + "\r\n";
    }

    cout<<"realFile: "<<realFile<<endl;
    //string header="";
    struct stat sbuf;
    if (stat(realFile.c_str(), &sbuf) < 0)
    {
        cout<<"realFile.c_str(), &sbuf) < 0"<<endl;
        header.clear();
        handleError(fd, 404, "Not Found!");
        return ANALYSIS_ERROR;
    }
    cout<<"after realFile.c_str(), &sbuf) < 0"<<endl;
    header += "Content-type: text/html\r\n";
    header += "Content-Length: " + to_string(sbuf.st_size) + "\r\n";
    // 头部结束
    header += "\r\n";

    outBuffer += header;
    cout<<"outBuffer: "<<outBuffer<<endl;
    int src_fd = open(realFile.c_str(), O_RDONLY, 0);
    char *src_addr = static_cast<char*>(mmap(NULL, sbuf.st_size, PROT_READ, MAP_PRIVATE, src_fd, 0));
    close(src_fd);

    outBuffer += src_addr;
    munmap(src_addr, sbuf.st_size);
	//return ANALYSIS_SUCCESS;
}

AnalysisState RequestData::analysisPostCGIRequest(int flag)
{	
    string header;
    header += string("HTTP/1.1 200 OK\r\n");
    if(headers.find("Connection") != headers.end() && headers["Connection"] == "Keep-Alive")
    {
        keep_alive = true;
        header += string("Connection: Keep-alive\r\n") + "Keep-Alive: timeout=" + to_string(5 * 60 * 1000) + "\r\n";
    }

    int length = stoi(headers["Content-Length"]);
    vector<char> data(inBuffer.begin(), inBuffer.begin() + length);

    string postInfo =inBuffer.substr(0,length);
    cout<<"METHOD_POST inBuffer "<<inBuffer.substr(0,length)<<endl;

    int pos1 = postInfo.find_first_of("=");
    int pos2 = postInfo.find_first_of("&");
    int pos3 = postInfo.find_last_of("=");

    user = postInfo.substr(pos1+1,pos2-pos1-1);
    password = postInfo.substr(pos3+1);
    cout<<"user: "<<user<<" password: "<<password<<endl;

    cout<<"enter exec/cgi"<<endl;
    string cmd = "./CGImysql/CGISQL.cgi -u " + user + " -p " + password + " -n yourdb" + " -f " +to_string(flag);
    string res = execShellPipe(cmd);
    cout<<"res: "<<res<<endl;

    if ( res == string("1\n"))
    {
        if( flag == 2 )//登陆校验
        {
            realFile=rootPath + "welcome.html";
        }
        else if( flag == 3 )//注册校验
        {
            realFile=rootPath + "log.html";
        }     
    }
    else if ( res == string("0\n"))
    {
        if( flag == 2 )
        {
            realFile=rootPath + "logError.html";
        }
        else if( flag == 3 )
        {
            realFile=rootPath + "registerError.html";
        }
    }
            
    cout<<"realFile: "<<realFile<<endl;

    struct stat sbuf;
    if (stat(realFile.c_str(), &sbuf) < 0)
    {
        header.clear();
        handleError(fd, 404, "Not Found!");
        return ANALYSIS_ERROR;
    }

    header += "Content-type: text/html\r\n";
    //text/html
    header += "Content-Length: " + to_string(sbuf.st_size) + "\r\n";
    // 头部结束
    header += "\r\n";
    outBuffer += header;
    cout<<"outBuffer: "<<outBuffer<<endl;

    int src_fd = open(realFile.c_str(), O_RDONLY, 0);
    char *src_addr = static_cast<char*>(mmap(NULL, sbuf.st_size, PROT_READ, MAP_PRIVATE, src_fd, 0));
    close(src_fd);

    outBuffer += src_addr;
    munmap(src_addr, sbuf.st_size);
	//return ANALYSIS_SUCCESS;
}

AnalysisState RequestData::analysisRequest()
{
    if (method == METHOD_POST)
    {
        //get inBuffer
        //string header;
      
        //解析POST为数字对应的实际文件
		//不能在analysisPostNumberRequest或analysisPostCGIRequest直接返回 ANALYSIS_SUCCESS，因为analysisRequest需要返回值，应该从主函数返回
        if (file_name.size() == 1 && (file_name[0]>='0'&&file_name[0]<='7') )
        {
            analysisPostNumberRequest(file_name[0]);
        }
		
		if(file_name == "2CGISQL.cgi"||file_name == "3CGISQL.cgi")
		{
			if (file_name == "2CGISQL.cgi")
				analysisPostCGIRequest(2);
			else
				analysisPostCGIRequest(3);
        }
		/* string user = postInfo.substr(pos1+1,pos2-pos1-1);
		string password = postInfo.substr(pos3+1);
		cout<<"user: "<<user<<" password: "<<password<<endl; */
		
      /*cout << " data.size()=" << data.size() << endl;
        Mat src = imdecode(data, CV_LOAD_IMAGE_ANYDEPTH|CV_LOAD_IMAGE_ANYCOLOR);
        imwrite("receive.bmp", src);
        cout << "1" << endl;
        Mat res = stitch(src);
        cout << "2" << endl;
        vector<uchar> data_encode;
        imencode(".png", res, data_encode);
        cout << "3" << endl;
        header += string("Content-Length: ") + to_string(data_encode.size()) + "\r\n\r\n";
        cout << "4" << endl;
        outBuffer += header + string(data_encode.begin(), data_encode.end());
        cout << "5" << endl;
        inBuffer = inBuffer.substr(length);*/

        return ANALYSIS_SUCCESS;
    }
    else if (method == METHOD_GET)
    {
        string header;
        header += "HTTP/1.1 200 OK\r\n";
        if(headers.find("Connection") != headers.end() && headers["Connection"] == "Keep-Alive")
        {
            keep_alive = true;
            header += string("Connection: Keep-Alive\r\n") + "Keep-Alive: timeout=" + to_string(5 * 60 * 1000) + "\r\n";
        }
        int dot_pos = file_name.find('.');
        string filetype;
        if (dot_pos < 0) 
            filetype = MimeType::getMime("default");
        else
            filetype = MimeType::getMime(file_name.substr(dot_pos));
		
		cout<<"path:"<<path<<endl;
		realFile = rootPath + file_name;
		cout<<"realFile"<<realFile<<endl;
		

		if (file_name == "favicon.ico") 
		{
		  header += "Content-Type: image/png\r\n";
		  header += "Content-Length: " + to_string(sizeof favicon) + "\r\n";
		  header += "Server: LinYa's Web Server\r\n";

		  header += "\r\n";
		  outBuffer += header;
		  outBuffer += string(favicon, favicon + sizeof favicon);
		  return ANALYSIS_SUCCESS;
		}
		
        struct stat sbuf;
        if (stat(realFile.c_str(), &sbuf) < 0)
        {
            header.clear();
            handleError(fd, 404, "Not Found!");
            return ANALYSIS_ERROR;
        }
        header += "Content-type: " + filetype + "\r\n";
        header += "Content-Length: " + to_string(sbuf.st_size) + "\r\n";
        // 头部结束
        header += "\r\n";
        outBuffer += header;
      
        int src_fd = open(realFile.c_str(), O_RDONLY, 0);
        char *src_addr = static_cast<char*>(mmap(NULL, sbuf.st_size, PROT_READ, MAP_PRIVATE, src_fd, 0));
        close(src_fd);

        outBuffer += src_addr;
        munmap(src_addr, sbuf.st_size);
   
        return ANALYSIS_SUCCESS;
    }
    else
        return ANALYSIS_ERROR;
}

void RequestData::handleError(int fd, int err_num, string short_msg)
{
    short_msg = " " + short_msg;
    char send_buff[MAX_BUFF];
    string body_buff, header_buff;
    body_buff += "<html><title>哎~出错了</title>";
    body_buff += "<body bgcolor=\"ffffff\">";
    body_buff += to_string(err_num) + short_msg;
    body_buff += "<hr><em> STLBE's Web Server</em>\n</body></html>";

    header_buff += "HTTP/1.1 " + to_string(err_num) + short_msg + "\r\n";
    header_buff += "Content-type: text/html\r\n";
    header_buff += "Connection: close\r\n";
    header_buff += "Content-Length: " + to_string(body_buff.size()) + "\r\n";
    header_buff += "\r\n";
    // 错误处理不考虑writen不完的情况
    sprintf(send_buff, "%s", header_buff.c_str());
    writen(fd, send_buff, strlen(send_buff));
    sprintf(send_buff, "%s", body_buff.c_str());
    writen(fd, send_buff, strlen(send_buff));
}


void RequestData::disableReadAndWrite()
{
    isAbleRead = false;
    isAbleWrite = false;
}
void RequestData::enableRead()
{
    isAbleRead = true;
}
void RequestData::enableWrite()
{
    isAbleWrite = true;
}
bool RequestData::canRead()
{
    return isAbleRead;
}
bool RequestData::canWrite()
{
    return isAbleWrite;
}