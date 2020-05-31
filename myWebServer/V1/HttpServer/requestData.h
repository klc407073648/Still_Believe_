#pragma once
#include "timer.h"
#include <string>
#include <unordered_map>
#include <memory>
#include <sys/epoll.h>
#include <cstring>


/* #include <opencv/cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>
using namespace cv; */

const int MAX_BUFF = 4096;

// 有请求出现但是读不到数据,可能是Request Aborted,
// 或者来自网络的数据没有达到等原因,
// 对这样的请求尝试超过一定的次数就抛弃
const int AGAIN_MAX_TIMES = 200;

enum ProcessState {
  STATE_PARSE_URI = 1,
  STATE_PARSE_HEADERS,
  STATE_RECV_BODY,
  STATE_ANALYSIS,
  STATE_FINISH
};

enum URIState {
  PARSE_URI_AGAIN = 1,
  PARSE_URI_ERROR,
  PARSE_URI_SUCCESS,
};

enum HeaderState {
  PARSE_HEADER_SUCCESS = 1,
  PARSE_HEADER_AGAIN,
  PARSE_HEADER_ERROR
};

enum ParseState {
  H_START = 0,
  H_KEY,
  H_COLON,
  H_SPACES_AFTER_COLON,
  H_VALUE,
  H_CR,
  H_LF,
  H_END_CR,
  H_END_LF
};

enum AnalysisState { ANALYSIS_SUCCESS = 1, ANALYSIS_ERROR };

enum HttpMethod { METHOD_POST = 1, METHOD_GET, METHOD_HEAD };

enum HttpVersion { HTTP_10 = 1, HTTP_11 };

const int EPOLL_WAIT_TIME = 500;

class MimeType
{
private:
    static void init();
    static std::unordered_map<std::string, std::string> mime;
    MimeType();
    MimeType(const MimeType &m);

public:
    static std::string getMime(const std::string &suffix);

private:
    static pthread_once_t once_control;
};


class TimerNode;

class RequestData : public std::enable_shared_from_this<RequestData>
{
private:
    std::string path;
    int fd;
    int epollfd;

    std::string inBuffer;
    std::string outBuffer;
    __uint32_t events;
    bool error;

    HttpMethod method;
    HttpVersion httpVersion;
    std::string file_name;
    std::string realFile;
    int now_read_pos;
    ProcessState state;
    ParseState h_state;
    bool isfinish;
    bool keep_alive;
    std::unordered_map<std::string, std::string> headers;
    std::weak_ptr<TimerNode> timer;

    bool isAbleRead;
    bool isAbleWrite;
	static std::string user;
	static std::string password;

private:
    URIState parseURI();
    HeaderState parseHeaders();
    AnalysisState analysisRequest();

    std::string execShellPipe(std::string cmd);
    AnalysisState analysisPostNumberRequest(char number);
    AnalysisState analysisPostCGIRequest(int flag);

    /* Mat stitch(Mat &src)
    {
        return src;
    } */

public:

    RequestData();
    RequestData(int _epollfd, int _fd, std::string _path);
    ~RequestData();
    void linkTimer(std::shared_ptr<TimerNode> mtimer);
    void reset();
    void seperateTimer();
    int getFd();
    void setFd(int _fd);
	std::string getPath();
    void setPath(std::string _path);
    void handleRead();
    void handleWrite();
    void handleError(int fd, int err_num, std::string short_msg);
    void handleConn();

    void disableReadAndWrite();

    void enableRead();

    void enableWrite();

    bool canRead();

    bool canWrite();
};

