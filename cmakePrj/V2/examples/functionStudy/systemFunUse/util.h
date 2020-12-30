/**
 * @file util.h
 * @brief 常用工具类
 * @author klc
 * @date 2020-04-30
 * @copyright Copyright (c) 2020年 klc
 */

#ifndef __STIBEL_UTIL_H_
#define __STIBEL_UTIL_H_

#include <cxxabi.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <stdint.h>
#include <vector>
#include <string>
#include <iomanip>
#include<stdlib.h>
#include<stdio.h>


#include <iostream>
//#include <boost/lexical_cast.hpp>

/**
* @brief Time与Str相关转换
*/
std::string Time2Str(time_t ts = time(0), const std::string& format = "%Y-%m-%d %H:%M:%S");
time_t Str2Time(const char* str, const char* format = "%Y-%m-%d %H:%M:%S");

template<class V, class Map, class K>
V GetParamValue(const Map& m, const K& k, const V& def = V()) {
    auto it = m.find(k);
    if(it == m.end()) {
        return def;
    }
    try {
        return static_cast<V>(it->second);
    } catch (...) {
    }
    return def;
}
template<class T>
void nop(T*) {}
/* template<class V, class Map, class K>
V GetParamValue(const Map& m, const K& k, const V& def = V()) {
    auto it = m.find(k);
    if(it == m.end()) {
        return def;
    }
    try {
        return static_cast<V>(it->second);
    } catch (...) {
    }
    return def;
}

template<class V, class Map, class K>
bool CheckGetParamValue(const Map& m, const K& k, V& v) {
    auto it = m.find(k);
    if(it == m.end()) {
        return false;
    }
    try {
        v = static_cast<V>(it->second);
        return true;
    } catch (...) {
    }
    return false;
} */

/**
* @brief 类型转换类
*/
class TypeUtil {
public:
    static int8_t ToChar(const std::string& str);
    static int64_t Atoi(const std::string& str);
    static double Atof(const std::string& str);
    static int8_t ToChar(const char* str);
    static int64_t Atoi(const char* str);
    static double Atof(const char* str);
};


/**
* @brief string工具类
*/
class StringUtil {
public:
    static std::string Format(const char* fmt, ...);
    static std::string Formatv(const char* fmt, va_list ap);

    static std::string UrlEncode(const std::string& str, bool space_as_plus = true);
    static std::string UrlDecode(const std::string& str, bool space_as_plus = true);

    static std::string Trim(const std::string& str, const std::string& delimit = " \t\r\n");
    static std::string TrimLeft(const std::string& str, const std::string& delimit = " \t\r\n");
    static std::string TrimRight(const std::string& str, const std::string& delimit = " \t\r\n");


    static std::string WStringToString(const std::wstring& ws);
    static std::wstring StringToWString(const std::string& s);

};

/**
* @brief string工具类
*/
class ShellUtil {
public:
    static void execShell(std::string cmd);
    static std::string execShellPipe(std::string cmd);
	static std::string getParamValue(std::string key , std::string fileName);
	static void modParamValue(std::string key , std::string value, std::string fileName);
};

#endif //__STIBEL_UTIL_H_
