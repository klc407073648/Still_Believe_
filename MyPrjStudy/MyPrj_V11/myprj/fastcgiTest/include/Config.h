/**
 * @file config.h
 * @brief 配置模块
 * @author sylar.yin
 * @email 564628276@qq.com
 * @date 2019-05-22
 * @copyright Copyright (c) 2019年 sylar.yin All rights reserved (www.sylar.top)
 */

#include "json/json.h"
#include<string>
#include <iostream>  
#include <fstream> 
#include"LoggerManager.h"

using namespace minilog;

using namespace std;
const string json_file = "./myprj/fastcgiTest/conf/conf.json";

class Config {
public:
    Config();
    ~Config();

    void readConfFromJson();
    /**
     * @brief 返回配置参数名称
     */
    const std::string& getName() const { return m_name;}

    /**
     * @brief 返回配置参数的描述
     */
    const std::string& getDescription() const { return m_description;}

    /**
     * @brief 返回配置参数的类型
     */
    const std::string getTypeName() const { return m_type;}
private:
    DECLARE_STATIC_LOGGERMANAGER(); 
    /// 配置参数的名称
    std::string m_name;
    /// 配置参数的描述
    std::string m_description;
    /// 配置参数的描述
    std::string m_type;
    //typedef std::unordered_map<std::string, Config::ptr> ConfigVarMap;
};


