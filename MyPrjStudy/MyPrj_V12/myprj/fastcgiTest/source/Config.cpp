#include "Config.h"

CREATE_STATIC_LOGGERMANAGER(Config);

Config::Config()
{
	Priority mPriority= Priority::DEBUG;
	mlogger->setRootPriority(mPriority);
    readConfFromJson();
	LogDebug("Config()");
}

Config::~Config()
{

}

void Config::readConfFromJson()
{
	Json::Reader reader;
	Json::Value root;
 
	//从文件中读取，保证当前文件有demo.json文件  
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

       LogInfo("mysql_host=%s,mysql_user=%s,mysql_passwd=%s,mysql_dbname=%s,m_name=%s,m_description=%s,m_type=%s", \
       mysql_host.c_str(),mysql_user.c_str(),mysql_passwd.c_str(),mysql_dbname.c_str(),m_name.c_str(),\
       m_description.c_str(),m_type.c_str());
	   LogInfo("Reading Complete!");
	}
	else
	{
        LogError("parse error!");
	}
 
	in.close();
}