#include "Config.h"

CREATE_STATIC_LOGGERMANAGER(Config);

Config::Config(string json_file)
{
	Priority mPriority= Priority::DEBUG;
	mlogger->setRootPriority(mPriority);
	LogDebug("Config Create!");
    readConfFromJson(json_file);
	
}

Config::~Config()
{
	LogDebug("Config Finish!");
}

void Config::readConfFromJson(const string& json_file)
{
	Json::Reader reader;
	Json::Value root;
 
	
	//从文件中读取，保证当前文件有demo.json文件  
	//const string json_file = "./myprj/login/conf/conf.json";
	ifstream in(json_file, ios::binary);
	
	if (!in.is_open())
	{
		LogError("Error opening file");
		return;
	}
 /*
 params["host"] = "localhost";
		params["user"] = "root";
		params["passwd"] = "klczxas789";
		params["dbname"] = "yourdb";*/
	if (reader.parse(in, root))
	{
       string mysql_host = root["mysql"]["host"].asString();
       string mysql_user = root["mysql"]["user"].asString();
       string mysql_passwd = root["mysql"]["passwd"].asString();
       string mysql_dbname = root["mysql"]["dbname"].asString();
	   
       string json_name = root["name"].asString();
       string json_description = root["description"].asString();
       string json_type = root["type"].asString();
       m_name = json_name;
       m_description = json_description;
       m_type =  json_type;

	   string redis_ip = root["redis"]["ip"].asString();
       string redis_port = root["redis"]["port"].asString();

	   string log4cpp_path = root["log4cpp"]["path"].asString();
       string log4cpp_expire_time = root["log4cpp"]["expire_time"].asString();

	   string dfs_client_path = root["dfs_path"]["client"].asString();

	   string storage_web_server_port = root["storage_web_server"]["port"].asString();

	   m_ConfigMap.insert(ConfigMap::value_type("mysql_host",mysql_host));
	   m_ConfigMap.insert(ConfigMap::value_type("mysql_user",mysql_user));
	   m_ConfigMap.insert(ConfigMap::value_type("mysql_passwd",mysql_passwd));
	   m_ConfigMap.insert(ConfigMap::value_type("mysql_dbname",mysql_dbname));

	   m_ConfigMap.insert(ConfigMap::value_type("redis_ip",redis_ip));
	   m_ConfigMap.insert(ConfigMap::value_type("redis_port",redis_port));

	   m_ConfigMap.insert(ConfigMap::value_type("log4cpp_path",log4cpp_path));
	   m_ConfigMap.insert(ConfigMap::value_type("log4cpp_expire_time",log4cpp_expire_time));

	   m_ConfigMap.insert(ConfigMap::value_type("dfs_client_path",dfs_client_path));

	   m_ConfigMap.insert(ConfigMap::value_type("storage_web_server_port",storage_web_server_port));
	
       LogInfo("mysql_host=%s,mysql_user=%s,mysql_passwd=%s,mysql_dbname=%s", \
       mysql_host.c_str(),mysql_user.c_str(),mysql_passwd.c_str(),mysql_dbname.c_str());

	   LogInfo("redis_ip=%s,redis_port=%s",redis_ip.c_str(),redis_port.c_str());

	   LogInfo("log4cpp_path=%s,log4cpp_expire_time=%s",log4cpp_path.c_str(),log4cpp_expire_time.c_str());
       
	   LogInfo("m_name=%s,m_description=%s,m_type=%s",m_name.c_str(),m_description.c_str(),m_type.c_str());

	   LogInfo("dfs_client_path=%s",dfs_client_path.c_str());

	   LogInfo("storage_web_server_port=%s",storage_web_server_port.c_str());

	   LogInfo("Reading Complete!");
	}
	else
	{
        LogError("parse error!");
	}
 
	in.close();
}



