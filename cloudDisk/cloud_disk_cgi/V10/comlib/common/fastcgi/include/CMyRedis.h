/**
 * @file CMyRedis.h
 * @brief redis封装类，底层是调用hiredis接口
 * @author klc
 * @date 2020-08-03
 * @copyright Copyright (c) 2020年 klc
 
* 所有失败操作以异常形式返回,请注意捕获异常.
 使用示例:
 	try{
 		CMyRedis myRedis;
 		myRedis.connect("127.0.0.1");
 		myRedis.set("hello", "world");
 		string str = myRedis.get("hello");
 		...
 	}catch(CErrorMsg& cErr)
 	{
		cout<<cErr.get_errorCode()<<cErr.get_errorMsg().c_str()<<endl;
 	}
 */

#ifndef __STIBEL_CMYREDIS_H__
#define __STIBEL_CMYREDIS_H__
#include <string.h>
#include <memory>
#include <hiredis/hiredis.h>
#include "CErrorMsg.h"
#include"LoggerManager.h"

using namespace minilog;

using std::string;

#define FILE_PUBLIC_ZSET              "FILE_PUBLIC_ZSET"
#define FILE_NAME_HASH                "FILE_NAME_HASH"


class CMyRedis
{
public:
	typedef std::shared_ptr<CMyRedis> ptr;
	CMyRedis();
	~CMyRedis();
	void Connect(const string& strIp, int nPort);	
	bool CheckConenct();
	
	static CMyRedis::ptr  getInstance(const string& ip="127.0.0.1" , int port=6379);
	
	void redisTest();
	/*
	 *字符串操作:Set Get Del
	 */
	void Set(const string& strKey, const string& strValue);
	void Setex(const string & strKey,int strSec,const string &strValue);
	//获取指定键的值,如果键不存在,返回"".
	string Get(const string& strKey);
	// 删除指定键的值,键存在返回1,键不存在返回0.
	int Del(const string& strKey);

	/*
	 *列表操作 Push Pop Lindex Lrange
	 */
	// nFlag = 0默认从右端推入,不等于0从左端推入,成功返回当前列表元素个数.
	int Push(const string& strListKey, const string& strValue, int nFlag = 0);
	// nFlag = 0默认从右端推出,不等于从左端推出,成功返回元素的值,没有元素返回"".
	string Pop(const string& strListKey, int nFlag = 0);
	// 返回指定序号的单个元素,成功返回元素值,不存在返回"".
	string Lindex(const string& strListKey, int nIndex);
	// 返回一定返回的元素,strArray返回的元素数组,nCout输入数组的大小,输出返回的元素个数.
	void Lrange(const string& strListKey, int nStart, int nEnd, string strArray[],
					int& nCount);
	
	/*
	 *集合操作
	 */
	// 尝试添加一个元素到集合中,成功返回1,返回0表示集合中已经存在这个元素.
	int Sadd(const string& strSetKey, const string& strItem);
	// 从集合中移除元素,返回移除元素的个数.
	int Srem(const string& strSetKey, const string& strItem);
	// 检查一个元素是否存在于集合中.
	bool SisMember(const string& strSetKey, const string& strItem);
	// 返回集合中所有元素,strArray存放返回元素的数组,nCount输入数组的大小,
	// 输出返回的元素个数.
	void Smembers(const string& strSetKey, string strArray[], int& nCount);

	/*
	 *散列操作
	*/
	// 尝试添加一个键值对到散列中,成功返回1,0表示给定的键已经存在.value会覆盖原有值.
	int HSet(const string& strHashKey, const string& key, const string& value);
	// 返回指定的键的值,不存在返回"".
	string HGet(const string& strHashKey,const string& key);
	// 删除指定的键,返回1表示删除成功,0表示指定键不存在于散列中.
	int HDel(const string& strHashKey, const string& key);
	// 返回散列所包含的所有键值对.输入nCount表示接收数组的个数,输出nCount表示实际返回的
	// 个数.
	void HGetAll(const string& strHashKey, string strArray[], int& nCount);

	/*
	 *有序集合操作
	*/
	// 尝试向有序集合中添加元素,返回添加元素的个数.
	int ZAdd(const string& strZSetKey, int nScore, const string& value);
	// 按序号返回有序集合的元素.
	void ZRange(const string& strZSetKey, int nStartIndex, int nEndIndex, 
					string strArray[], int& nCount);
	// 按分值返回有序集合的元素.
	void ZRangeByScore(const string& strZSetKey, int nStartScore,
								int nEndScore, string strArray[], int& nCount);

	int ZScore(const string& strZSetKey, const string& value);
	// 返回被移除元素的数量.
	int ZRem(const string& strZSetKey, const string& value);

	
	int ZLexCount(const string& strZSetKey, const string& value);

	int ZIncre(const string& strZSetKey, const string& value);

	int Zcard(const string& strZSetKey);
private:
    DECLARE_STATIC_LOGGERMANAGER(); 
	static CMyRedis::ptr m_MyRedis;
	redisContext* m_pRedisContext;
	redisReply* m_pRedisReply;
	string m_strIp;
	int m_nPort;
};
#endif // __STIBEL_CMYREDIS_H__