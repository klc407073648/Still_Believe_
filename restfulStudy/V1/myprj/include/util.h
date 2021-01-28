#pragma once
#include <cstdlib>
#include <string>
#include <vector>
#include <map>

ssize_t readn(int fd, void *buff, size_t n);
ssize_t readn(int fd, std::string &inBuffer);
ssize_t writen(int fd, void *buff, size_t n);
ssize_t writen(int fd, std::string &sbuff);
void handle_for_sigpipe();
int setSocketNonBlocking(int fd);

/**
* @brief string工具类
*/
class StringUtil {
public:
    static std::vector<std::string> mySplit(const std::string& str, std::string sp_string);  // split(), str 是要分割的string
    static void parseParam(std::string paramsStr, std::map<std::string, std::string> &reqParams);
	static std::string replace_all(std::string& str, const std::string& old_value, const std::string& new_value);
};

/**
* @brief shell工具类
*/
class ShellUtil {
public:
    static std::string execShellPipe(std::string cmd);
};

