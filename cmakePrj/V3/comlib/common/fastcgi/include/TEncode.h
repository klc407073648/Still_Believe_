/**
 * @file TEncode.h
 * @brief 数据加密解密处理
 * @author klc
 * @date 2020-08-09
 * @copyright Copyright (c) 2020年 klc
 */

#ifndef __STIBEL_TENCODE_H__
#define __STIBEL_TENCODE_H__

#include <cstdlib>
#include <string.h>
#include <string>

typedef struct
{
    unsigned int count[2];
    unsigned int state[4];
    unsigned char buffer[64];   
}MD5_CTX;
 
                         
#define F(x,y,z) ((x & y) | (~x & z))
#define G(x,y,z) ((x & z) | (y & ~z))
#define H(x,y,z) (x^y^z)
#define I(x,y,z) (y ^ (x | ~z))
#define ROTATE_LEFT(x,n) ((x << n) | (x >> (32-n)))
#define FF(a,b,c,d,x,s,ac) \
          { \
          a += F(b,c,d) + x + ac; \
          a = ROTATE_LEFT(a,s); \
          a += b; \
          }
#define GG(a,b,c,d,x,s,ac) \
          { \
          a += G(b,c,d) + x + ac; \
          a = ROTATE_LEFT(a,s); \
          a += b; \
          }
#define HH(a,b,c,d,x,s,ac) \
          { \
          a += H(b,c,d) + x + ac; \
          a = ROTATE_LEFT(a,s); \
          a += b; \
          }
#define II(a,b,c,d,x,s,ac) \
          { \
          a += I(b,c,d) + x + ac; \
          a = ROTATE_LEFT(a,s); \
          a += b; \
          }

class  MD5
{
public:
    static void MD5Init(MD5_CTX *context);
    static void MD5Update(MD5_CTX *context,unsigned char *input,unsigned int inputlen);
    static void MD5Final(MD5_CTX *context,unsigned char digest[16]);
    static void MD5Transform(unsigned int state[4],unsigned char block[64]);
    static void MD5Encode(unsigned char *output,unsigned int *input,unsigned int len);
    static void MD5Decode(unsigned int *output,unsigned char *input,unsigned int len);

private:
    /// HEX字符转换为ASC字符
    static char hexToAsc(const std::string &src);

};

class BASE64
{
public:
    //bindata编码成base64
    //bindata：  源字符串
    //binlength: 源字符串长度
    //base64：目的字符串，base64字符串
    //返回值：base64字符串
    static char * base64Encode( const unsigned char * bindata, int binlength, char * base64 );

    //解码base64
    //base64：源字符串
    //bindata: 目的字符串
    //返回值：目的字符串长度
    static int base64Decode( const char * base64, unsigned char * bindata );
};


class DesEncode
{
public:
    //加密小于4k的数据（encrypt：加密）
    //pInData：明文数据
    //nInDataLen：明文数据长度
    //pOutData：加密后的数据
    //pOutDataLen：加密数据的长度
    static int DesEnc(
            unsigned char *pInData,
            int            nInDataLen,
            unsigned char *pOutData,
            int           *pOutDataLen);

    //加密等于4k的数据
    static int DesEnc_raw(
        unsigned char *pInData,
        int            nInDataLen,
        unsigned char *pOutData,
        int           *pOutDataLen);

    //解密小于4k的数据(decrypt：解密)
    //pInData：密文数据
    //nInDataLen：密文数据长度
    //pOutData：解密后的数据
    //pOutDataLen：解密数据的长度
    static int DesDec(
        unsigned char *pInData,
        int            nInDataLen,
        unsigned char *pOutData,
        int           *pOutDataLen);

    //解密等于4k的数据
    static int DesDec_raw(
        unsigned char *pInData,
        int            nInDataLen,
        unsigned char *pOutData,
        int           *pOutDataLen);
};


#endif  //__STIBEL_TENCODE_H__

