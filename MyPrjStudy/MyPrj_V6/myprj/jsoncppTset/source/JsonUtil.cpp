#include "JsonUtil.h"
#include "json/json.h"


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
