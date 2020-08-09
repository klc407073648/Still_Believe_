/**
 * @file Config.h
 * @brief 配置类封装
 * @author klc
 * @date 2020-08-03
 * @copyright Copyright (c) 2020年 klc
 */

#ifndef __STIBEL_CONFIG_H__
#define __STIBEL_CONFIG_H__

#include "json/json.h"
#include<string>
#include <iostream>  
#include <fstream> 
#include"LoggerManager.h"
#include"common.h"
using namespace minilog;

using namespace std;

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
#endif // __STIBEL_CONFIG_H__

