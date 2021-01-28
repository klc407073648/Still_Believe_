#include "requestData.h"
#include "util.h"
#include "epoll.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <queue>
#include <cstdlib>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <cstdlib>

using namespace std;
const std::string rootPath = "./webContent/";

std::map<std::string, Json::Value> RequestData::rest_uri;

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
	
    if(rest_uri.size()==0)
	{
		parseInitJson();
	}
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

    if (rest_uri.size() == 0)
    {
        parseInitJson();
    }
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
    cout << "seperateTimer" << endl;
    if (timer.lock())
    {
        shared_ptr<TimerNode> my_timer(timer.lock());
        my_timer->clearReq();
        timer.reset();
    }
}

void RequestData::parseInitJson()
{
    Json::Reader reader;
    Json::Value data;

    ifstream in("./webContent/db.json", ios::binary);

    if (!in.is_open())
    {
        cout << "Error opening file\n";
        return;
    }

    if (reader.parse(in, data))
    {
        Json::Value::Members mem = data.getMemberNames();

        for (auto iter = mem.begin(); iter != mem.end(); iter++)
        {
            std::string m_key = *iter;
            Json::Value m_value = data[*iter];

            rest_uri.insert(make_pair(m_key, m_value));
        }
    }

    for (auto iter = rest_uri.begin(); iter != rest_uri.end(); ++iter)
    {
        cout << "mem:" << iter->first << " value:" << iter->second << endl;
    }
}

void RequestData::handleRead()
{
    cout << "handleRead begin" << endl;

    do
    {
        //从fd获取信息存储到inBuffer
        int read_num = readn(fd, inBuffer);
        //printf("read_num=%d\n", read_num);
        printf("read_num: %d\n , inBuffer: \n [%s]", read_num, inBuffer.c_str());
        //cout << "read_num: " << read_num << " inBuffer: " << inBuffer << endl;
        //cout << "---------------------------" << endl;

        //判断读取的字符数量
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

        printf("begin to parseURI\n");
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

        printf("begin to parseHeaders\n");
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

            if (method == METHOD_POST || method == METHOD_PUT || method == METHOD_DELETE)
            {
                // POST方法准备
                state = STATE_RECV_BODY;
            }
            else
            {
                state = STATE_ANALYSIS;
            }
        }

        printf("begin to ensure if STATE_RECV_BODY\n");
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

        printf("begin to analysisRequest\n");
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

    vector<std::string> vec = {"null", "GET", "POST", "PUT", "DELETE", "HEAD"};

    for (int i = 1; i < vec.size(); i++)
    {
        int posTemp = request_line.find(vec[i]);

        if (posTemp >= 0)
        {
            pos = posTemp;

            switch (i)
            {
            case 1:
                method = METHOD_GET;
                break;
            case 2:
                method = METHOD_POST;
                break;
            case 3:
                method = METHOD_PUT;
                break;
            case 4:
                method = METHOD_DELETE;
                break;
            case 5:
                method = METHOD_HEAD;
                break;
            }

            cout << "method: " << vec[i] << " method value: " << method << endl;
            break;
        }
    }

    //POST /login HTTP/1.1
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
                file_name = "db.json";
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

//解析请求报头，按照:保存为map结构
HeaderState RequestData::parseHeaders()
{
    string &str = inBuffer;
    int key_start = -1, key_end = -1, value_start = -1, value_end = -1;
    int now_read_line_begin = 0;
    bool notFinish = true;
    size_t i = 0;
    for (; i < str.size() && notFinish; ++i)
    {
        switch (h_state)
        {
        case H_START:
        {
            if (str[i] == '\n' || str[i] == '\r')
                break;
            h_state = H_KEY;
            key_start = i;
            now_read_line_begin = i;
            break;
        }
        case H_KEY:
        {
            if (str[i] == ':')
            {
                key_end = i;
                if (key_end - key_start <= 0)
                    return PARSE_HEADER_ERROR;
                h_state = H_COLON;
            }
            else if (str[i] == '\n' || str[i] == '\r')
                return PARSE_HEADER_ERROR;
            break;
        }
        case H_COLON:
        {
            if (str[i] == ' ')
            {
                h_state = H_SPACES_AFTER_COLON;
            }
            else
                return PARSE_HEADER_ERROR;
            break;
        }
        case H_SPACES_AFTER_COLON:
        {
            h_state = H_VALUE;
            value_start = i;
            break;
        }
        case H_VALUE:
        {
            if (str[i] == '\r')
            {
                h_state = H_CR;
                value_end = i;
                if (value_end - value_start <= 0)
                    return PARSE_HEADER_ERROR;
            }
            else if (i - value_start > 255)
                return PARSE_HEADER_ERROR;
            break;
        }
        case H_CR:
        {
            if (str[i] == '\n')
            {
                h_state = H_LF;
                string key(str.begin() + key_start, str.begin() + key_end);
                string value(str.begin() + value_start, str.begin() + value_end);
                headers[key] = value;
                now_read_line_begin = i;
            }
            else
                return PARSE_HEADER_ERROR;
            break;
        }
        case H_LF:
        {
            if (str[i] == '\r')
            {
                h_state = H_END_CR;
            }
            else
            {
                key_start = i;
                h_state = H_KEY;
            }
            break;
        }
        case H_END_CR:
        {
            if (str[i] == '\n')
            {
                h_state = H_END_LF;
            }
            else
                return PARSE_HEADER_ERROR;
            break;
        }
        case H_END_LF:
        {
            notFinish = false;
            key_start = i;
            now_read_line_begin = i;
            break;
        }
        }
    }
    if (h_state == H_END_LF)
    {
        str = str.substr(now_read_line_begin);
        return PARSE_HEADER_SUCCESS;
    }
    str = str.substr(now_read_line_begin);
    return PARSE_HEADER_AGAIN;
}

void RequestData::analysisGetRequest()
{
    cout << "analysisGetRequest() begin:" << endl;

    int findPos = -1;
    Json::Value root = rest_uri[uri];

    if (root.type() == Json::objectValue)
    {
        //如果是Json::objectValue，直接返回即可
        if (uri_after == "") //Json::objectValue 必然没有 uri_after
        {
            //
        }
        else
        {
            realFile = "";
            return;
        }
    }
    else if (root.type() == Json::arrayValue)
    {
        if (uri_after == "")
        {
            //
        }
        else if (root.size() >= 0)
        {

            for (int i = 0; i < root.size(); i++)
            {

                Json::Value::Members mem = root[i].getMemberNames();
                for (auto iter = mem.begin(); iter != mem.end(); iter++)
                {
                    string m_key = *iter;
                    string m_value = root[i][*iter].toStyledString();
                    cout << "Json::arrayValue m_key:[" << m_key << "] m_value:[" << m_value << "]" << endl;
                    cout << "Json::arrayValue uri_after:[" << uri_after << "]" << endl;
                    if (m_key == "id" && m_value == uri_after + "\n")
                    {
                        findPos = i;
                        break;
                    }
                }

                if (findPos != -1)
                {
                    root = root[findPos]; //取其中一个元素
                    cout << "analysisGetRequest() root [" << root << "]" << endl;
                    break;
                }
            }

            //上述处理过程中，没有在root的内容里找到对应 "uri_after"的内容
            if (findPos == -1)
            {
                realFile = "";
                return;
            }
        }
    }

    ofstream ofs;
    ofs.open("./webContent/get.json");
    ofs << root;
    ofs.close();

    realFile = rootPath + "get.json";
}

/*
GET    /posts
GET    /posts/1
POST   /posts
PUT    /posts/1
PATCH  /posts/1
DELETE /posts/1
*/

void RequestData::parseContent()
{
    int length = stoi(headers["Content-Length"]);
    vector<char> data(inBuffer.begin(), inBuffer.begin() + length);

    string postInfo = inBuffer.substr(0, length);
    cout << "postInfo: " << postInfo << endl;

    StringUtil::parseParam(postInfo, m_paramsMap);
}

void RequestData::analysisPostRequest()
{
    //已经存在的原始不能posts，返回404
    cout << "[analysisPostRequest] bgein" << endl;

    if (uri_after != "")
    {
        realFile = "";
        return;
    }

    Json::Value &root = rest_uri[uri]; //获取uri对应的数据，一般为Json::objectValue或Json::arrayValue类型

    //如果是Json::objectValue，直接替换里面的内容
    if (root.type() == Json::objectValue)
    {
        cout << "[analysisPostRequest] root.type() == Json::objectValue" << endl;
        Json::Value data;

        //重新组织内容返回
        for (auto m_iter = m_paramsMap.begin(); m_iter != m_paramsMap.end(); ++m_iter)
        {
            data[m_iter->first] = m_iter->second;
        }

        root = data;
    }
    else if (root.type() == Json::arrayValue)
    {
        cout << "[analysisPostRequest] root.type() == Json::arrayValue" << endl;

        for (int i = 0; i < root.size(); i++)
        {
            Json::Value::Members mem = root[i].getMemberNames();
            for (auto iter = mem.begin(); iter != mem.end(); iter++)
            {
                string m_key = *iter;
                string m_value = root[i][*iter].toStyledString();

                //遗留不填写id值的处理
                //cout << "m_value:[" << m_value << "] ,m_paramsMap[id]" << m_paramsMap["id"] << endl;
                if (m_key == "id" && m_value == m_paramsMap["id"] + "\n")
                {
                    realFile = "";
                    return;
                }
            }
        }

        //如果id，不存在就重新组织一个
        Json::Value data;

        //重新组织内容返回
        for (auto m_iter = m_paramsMap.begin(); m_iter != m_paramsMap.end(); ++m_iter)
        {
            if (m_iter->first == "id")
            {
                data[m_iter->first] = atoi((m_iter->second).c_str());
            }
            else
            {
                data[m_iter->first] = m_iter->second;
            }
        }

        cout << "before:" << root << endl;
        root[root.size()] = data;
        cout << "after:" << root << endl;
    }

    ofstream ofs;
    ofs.open("./webContent/post.json");
    ofs << root;
    ofs.close();

    realFile = rootPath + "post.json";

    cout << "[analysisPostRequest] end" << endl;
}

void RequestData::analysisPutRequest()
{

    //已经存在的原始不能posts，返回404
    cout << "[analysisPutRequest] begin" << endl;

    Json::Reader reader;
    Json::Value &root = rest_uri[uri];
    Json::Value next;

    bool isFind = false;

    //如果是Json::objectValue，直接替换里面的内容
    if (root.type() == Json::objectValue && uri_after == "")
    {
        cout << "root.type() == Json::objectValue" << endl;
        Json::Value data;

        //重新组织内容返回
        for (auto m_iter = m_paramsMap.begin(); m_iter != m_paramsMap.end(); ++m_iter)
        {
            data[m_iter->first] = m_iter->second;
        }

        root = data;
        next = root;
    }
    else if (root.type() == Json::arrayValue && uri_after != "")
    {

        cout << "root.type() == Json::arrayValue" << endl;
        for (int i = 0; i < root.size(); i++)
        {
            cout << "This is i:" << i << endl;
            Json::Value::Members mem = root[i].getMemberNames();
            for (auto iter = mem.begin(); iter != mem.end(); iter++)
            {
                string m_key = *iter;
                string m_value = root[i][*iter].toStyledString();

                if (m_key == "id" && m_value == uri_after + "\n")
                {
                    cout << " in loop m_key:[" << m_key << "] m_value:[" << m_value << "]" << endl;
                    Json::Value data;

                    //重新组织内容返回
                    for (auto m_iter = m_paramsMap.begin(); m_iter != m_paramsMap.end(); ++m_iter)
                    {
                        if (m_iter->first != "id")
                        {
                            root[i][m_iter->first] = m_iter->second;
                            isFind = true;
                        }
                    }

                    next = root[i];

                    break;
                }
            }
        }

        if (!isFind)
        {
            realFile = "";
            return;
        }
    }

    ofstream ofs;
    ofs.open("./webContent/put.json");
    ofs << next;
    ofs.close();

    realFile = rootPath + "put.json";
}

void RequestData::analysisDeleteRequest()
{
    //暂时未解决，删除应该返回为{}的问题
    //已经存在的原始不能posts，返回404
    cout << "[analysisDeleteRequest] begin" << endl;

    Json::Value root = rest_uri[uri];

    Json::Value next;
    std::vector<std::string> vec;

    //如果是Json::objectValue，直接替换里面的内容
    if (root.type() == Json::objectValue)
    {
        realFile = "";
        return;
    }
    else if (root.type() == Json::arrayValue && uri_after != "")
    {

        cout << "root.type() == Json::arrayValue" << endl;
        for (int i = 0; i < root.size(); i++)
        {
            cout << "This is i:" << i << endl;
            Json::Value::Members mem = root[i].getMemberNames();
            for (auto iter = mem.begin(); iter != mem.end(); iter++)
            {
                string m_key = *iter;
                string m_value = root[i][*iter].toStyledString();

                vec.push_back(m_value);
                if (m_key == "id" && m_value != uri_after + "\n")
                {
                    next.append(root[i]);

                    break;
                }
            }
        }
    }
    string m_str = uri_after + "\n";
    std::vector<std::string>::iterator result = find(vec.begin(), vec.end(), m_str);

    if (result == vec.end())
    {
        realFile = "";
        return;
    }

    rest_uri[uri] = next;

    ofstream ofs;
    ofs.open("./webContent/delete.json");
    ofs << next;
    ofs.close();

    realFile = rootPath + "delete.json";
}

AnalysisState RequestData::analysisRequest()
{
    cout << "realFile before: " << realFile << endl;

    cout << "[analysisRequest] file_name:" << file_name << " method: " << method << endl;

    //解析 posts/1 为 uri=posts , uri_after=1
    uri = "";
    uri_after = "";

    int pos = file_name.find_first_of("/");

    if (pos != -1)
    {
        uri = file_name.substr(0, pos);
        uri_after = file_name.substr(pos + 1);
    }
    else
    {
        uri = file_name;
    }

    if (rest_uri.count(uri) <= 0)
    {
        handleError(fd, 404, "Not Found!");
        return ANALYSIS_ERROR;
    }

    switch (method)
    {
    case METHOD_GET:
        analysisGetRequest();
        break;
    case METHOD_POST:
        parseContent();
        analysisPostRequest();
        break;
    case METHOD_PUT:
        parseContent();
        analysisPutRequest();
        break;
    case METHOD_DELETE:
        analysisDeleteRequest();
        /*if(realFile.find("delete.json")!="-1")
		{
			return ANALYSIS_SUCCESS;
		}*/
        break;
    default:
        break;
    }

    //提取公共部分
    string header;
    header += "HTTP/1.1 200 OK\r\n";
    if (headers.find("Connection") != headers.end() && headers["Connection"] == "Keep-Alive")
    {
        keep_alive = true;
        header += string("Connection: Keep-Alive\r\n") + "Keep-Alive: timeout=" + to_string(5 * 60 * 1000) + "\r\n";
    }

    cout << "realFile after: " << realFile << endl;

    struct stat sbuf;
    if (stat(realFile.c_str(), &sbuf) < 0)
    {
        header.clear();
        handleError(fd, 404, "Not Found!");
        return ANALYSIS_ERROR;
    }
    header += "Content-type: application/json\r\n";
    header += "Content-Length: " + to_string(sbuf.st_size) + "\r\n";
    // 头部结束
    header += "\r\n";
    outBuffer += header;

    int src_fd = open(realFile.c_str(), O_RDONLY, 0);
    char *src_addr = static_cast<char *>(mmap(NULL, sbuf.st_size, PROT_READ, MAP_PRIVATE, src_fd, 0));
    close(src_fd);

    outBuffer += src_addr;
    munmap(src_addr, sbuf.st_size);

    return ANALYSIS_SUCCESS;
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
    header_buff += "Content-type: application/json\r\n";
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