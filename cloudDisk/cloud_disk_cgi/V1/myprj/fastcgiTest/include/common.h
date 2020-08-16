/**
 * @file common.h
 * @brief 公共常量定义
 * @author klc
 * @date 2020-08-04
 * @copyright Copyright (c) 2020年 klc
 */

#ifndef __STIBEL_COMMON_H_
#define __STIBEL_COMMON_H_

#include <string>
#include <map>

using namespace std;

const string json_file = "./myprj/fastcgiTest/conf/conf.json";
const string HTML_PATH = "/usr/local/nginx/test/";
const string myLogPath = "./myprj/fastcgiTest/conf/log4cpp.conf";

typedef std::map<string , string> LoginInfoMap;

static LoginInfoMap mLoginInfoMap;


const int expire_time= 120;//redis的默认过期时间120s

#define USER_REG        "^[a-zA-Z_]{1}[a-zA-Z\\d_]{5,19}$"  //用户名长度6-20位，由数字，字母，下划线组成，且不能以数字开头，字母不区分大小写
#define PASSWD_REG      "(?=.*([a-zA-Z].*))(?=.*[0-9].*)[a-zA-Z0-9-*/+.~!@#$%^&*()]{8,20}$"    //密码长度8-20位，至少包含数字跟字母，可以有特殊字符

const string uploadPath = "./pic/";


#endif  //__STIBEL_COMMON_H_