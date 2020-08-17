#include "MySQL.h"
#include "Util.h"
#include <time.h>
#include"common.h"
#include "LoginCgi.h"

MySQL::ptr MySQL::m_MySQL= NULL;
/*
localtime_r,用于在linux下获取系统时间，将time_t转换成本地时间（年月日）
mktime，以本地时间的年、月、日等作为参数，将其变换成time_t值。
*/
bool mysql_time_to_time_t(const MYSQL_TIME& mt, time_t& ts) {
    struct tm tm;
    ts = 0;
    localtime_r(&ts, &tm);
    tm.tm_year = mt.year - 1900;
    tm.tm_mon = mt.month - 1;
    tm.tm_mday = mt.day;
    tm.tm_hour = mt.hour;
    tm.tm_min = mt.minute;
    tm.tm_sec = mt.second;
    ts = mktime(&tm);
    if(ts < 0) {
        ts = 0;
    }
    return true;
}

bool time_t_to_mysql_time(const time_t& ts, MYSQL_TIME& mt) {
    struct tm tm;
    localtime_r(&ts, &tm);
    mt.year = tm.tm_year + 1900;
    mt.month = tm.tm_mon + 1;
    mt.day = tm.tm_mday;
    mt.hour = tm.tm_hour;
    mt.minute = tm.tm_min;
    mt.second = tm.tm_sec;
    return true;
}

namespace {
    struct MySQLThreadIniter {
        MySQLThreadIniter() {
            mysql_thread_init();
        }

        ~MySQLThreadIniter() {
            mysql_thread_end();
        }
    };
}

/*
static thread_local 线程本地数据

*/
static MYSQL* mysql_init(std::map<std::string, std::string>& params,
                         const int& timeout) {

    static thread_local MySQLThreadIniter s_thread_initer;

    MYSQL* mysql = ::mysql_init(nullptr);
    if(mysql == nullptr) {
        cout << "mysql_init error" << endl;
        return nullptr;
    }

    if(timeout > 0) {
        mysql_options(mysql, MYSQL_OPT_CONNECT_TIMEOUT, &timeout);
    }
    bool close = false;
    mysql_options(mysql, MYSQL_OPT_RECONNECT, &close);
    mysql_options(mysql, MYSQL_SET_CHARSET_NAME, "utf8mb4");

    //int port = GetParamValue(params, "port", 0);
    int port =0;
    std::string host = GetParamValue<std::string>(params, "host");
    std::string user = GetParamValue<std::string>(params, "user");
    std::string passwd = GetParamValue<std::string>(params, "passwd");
    std::string dbname = GetParamValue<std::string>(params, "dbname"); 
	/*
    int port = 0;
    std::string host = "localhost";
	std::string user = "root";
	std::string passwd = "klczxas789";
	std::string dbname = "student";
    */
    if(mysql_real_connect(mysql, host.c_str(), user.c_str(), passwd.c_str()
                          ,dbname.c_str(), port, NULL, 0) == nullptr) {
        cout << "mysql_real_connect(" << host
                                  << ", " << port << ", " << dbname
                                  << ") error: " << mysql_error(mysql) <<endl;
        mysql_close(mysql);
        return nullptr;
    }
    return mysql;
}

MySQL::MySQL(const std::map<std::string, std::string>& args)
    :m_params(args)
    ,m_lastUsedTime(0)
    ,m_hasError(false)
    ,m_poolSize(10) {
}

bool MySQL::connect() {
    if(m_mysql && !m_hasError) {
        return true;
    }

    MYSQL* m = mysql_init(m_params, 0);
    if(!m) {
        m_hasError = true;
        return false;
    }
    m_hasError = false;
    //m_poolSize = sylar::GetParamValue(m_params, "pool", 5);
	m_poolSize=5;
    //将创建的MYSQL* m，所有权移交给m_mysql，并调用mysql_close释放原来的m
    m_mysql.reset(m, mysql_close);
    return true;
}

MySQL::ptr MySQL::getInstance() {
    if(m_MySQL ==NULL)
	{
        std::map<std::string, std::string> params;
            //数据库的参数
           /*   params["host"] = "localhost";
            params["user"] = "root";
            params["passwd"] = "klczxas789";
            params["dbname"] = "cloud_disk"; */
		
		map<string,string> mConfigMap =LoginCgi::getConfigMap();
			
        params["host"] = mConfigMap["mysql_host"];
        params["user"] = mConfigMap["mysql_user"];
        params["passwd"] = mConfigMap["mysql_passwd"];
        params["dbname"] = mConfigMap["mysql_dbname"]; 
            
        MySQL::ptr mysql(new MySQL(params));	
        m_MySQL =mysql;
    }
    return m_MySQL;
}


ITransaction::ptr MySQL::openTransaction(bool auto_commit) {
    return MySQLTransaction::Create(shared_from_this(), auto_commit);
}

int64_t MySQL::getLastInsertId() {
    return mysql_insert_id(m_mysql.get());
}

bool MySQL::isNeedCheck() {
    if((time(0) - m_lastUsedTime) < 5
            && !m_hasError) {
        return false;
    }
    return true;
}

bool MySQL::ping() {
    if(!m_mysql) {
        return false;
    }
    if(mysql_ping(m_mysql.get())) {
        m_hasError = true;
        return false;
    }
    m_hasError = false;
    return true;
}

int MySQL::execute(const char* format, ...) {
    va_list ap;
    va_start(ap, format);
    int rt = execute(format, ap);
    va_end(ap);
    return rt;
}

int MySQL::execute(const char* format, va_list ap) {
    m_cmd = StringUtil::Formatv(format, ap);
    int r = ::mysql_query(m_mysql.get(), m_cmd.c_str());
    if(r) {
        cout << "cmd=" << cmd()
            << ", error: " << getErrStr()<<endl;
        m_hasError = true;
    } else {
        m_hasError = false;
    }
    return r;
}

int MySQL::execute(const std::string& sql) {
    m_cmd = sql;
    int r = ::mysql_query(m_mysql.get(), m_cmd.c_str());
    if(r) {
        cout << "cmd=" << cmd()
            << ", error: " << getErrStr()<<endl;
        m_hasError = true;
    } else {
        m_hasError = false;
    }
    return r;
}

std::shared_ptr<MySQL> MySQL::getMySQL() {
    return MySQL::ptr(this, nop<MySQL>);
}

std::shared_ptr<MYSQL> MySQL::getRaw() {
    return m_mysql;
}

uint64_t MySQL::getAffectedRows() {
    if(!m_mysql) {
        return 0;
    }
    return mysql_affected_rows(m_mysql.get());
}

static MYSQL_RES* my_mysql_query(MYSQL* mysql, const char* sql) {
    if(mysql == nullptr) {
        cout << "mysql_query mysql is null";
        return nullptr;
    }

    if(sql == nullptr) {
        cout << "mysql_query sql is null";
        return nullptr;
    }

    if(::mysql_query(mysql, sql)) {
        cout << "mysql_query(" << sql << ") error:"
            << mysql_error(mysql);
        return nullptr;
    }

    MYSQL_RES* res = mysql_store_result(mysql);
    if(res == nullptr) {
        cout << "mysql_store_result() error:"
            << mysql_error(mysql);
    }
    return res;
}




MySQLRes::MySQLRes(MYSQL_RES* res, int eno, const char* estr)
    :m_errno(eno)
    ,m_errstr(estr)
    ,m_cur(nullptr)
    ,m_curLength(nullptr) {
    if(res) {
        m_data.reset(res, mysql_free_result);
    }
}

bool MySQLRes::foreach(data_cb cb) {
    MYSQL_ROW row;
    uint64_t fields = getColumnCount();
    int i = 0;
    while((row = mysql_fetch_row(m_data.get()))) {
        if(!cb(row, fields, i++)) {
            break;
        }
    }
    return true;
}

void MySQLRes::showAllRes()
{

    for(int i=0; i < mysql_num_rows(m_data.get()); ++i)
    {
         // 获取下一行
        MYSQL_ROW row = mysql_fetch_row(m_data.get());
        if(row <= 0)
         {
             break;
        }
        // mysql_num_fields()返回结果集中的字段数
        for(int j=0; j < mysql_num_fields(m_data.get()); ++j)
        {
			cout << row[j] << " ";
        }
		cout << endl;
    }

}

void MySQLRes::showTableContent()
{
    unsigned int num_fields;
	unsigned int i;
	MYSQL_FIELD *field;

	num_fields = getColumnCount();
	for(i = 0; i < num_fields; i++)
	{
		field = mysql_fetch_field_direct(m_data.get(), i);
		printf("Field %u name is %s\n", i, field->name);
		//printf("Field %u type is %s\n", i, field->type);
	}
}

int MySQLRes::getDataCount() {
    return mysql_num_rows(m_data.get());
}

int MySQLRes::getColumnCount() {
    return mysql_num_fields(m_data.get());
}

int MySQLRes::getColumnBytes(int idx) {
    return m_curLength[idx];
}

//暂未实现
int MySQLRes::getColumnType(int idx) {
    return 0;
}

std::string MySQLRes::getColumnName(int idx) {
    MYSQL_FIELD *field = mysql_fetch_field_direct(m_data.get(), idx);
    //printf("Field %u name is %s\n", idx, field->name);
    return std::string(field->name);
}

bool MySQLRes::isNull(int idx) {
    if(m_cur[idx] == nullptr) {
        return true;
    }
    return false;
}

int8_t MySQLRes::getInt8(int idx) {
    return getInt64(idx);
}

uint8_t MySQLRes::getUint8(int idx) {
    return getInt64(idx);
}

int16_t MySQLRes::getInt16(int idx) {
    return getInt64(idx);
}

uint16_t MySQLRes::getUint16(int idx) {
    return getInt64(idx);
}

int32_t MySQLRes::getInt32(int idx) {
    return getInt64(idx);
}

uint32_t MySQLRes::getUint32(int idx) {
    return getInt64(idx);
}

int64_t MySQLRes::getInt64(int idx) {
    return TypeUtil::Atoi(m_cur[idx]);
}

uint64_t MySQLRes::getUint64(int idx) {
    return getInt64(idx);
}

float MySQLRes::getFloat(int idx) {
    return getDouble(idx);
}

double MySQLRes::getDouble(int idx) {
    return TypeUtil::Atof(m_cur[idx]);
}

std::string MySQLRes::getString(int idx) {
    return std::string(m_cur[idx], m_curLength[idx]);
}

std::string MySQLRes::getBlob(int idx) {
    return std::string(m_cur[idx], m_curLength[idx]);
}

time_t MySQLRes::getTime(int idx) {
    if(!m_cur[idx]) {
        return 0;
    }
    return Str2Time(m_cur[idx]);
}

bool MySQLRes::next() {
    m_cur = mysql_fetch_row(m_data.get());
    m_curLength = mysql_fetch_lengths(m_data.get());
    return m_cur;
}


ISQLData::ptr MySQL::query(const char* format, ...) {
    va_list ap;
    va_start(ap, format);
    auto rt = query(format, ap);
    va_end(ap);
    return rt;
}

ISQLData::ptr MySQL::query(const char* format, va_list ap) {
    m_cmd = StringUtil::Formatv(format, ap);
    MYSQL_RES* res = my_mysql_query(m_mysql.get(), m_cmd.c_str());
    if(!res) {
        m_hasError = true;
        return nullptr;
    }
    m_hasError = false;
    ISQLData::ptr rt(new MySQLRes(res, mysql_errno(m_mysql.get())
                        ,mysql_error(m_mysql.get())));
    return rt;
}

ISQLData::ptr MySQL::query(const std::string& sql) {
    m_cmd = sql;
    MYSQL_RES* res = my_mysql_query(m_mysql.get(), m_cmd.c_str());
    if(!res) {
        m_hasError = true;
        return nullptr;
    }
    m_hasError = false;
    ISQLData::ptr rt(new MySQLRes(res, mysql_errno(m_mysql.get())
                        ,mysql_error(m_mysql.get())));
    return rt;

}

const char* MySQL::cmd() {
    return m_cmd.c_str();
}

std::string MySQL::getDbname() const{
    return m_dbname;
}

bool MySQL::use(const std::string& dbname) {
    if(!m_mysql) {
        return false;
    }
    if(m_dbname == dbname) {
        return true;
    }
    if(mysql_select_db(m_mysql.get(), dbname.c_str()) == 0) {
        m_dbname = dbname;
        m_hasError = false;
        return true;
    } else {
        m_dbname = "";
        m_hasError = true;
        return false;
    }
}

std::string MySQL::getErrStr() {
    if(!m_mysql) {
        return "mysql is null";
    }
    const char* str = mysql_error(m_mysql.get());
    if(str) {
        return str;
    }
    return "";
}

int MySQL::getErrno() {
    if(!m_mysql) {
        return -1;
    }
    return mysql_errno(m_mysql.get());
}

uint64_t MySQL::getInsertId() {
    if(m_mysql) {
        return mysql_insert_id(m_mysql.get());
    }
    return 0;
}

MySQLTransaction::ptr MySQLTransaction::Create(MySQL::ptr mysql, bool auto_commit) {
    MySQLTransaction::ptr rt(new MySQLTransaction(mysql, auto_commit));
    if(rt->begin()) {
        return rt;
    }
    return nullptr;
}

MySQLTransaction::~MySQLTransaction() {
    if(m_autoCommit) {
        commit();
    } else {
        rollback();
    }
}

int64_t MySQLTransaction::getLastInsertId() {
    return m_mysql->getLastInsertId();
}

bool MySQLTransaction::begin() {
    int rt = execute("BEGIN");
    return rt == 0;
}

bool MySQLTransaction::commit() {
    if(m_isFinished || m_hasError) {
        return !m_hasError;
    }
    int rt = execute("COMMIT");
    if(rt == 0) {
        m_isFinished = true;
    } else {
        m_hasError = true;
    }
    return rt == 0;
}

bool MySQLTransaction::rollback() {
    if(m_isFinished) {
        return true;
    }
    int rt = execute("ROLLBACK");
    if(rt == 0) {
        m_isFinished = true;
    } else {
        m_hasError = true;
    }
    return rt == 0;
}

int MySQLTransaction::execute(const char* format, ...) {
    va_list ap;
    va_start(ap, format);
    return execute(format, ap);
}

int MySQLTransaction::execute(const char* format, va_list ap) {
    if(m_isFinished) {
        cout << "transaction is finished, format=" << format;
        return -1;
    }
    int rt = m_mysql->execute(format, ap);
    if(rt) {
        m_hasError = true;
    }
    return rt;
}

int MySQLTransaction::execute(const std::string& sql) {
    if(m_isFinished) {
        cout << "transaction is finished, sql=" << sql;
        return -1;
    }
    int rt = m_mysql->execute(sql);
    if(rt) {
        m_hasError = true;
    }
    return rt;

}

std::shared_ptr<MySQL> MySQLTransaction::getMySQL() {
    return m_mysql;
}

MySQLTransaction::MySQLTransaction(MySQL::ptr mysql, bool auto_commit)
    :m_mysql(mysql)
    ,m_autoCommit(auto_commit)
    ,m_isFinished(false)
    ,m_hasError(false) {
}

MySQLManager::MySQLManager()
    :m_maxConn(10) {
    mysql_library_init(0, nullptr, nullptr);
}

MySQLManager::~MySQLManager() {
    mysql_library_end();
    for(auto& i : m_conns) {
        for(auto& n : i.second) {
            delete n;
        }
    }
}

MySQL::ptr MySQLManager::get(const std::string& name) {
    MutexLockGuard locker(lock);
    auto it = m_conns.find(name);
    if(it != m_conns.end()) {
        if(!it->second.empty()) {
            MySQL* rt = it->second.front();
            it->second.pop_front();
            
            if(!rt->isNeedCheck()) {
                rt->m_lastUsedTime = time(0);
                return MySQL::ptr(rt, std::bind(&MySQLManager::freeMySQL,
                            this, name, std::placeholders::_1));
            }
            if(rt->ping()) {
                rt->m_lastUsedTime = time(0);
                return MySQL::ptr(rt, std::bind(&MySQLManager::freeMySQL,
                            this, name, std::placeholders::_1));
            } else if(rt->connect()) {
                rt->m_lastUsedTime = time(0);
                return MySQL::ptr(rt, std::bind(&MySQLManager::freeMySQL,
                            this, name, std::placeholders::_1));
            } else {
                cout << "reconnect " << name << " fail";
                return nullptr;
            }
        }
 /*    }
    auto config = g_mysql_dbs->getValue();
    auto sit = config.find(name);
    std::map<std::string, std::string> args;
    if(sit != config.end()) {
        args = sit->second;
    } else {
        sit = m_dbDefines.find(name);
        if(sit != m_dbDefines.end()) {
            args = sit->second;
        } else {
            return nullptr;
        }
    } */
    
    /* MySQL* rt = new MySQL(args);
    if(rt->connect()) {
        rt->m_lastUsedTime = time(0);
        return MySQL::ptr(rt, std::bind(&MySQLManager::freeMySQL,
                    this, name, std::placeholders::_1));
    } else {
        delete rt;
        return nullptr;
    } */
}
}

void MySQLManager::registerMySQL(const std::string& name, const std::map<std::string, std::string>& params) {
    MutexLockGuard locker(lock);
    m_dbDefines[name] = params;
}

void MySQLManager::checkConnection(int sec) {
    time_t now = time(0);
    std::vector<MySQL*> conns;
    MutexLockGuard locker(lock);
    for(auto& i : m_conns) {
        for(auto it = i.second.begin();
                it != i.second.end();) {
            if((int)(now - (*it)->m_lastUsedTime) >= sec) {
                auto tmp = *it;
                i.second.erase(it++);
                conns.push_back(tmp);
            } else {
                ++it;
            }
        }
    }
    
    for(auto& i : conns) {
        delete i;
    }
}

int MySQLManager::execute(const std::string& name, const char* format, ...) {
    va_list ap;
    va_start(ap, format);
    int rt = execute(name, format, ap);
    va_end(ap);
    return rt;
}

int MySQLManager::execute(const std::string& name, const char* format, va_list ap) {
    auto conn = get(name);
    if(!conn) {
        cout << "MySQLManager::execute, get(" << name
            << ") fail, format=" << format;
        return -1;
    }
    return conn->execute(format, ap);
}

int MySQLManager::execute(const std::string& name, const std::string& sql) {
    auto conn = get(name);
    if(!conn) {
        cout << "MySQLManager::execute, get(" << name
            << ") fail, sql=" << sql;
        return -1;
    }
    return conn->execute(sql);
}

ISQLData::ptr MySQLManager::query(const std::string& name, const char* format, ...) {
    va_list ap;
    va_start(ap, format);
    auto res = query(name, format, ap);
    va_end(ap);
    return res;
}

ISQLData::ptr MySQLManager::query(const std::string& name, const char* format, va_list ap) {
    auto conn = get(name);
    if(!conn) {
        cout << "MySQLManager::query, get(" << name
            << ") fail, format=" << format;
        return nullptr;
    }
    return conn->query(format, ap);
}

ISQLData::ptr MySQLManager::query(const std::string& name, const std::string& sql) {
    auto conn = get(name);
    if(!conn) {
        cout << "MySQLManager::query, get(" << name
            << ") fail, sql=" << sql;
        return nullptr;
    }
    return conn->query(sql);
}

MySQLTransaction::ptr MySQLManager::openTransaction(const std::string& name, bool auto_commit) {
    auto conn = get(name);
    if(!conn) {
        cout << "MySQLManager::openTransaction, get(" << name
            << ") fail";
        return nullptr;
    }
    MySQLTransaction::ptr trans(MySQLTransaction::Create(conn, auto_commit));
    return trans;
}

void MySQLManager::freeMySQL(const std::string& name, MySQL* m) {
    if(m->m_mysql) {
        MutexLockGuard locker(lock);
        if(m_conns[name].size() < (size_t)m->m_poolSize) {
            m_conns[name].push_back(m);
            return;
        }
    }
    delete m;
}

ISQLData::ptr MySQLUtil::Query(const std::string& name, const char* format, ...) {
    va_list ap;
    va_start(ap, format);
    auto rpy = Query(name, format, ap);
    va_end(ap);
    return rpy;
}

ISQLData::ptr MySQLUtil::Query(const std::string& name, const char* format,va_list ap) {
    auto m = MySQLMgr::GetInstance()->get(name);
    if(!m) {
        return nullptr;
    }
    return m->query(format, ap);
}

ISQLData::ptr MySQLUtil::Query(const std::string& name, const std::string& sql) {
    auto m = MySQLMgr::GetInstance()->get(name);
    if(!m) {
        return nullptr;
    }
    return m->query(sql);
}

ISQLData::ptr MySQLUtil::TryQuery(const std::string& name, uint32_t count, const char* format, ...) {
    for(uint32_t i = 0; i < count; ++i) {
        va_list ap;
        va_start(ap, format);
        auto rpy = Query(name, format, ap);
        va_end(ap);
        if(rpy) {
            return rpy;
        }
    }
    return nullptr;
}

ISQLData::ptr MySQLUtil::TryQuery(const std::string& name, uint32_t count, const std::string& sql) {
    for(uint32_t i = 0; i < count; ++i) {
        auto rpy = Query(name, sql);
        if(rpy) {
            return rpy;
        }
    }
    return nullptr;

}

int MySQLUtil::Execute(const std::string& name, const char* format, ...) {
    va_list ap;
    va_start(ap, format);
    auto rpy = Execute(name, format, ap);
    va_end(ap);
    return rpy;
}

int MySQLUtil::Execute(const std::string& name, const char* format, va_list ap) {
    auto m = MySQLMgr::GetInstance()->get(name);
    if(!m) {
        return -1;
    }
    return m->execute(format, ap);
}

int MySQLUtil::Execute(const std::string& name, const std::string& sql) {
    auto m = MySQLMgr::GetInstance()->get(name);
    if(!m) {
        return -1;
    }
    return m->execute(sql);

}

int MySQLUtil::TryExecute(const std::string& name, uint32_t count, const char* format, ...) {
    int rpy = 0;
    for(uint32_t i = 0; i < count; ++i) {
        va_list ap;
        va_start(ap, format);
        rpy = Execute(name, format, ap);
        va_end(ap);
        if(!rpy) {
            return rpy;
        }
    }
    return rpy;
}

int MySQLUtil::TryExecute(const std::string& name, uint32_t count, const std::string& sql) {
    int rpy = 0;
    for(uint32_t i = 0; i < count; ++i) {
        rpy = Execute(name, sql);
        if(!rpy) {
            return rpy;
        }
    }
    return rpy;
}


