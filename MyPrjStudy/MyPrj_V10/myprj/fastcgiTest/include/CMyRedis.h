/* Author:zhangxp
 * Date:2017-5-23
 * Version:1.0
 * Modify:
 * ����ʧ�ܲ������쳣��ʽ����,��ע�Ⲷ���쳣.
 ʹ��ʾ��:
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
 *
 *Welcome to use!
 */
#ifndef C_MY_REDIS_H
#define C_MY_REDIS_H
#include <string.h>
#include <hiredis/hiredis.h>
#include "CErrorMsg.h"
#include"LoggerManager.h"

using namespace minilog;

using std::string;

class CMyRedis
{
public:
	CMyRedis();
	~CMyRedis();
	void Connect(const string& strIp, int nPort);	
	bool CheckConenct();
	
	void redisTest();
	/*
	 *�ַ�������:Set Get Del
	 */
	void Set(const string& strKey, const string& strValue);
	//��ȡָ������ֵ,�����������,����"".
	string Get(const string& strKey);
	// ɾ��ָ������ֵ,�����ڷ���1,�������ڷ���0.
	int Del(const string& strKey);

	/*
	 *�б����� Push Pop Lindex Lrange
	 */
	// nFlag = 0Ĭ�ϴ��Ҷ�����,������0���������,�ɹ����ص�ǰ�б�Ԫ�ظ���.
	int Push(const string& strListKey, const string& strValue, int nFlag = 0);
	// nFlag = 0Ĭ�ϴ��Ҷ��Ƴ�,�����ڴ�����Ƴ�,�ɹ�����Ԫ�ص�ֵ,û��Ԫ�ط���"".
	string Pop(const string& strListKey, int nFlag = 0);
	// ����ָ����ŵĵ���Ԫ��,�ɹ�����Ԫ��ֵ,�����ڷ���"".
	string Lindex(const string& strListKey, int nIndex);
	// ����һ�����ص�Ԫ��,strArray���ص�Ԫ������,nCout��������Ĵ�С,������ص�Ԫ�ظ���.
	void Lrange(const string& strListKey, int nStart, int nEnd, string strArray[],
					int& nCount);
	
	/*
	 *���ϲ���
	 */
	// ��������һ��Ԫ�ص�������,�ɹ�����1,����0��ʾ�������Ѿ��������Ԫ��.
	int Sadd(const string& strSetKey, const string& strItem);
	// �Ӽ������Ƴ�Ԫ��,�����Ƴ�Ԫ�صĸ���.
	int Srem(const string& strSetKey, const string& strItem);
	// ���һ��Ԫ���Ƿ�����ڼ�����.
	bool SisMember(const string& strSetKey, const string& strItem);
	// ���ؼ���������Ԫ��,strArray��ŷ���Ԫ�ص�����,nCount��������Ĵ�С,
	// ������ص�Ԫ�ظ���.
	void Smembers(const string& strSetKey, string strArray[], int& nCount);

	/*
	 *ɢ�в���
	*/
	// ��������һ����ֵ�Ե�ɢ����,�ɹ�����1,0��ʾ�����ļ��Ѿ�����.value�Ḳ��ԭ��ֵ.
	int HSet(const string& strHashKey, const string& key, const string& value);
	// ����ָ���ļ���ֵ,�����ڷ���"".
	string HGet(const string& strHashKey,const string& key);
	// ɾ��ָ���ļ�,����1��ʾɾ���ɹ�,0��ʾָ������������ɢ����.
	int HDel(const string& strHashKey, const string& key);
	// ����ɢ�������������м�ֵ��.����nCount��ʾ��������ĸ���,���nCount��ʾʵ�ʷ��ص�
	// ����.
	void HGetAll(const string& strHashKey, string strArray[], int& nCount);

	/*
	 *���򼯺ϲ���
	*/
	// ���������򼯺�������Ԫ��,��������Ԫ�صĸ���.
	int ZAdd(const string& strZSetKey, int nScore, const string& value);
	// ����ŷ������򼯺ϵ�Ԫ��.
	void ZRange(const string& strZSetKey, int nStartIndex, int nEndIndex, 
					string strArray[], int& nCount);
	// ����ֵ�������򼯺ϵ�Ԫ��.
	void ZRangeByScore(const string& strZSetKey, int nStartScore,
								int nEndScore, string strArray[], int& nCount);
	// ���ر��Ƴ�Ԫ�ص�����.
	int ZRem(const string& strZSetKey, const string& value);
private:
    DECLARE_STATIC_LOGGERMANAGER(); 
	redisContext* m_pRedisContext;
	redisReply* m_pRedisReply;
	string m_strIp;
	int m_nPort;
};
#endif // C_MY_REDIS_H