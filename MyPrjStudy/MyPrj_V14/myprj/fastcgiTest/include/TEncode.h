/**
 * @file TEncode.h
 * @brief 数据加密解密处理
 * @author klc
 * @date 2020-08-09
 * @copyright Copyright (c) 2020年 klc
 */

#ifndef __STIBEL_TENCODE_H__
#define __STIBEL_TENCODE_H__

#include <string>

namespace EncodeFun {

class  TEncode
{
public:
    static std::string uriEncode(const std::string &source);
    static std::string uriDecode(const std::string &source);

    static std::string base64Encode(const std::string &source);
    static std::string base64Decode(const std::string &source);

    static std::string md5Decode(const std::string &source);

private:
    /// HEX字符转换为ASC字符
    static char hexToAsc(const std::string &src);

};

}//namespace EncodeFun

#endif  //__STIBEL_TENCODE_H__

