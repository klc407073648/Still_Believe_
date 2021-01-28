#pragma once
#include "timer.h"
#include <string>
#include <unordered_map>
#include <memory>
#include <sys/epoll.h>
#include <cstring>
#include <set>
#include <map>
#include "JsonUtil.h"

const int MAX_BUFF = 4096;
const int AGAIN_MAX_TIMES = 200;
const int EPOLL_WAIT_TIME = 500;
//const std::string rootPath = "./webContent/";

enum ProcessState
{
  STATE_PARSE_URI = 1,
  STATE_PARSE_HEADERS,
  STATE_RECV_BODY,
  STATE_ANALYSIS,
  STATE_FINISH
};

enum URIState
{
  PARSE_URI_AGAIN = 1,
  PARSE_URI_ERROR,
  PARSE_URI_SUCCESS,
};

enum HeaderState
{
  PARSE_HEADER_SUCCESS = 1,
  PARSE_HEADER_AGAIN,
  PARSE_HEADER_ERROR
};

enum ParseState
{
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

enum AnalysisState
{
  ANALYSIS_SUCCESS = 1,
  ANALYSIS_ERROR
};

enum HttpMethod
{
  METHOD_GET = 1,
  METHOD_POST ,
  METHOD_PUT,
  METHOD_DELETE,
  METHOD_HEAD
};

enum HttpVersion
{
  HTTP_10 = 1,
  HTTP_11
};

class TimerNode;

class RequestData : public std::enable_shared_from_this<RequestData>
{

public:
  RequestData();
  RequestData(int _epollfd, int _fd, std::string _path);
  ~RequestData();

  void linkTimer(std::shared_ptr<TimerNode> mtimer);
  void reset();
  void seperateTimer();

  //第一个const修饰的是返回值，表示返回的是常整型;第二个const该函数为只读函数，不允许修改其中的数据成员的值。
  const int getFd() const { return fd; }
  void setFd(int _fd) { fd = _fd; }
  const std::string& getPath() const{ return path; }
  void setPath(const std::string& _path) { path = _path; }

  void handleRead();
  void handleWrite();
  void handleError(int fd, int err_num, std::string short_msg);
  void handleConn();

  void enableRead();
  void enableWrite();
  bool canRead();
  bool canWrite();

  void disableReadAndWrite();

private:
  URIState parseURI();
  HeaderState parseHeaders();
  AnalysisState analysisRequest();

  void parseInitJson();
  void parseContent();
  
  void analysisGetRequest();
  void analysisPostRequest();
  void analysisPutRequest();
  void analysisDeleteRequest();

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
  //std::set<std::string> rest_uri;
  static std::map<std::string, Json::Value> rest_uri;

  std::string uri;
  std::string uri_after;
  std::map<std::string, std::string> m_paramsMap;

  bool isAbleRead;
  bool isAbleWrite;
};
