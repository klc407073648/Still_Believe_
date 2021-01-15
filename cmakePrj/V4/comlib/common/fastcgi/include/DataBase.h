/**
 * @file database.h
 * @brief 数据库基类封装
 * @author klc
 * @date 2020-04-30
 * @copyright Copyright (c) 2020年 klc
 */

#ifndef __STIBEL_DATABASE_H__
#define __STIBEL_DATABASE_H__

#include <memory>
#include <string>


/**
* @brief SQL数据基类，包含常用方法函数接口定义
*/
class ISQLData {
public:
    typedef std::shared_ptr<ISQLData> ptr;
    virtual ~ISQLData() {}

    virtual int getErrno() const = 0;
    virtual const std::string& getErrStr() const = 0;

    virtual void showAllRes() = 0;
	virtual MYSQL_RES* getAllRes() = 0;
    virtual MYSQL_ROW getRowRes()  = 0;
    virtual void showTableContent() = 0;
    virtual int getDataCount() = 0;
    virtual int getColumnCount() = 0;
    virtual int getColumnBytes(int idx) = 0;
    virtual int getColumnType(int idx) = 0;
    virtual std::string getColumnName(int idx) = 0;

    virtual bool isNull(int idx) = 0;
    virtual int8_t getInt8(int idx) = 0;
    virtual uint8_t getUint8(int idx) = 0;
    virtual int16_t getInt16(int idx) = 0;
    virtual uint16_t getUint16(int idx) = 0;
    virtual int32_t getInt32(int idx) = 0;
    virtual uint32_t getUint32(int idx) = 0;
    virtual int64_t getInt64(int idx) = 0;
    virtual uint64_t getUint64(int idx) = 0;
    virtual float getFloat(int idx) = 0;
    virtual double getDouble(int idx) = 0;
    virtual std::string getString(int idx) = 0;
    virtual std::string getBlob(int idx) = 0;
    virtual time_t getTime(int idx) = 0;
    virtual bool next() = 0;
};

/**
* @brief 数据库更新基类
*/
class ISQLUpdate {
public:
    virtual ~ISQLUpdate() {}
    virtual int execute(const char* format, ...) = 0;
    virtual int execute(const std::string& sql) = 0;
    virtual int64_t getLastInsertId() = 0;
};

/**
* @brief 数据库查询基类
*/
class ISQLQuery {
public:
    virtual ~ISQLQuery() {}
    virtual ISQLData::ptr query(const char* format, ...) = 0;
    virtual ISQLData::ptr query(const std::string& sql) = 0;
};


/**
* @brief 数据库处理基类
*/
class ITransaction : public ISQLUpdate {
public:
    typedef std::shared_ptr<ITransaction> ptr;
    virtual ~ITransaction() {};
    virtual bool begin() = 0;
    virtual bool commit() = 0;
    virtual bool rollback() = 0;
};

/**
* @brief 数据库基类
*/
class IDB : public ISQLUpdate
            ,public ISQLQuery {
public:
    typedef std::shared_ptr<IDB> ptr;
    virtual ~IDB() {}

    //virtual IStmt::ptr prepare(const std::string& stmt) = 0;
    virtual int getErrno() = 0;
    virtual std::string getErrStr() = 0;
    virtual ITransaction::ptr openTransaction(bool auto_commit = false) = 0;
};

#endif //__STIBEL_DATABASE_H__
