#include "JsonUtil.h"

using namespace std;

bool JsonUtil::isValid(const std::string &obj)
{
    Json::Reader jsonReader;
    Json::Value jsonRoot;
    return jsonReader.parse(obj, jsonRoot);
}

std::string JsonUtil::getStr(const std::string &obj, const std::string &sKey)
{
    Json::Reader jsonReader;
    Json::Value jsonRoot;
    if(jsonReader.parse(obj, jsonRoot))
    {
        return jsonRoot[sKey].asString();
    }
    return "";
}

std::string JsonUtil::getObj(const std::string &obj, const std::string &sKey)
{
    Json::Reader jsonReader;
    Json::Value jsonRoot;
    if(jsonReader.parse(obj, jsonRoot))
    {
        return jsonRoot[sKey].toStyledString();
    }
    return "";
}

int JsonUtil::getInt(const std::string &obj,  const std::string &sKey)
{
    Json::Reader jsonReader;
    Json::Value jsonRoot;
    if(jsonReader.parse(obj, jsonRoot))
    {
        return jsonRoot[sKey].asInt();
    }
    return -1;
}

std::vector<std::string> JsonUtil::getArray(const std::string &obj, const std::string &sKey)
{

    std::string strArray = getObj(obj, sKey);
    Json::Reader jsonReader;
    Json::Value jsonRoot;
    std::vector<std::string> array;
    if(jsonReader.parse(strArray, jsonRoot))
    {
        for(auto str : jsonRoot)
        {
            array.push_back(str.asString());
        }
    }
    return array;
}

void JsonUtil::setStr(std::string &obj, const std::string &sKey,const std::string &sValue)
{
    Json::Reader jsonReader;
    Json::Value jsonRoot;
    jsonReader.parse(obj, jsonRoot);
    jsonRoot[sKey] = sValue;
    obj = jsonRoot.toStyledString();
}

void JsonUtil::setObj(std::string &obj, const std::string &sKey,const std::string &sValue)
{
    Json::Reader jsonReader;
    Json::Value jsonRoot;
    Json::Value jsonRootTmp;
    jsonReader.parse(obj, jsonRoot);
    jsonReader.parse(sValue, jsonRootTmp);
    jsonRoot[sKey] = jsonRootTmp;
    obj = jsonRoot.toStyledString();
}

void JsonUtil::setInt(std::string &obj, const std::string &sKey,const int &iValue)
{
    Json::Reader jsonReader;
    Json::Value jsonRoot;
    jsonReader.parse(obj, jsonRoot);
    jsonRoot[sKey] = iValue;
    obj = jsonRoot.toStyledString();
}

void JsonUtil::setArray(std::string &obj, const std::string &sKey, const std::vector<std::string> &array)
{
    Json::Reader jsonReader;
    Json::Value jsonRoot;
    Json::Value jsonRootTmp;
    jsonReader.parse("", jsonRootTmp);
    for(auto str : array)
    {
        jsonRootTmp.append(str);
    }
    jsonReader.parse(obj, jsonRoot);
    jsonRoot[sKey] = jsonRootTmp;
    obj = jsonRoot.toStyledString();
}

void JsonUtil::printJson(Json::Value data)  
{  
    Json::Value::Members mem = data.getMemberNames();  
    for (auto iter = mem.begin(); iter != mem.end(); iter++)  
    {  
        cout << *iter << "\t: ";  
        if (data[*iter].type() == Json::objectValue)  
        {  
            cout << endl;  
            printJson(data[*iter]);  
        }  
        else if (data[*iter].type() == Json::arrayValue)  
        {  
            cout << endl;  
            auto cnt = data[*iter].size();  
            for (auto i = 0; i < cnt; i++)  
            {  
                printJson(data[*iter][i]);  
            }  
        }  
        else if (data[*iter].type() == Json::stringValue)  
        {  
            cout << data[*iter].asString() << endl;  
        }  
        else if (data[*iter].type() == Json::realValue)  
        {  
            cout << data[*iter].asDouble() << endl;  
        }  
        else if (data[*iter].type() == Json::uintValue)  
        {  
            cout << data[*iter].asUInt() << endl;  
        }  
        else  
        {  
            cout << data[*iter].asInt() << endl;  
        }  
    }  
    return;  
}  

