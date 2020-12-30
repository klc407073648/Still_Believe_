/**
 * @file CErrorMsg.h
 * @brief redis���쳣�����װ
 *        �����쳣�Ļ���,����ͨ���̳���ɸ��������쳣��ʵ��
 * @author klc
 * @date 2020-08-03
 * @copyright Copyright (c) 2020�� klc
 */

#ifndef __STIBEL_CERRORMSG_H__
#define __STIBEL_CERRORMSG_H__

#include <string>
using namespace std;

class CErrorMsg
{
public:
	CErrorMsg(string strErrorMsg = "", int nErrorCode = 0, string strHint=""):m_strErrorMsg(strErrorMsg), m_nErrorCode(nErrorCode), m_strHint(strHint)
	{
	}
	virtual ~CErrorMsg(){}
	int get_errorCode() const{return m_nErrorCode;}
	string get_errorMsg() const{return m_strErrorMsg;}
	string get_errorHint() const{return m_strHint;}
	void set_errorCode(int nErrorCode){m_nErrorCode = nErrorCode;}
	void set_errorMsg(string strMsg){m_strErrorMsg = strMsg;}
	void set_errorHint(string strHint){m_strHint = strHint;}
protected:
private:
	string m_strErrorMsg;
	int m_nErrorCode;
	string m_strHint;
};
#endif // __STIBEL_CERRORMSG_H__