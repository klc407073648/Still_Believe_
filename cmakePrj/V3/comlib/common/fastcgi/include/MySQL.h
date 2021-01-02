/**
 * @file mysql.h
 * @brief MYSQL数据库相关类
 * @author klc
 * @date 2020-04-30
 * @copyright Copyright (c) 2020年 klc
 */

#ifndef __STIBEL_MYSQL_H_
#define __STIBEL_MYSQL_H_

#include <mysql/mysql.h>
#include <memory>
#include <functional>
#include <map>
#include <vector>
#include <list>
#include "MutexLock.h"
#include "DataBase.h"
#include "Util.h"
#include "Singleton.h"

class MySQL;

/**
* @brief MYSQL时间相关处理函数
*/
struct MySQLTime {
    MySQLTime(time_t t)
        :ts(t) { }
    time_t ts;
};

bool mysql_time_to_time_t(const MYSQL_TIME& mt, time_t& ts);
bool time_t_to_mysql_time(const time_t& ts, MYSQL_TIME& mt);


/**
* @brief MYSQL查询结果集MYSQL_RES封装
*/
class MySQLRes : public ISQLData {
public:
    typedef std::shared_ptr<MySQLRes> ptr;

    //定义回调函数类型data_cb
    typedef std::function<bool(MYSQL_ROW row
                ,int field_count, int row_no)> data_cb;
    
    //构造函数
    MySQLRes(MYSQL_RES* res, int eno, const char* estr);

    //获取mysql的结果集
    MYSQL_RES* get() const { return m_data.get();}

    //获取mysql执行的错误码,后接const，表示只读函数，不改变数据成员的值
    int getErrno() const { return m_errno;}

    //获取mysql执行的错误具体信息
    const std::string& getErrStr() const { return m_errstr;}

    bool foreach(data_cb cb);

    //回显全部结果集
    void showAllRes() override;
	
	//回显全部结果集
    MYSQL_RES* getAllRes() { return m_data.get();}

    //回显表所有字段。  给定字段编号，返回表字段的类型。mysql_fetch_field_direct
    void showTableContent() override;

    MYSQL_ROW getRowRes() override;

    //获取查询结果集的行数目
    int getDataCount() override;

    //获取查询结果集的列数目
    int getColumnCount() override;

    //获取对应列所包含的字节数,配合next函数使用
    int getColumnBytes(int idx) override;

    //获取对应列的类型
    int getColumnType(int idx) override;

    //获取对应列的名称
    std::string getColumnName(int idx) override;

    //获取当前行的，指定列信息，判断是否为空
    bool isNull(int idx) override;

    //以下函数统一调用getInt64(int idx)，通过工具函数将对应MYSQL_ROW  m_cur[idx]
    //中的string内容转换成int64_t(TypeUtil::Atoi(m_cur[idx]))
    int8_t getInt8(int idx) override;
    uint8_t getUint8(int idx) override;
    int16_t getInt16(int idx) override;
    uint16_t getUint16(int idx) override;
    int32_t getInt32(int idx) override;
    uint32_t getUint32(int idx) override;
    int64_t getInt64(int idx) override;
    uint64_t getUint64(int idx) override;

    //将string 内容转换成浮点型 double
    float getFloat(int idx) override;
    double getDouble(int idx) override;

    //对应当前行m_cur，idx列的具体内容
    std::string getString(int idx) override;

    //Blob类型的当前行m_cur，idx列的具体内容
    std::string getBlob(int idx) override;

    //调用Str2Time，将按照format格式的string时间转换成time_t格式
    time_t getTime(int idx) override;

    //获取下一行的数据
    bool next() override;

private:
    int m_errno;
    std::string m_errstr;
    MYSQL_ROW m_cur;
    unsigned long* m_curLength;
    std::shared_ptr<MYSQL_RES> m_data;
};

class MySQLManager;

/**
* @brief MYSQL类
*/
class MySQL : public IDB
              ,public std::enable_shared_from_this<MySQL> {
friend class MySQLManager;
public:
    typedef std::shared_ptr<MySQL> ptr;

    MySQL(const std::map<std::string, std::string>& args);

    //连接数据库，初始化m_mysql
    bool connect();

    static MySQL::ptr getInstance(const std::map<std::string, std::string>& args );

    //检测服务器是否关闭
    bool ping();

    //按照format格式，解析可变参数，最终得到可执行的sql语句
    virtual int execute(const char* format, ...) override;
    int execute(const char* format, va_list ap);
    virtual int execute(const std::string& sql) override;

    int64_t getLastInsertId() override;

    std::shared_ptr<MySQL> getMySQL();

    //获取原来的智能指针
    std::shared_ptr<MYSQL> getRaw();

    //返回前一次 MySQL 操作所实际影响的记录行数
    uint64_t getAffectedRows();
    
    //按照format格式，解析可变参数得到可执行的sql语句
    //调用my_mysql_query函数生成结果集(MYSQL_RES* res),返回基类指针ISQLData::ptr
    //class MySQLRes : public ISQLData，通过基类指针指向派生类，从而实现多态
    ISQLData::ptr query(const char* format, ...) override;
    ISQLData::ptr query(const char* format, va_list ap); 
    ISQLData::ptr query(const std::string& sql) override;

    ITransaction::ptr openTransaction(bool auto_commit) override;
    
    //记录最近一次执行的sql语句
    const char* cmd();

    //更换当前使用的数据库
    bool use(const std::string& dbname);

    //获取当前指向的数据库名称
    std::string getDbname() const;

    //获取错误码
    int getErrno() override;

    //获取错误码具体内容描述
    std::string getErrStr() override;

    //函数返回上一步 INSERT 操作产生的 ID
    uint64_t getInsertId();

    bool isError(){return m_hasError;}
private:
    bool isNeedCheck();
private:
    std::map<std::string, std::string> m_params;
    std::shared_ptr<MYSQL> m_mysql;

    std::string m_cmd;
    std::string m_dbname;

    uint64_t m_lastUsedTime;
    bool m_hasError;
    int32_t m_poolSize;
    static MySQL::ptr m_MySQL;
};

/**
* @brief MYSQL事务类
*/
class MySQLTransaction : public ITransaction {
public:
    typedef std::shared_ptr<MySQLTransaction> ptr;

    //创建MYSQL事务
    static MySQLTransaction::ptr Create(MySQL::ptr mysql, bool auto_commit);

    //析构函数作了保护，如果是自动提交，则提交修改，否则回滚
    ~MySQLTransaction();

    //MYSQL 事务处理主要有两种方法
    /*
    1、用 BEGIN, ROLLBACK, COMMIT来实现
    BEGIN 开始一个事务
    insert/update/delete 操作
    ROLLBACK 事务回滚(清空之前的操作)
    insert/update/delete 操作
    COMMIT 事务确认
    
    2、直接用 SET 来改变 MySQL 的自动提交模式:
    SET AUTOCOMMIT=0 禁止自动提交
    SET AUTOCOMMIT=1 开启自动提交
    */
    bool begin() override;
    bool commit() override;
    bool rollback() override;

    //事务的执行过程，调用m_mysql的execute函数
    virtual int execute(const char* format, ...) override;
    int execute(const char* format, va_list ap);
    virtual int execute(const std::string& sql) override;

    int64_t getLastInsertId() override;

    //获取MYSQL智能指针
    std::shared_ptr<MySQL> getMySQL();

    //返回自动提交的标志位
    bool isAutoCommit() const { return m_autoCommit;}

    //返回执行结束的标志位
    bool isFinished() const { return m_isFinished;}

    //返回是否存在错误的标志位
    bool isError() const { return m_hasError;}
private:
    //构造函数私有化，类似单例模式
    MySQLTransaction(MySQL::ptr mysql, bool auto_commit);
private:
    MySQL::ptr m_mysql;
    bool m_autoCommit;
    bool m_isFinished;
    bool m_hasError;
};
//pedef Singleton<MySQLManager> MySQLMgr;
typedef SingletonPtr<MySQLManager> MySQLMgr;

/**
* @brief MYSQL管理类
*/
class MySQLManager {
public:

    //设置最大连接数，mysql_library_init() 初始化MySQL C API库
    MySQLManager();

    //mysql_library_end() 最终确定MySQL C API库 ,释放对应的连接
    ~MySQLManager();

    //根据名字获取对应的MYSQL智能指针
    MySQL::ptr get(const std::string& name);

    //注册MYSQL到对应map  (m_dbDefines)
    void registerMySQL(const std::string& name, const std::map<std::string, std::string>& params);

    //检测链接状态，超过sec秒的连接默认删除
    void checkConnection(int sec = 30);

    //获取最大连接数
    uint32_t getMaxConn() const { return m_maxConn;}

    //设置最大连接数
    void setMaxConn(uint32_t v) { m_maxConn = v;}

    //MySQLManager的execute函数，底层调用m_mysql的execute函数
    int execute(const std::string& name, const char* format, ...);
    int execute(const std::string& name, const char* format, va_list ap);
    int execute(const std::string& name, const std::string& sql);

    //MySQLManager的query函数，底层调用m_mysql的query函数
    ISQLData::ptr query(const std::string& name, const char* format, ...);
    ISQLData::ptr query(const std::string& name, const char* format, va_list ap); 
    ISQLData::ptr query(const std::string& name, const std::string& sql);

    //开启事务
    MySQLTransaction::ptr openTransaction(const std::string& name, bool auto_commit);
private:
    void freeMySQL(const std::string& name, MySQL* m);
private:
    uint32_t m_maxConn;
    MutexLock lock;
    std::map<std::string, std::list<MySQL*> > m_conns;
    std::map<std::string, std::map<std::string, std::string> > m_dbDefines;
};

/**
* @brief MYSQL工具类
*/
class MySQLUtil {
public:
    static ISQLData::ptr Query(const std::string& name, const char* format, ...);
    static ISQLData::ptr Query(const std::string& name, const char* format,va_list ap); 
    static ISQLData::ptr Query(const std::string& name, const std::string& sql);

    static ISQLData::ptr TryQuery(const std::string& name, uint32_t count, const char* format, ...);
    static ISQLData::ptr TryQuery(const std::string& name, uint32_t count, const std::string& sql);

    static int Execute(const std::string& name, const char* format, ...);
    static int Execute(const std::string& name, const char* format, va_list ap); 
    static int Execute(const std::string& name, const std::string& sql);

    static int TryExecute(const std::string& name, uint32_t count, const char* format, ...);
    static int TryExecute(const std::string& name, uint32_t count, const char* format, va_list ap); 
    static int TryExecute(const std::string& name, uint32_t count, const std::string& sql);
};

#endif  //__STIBEL_MYSQL_H_
