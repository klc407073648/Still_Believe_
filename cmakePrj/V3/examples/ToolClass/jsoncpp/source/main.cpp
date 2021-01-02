#include <iostream>
#include "JsonUtil.h"


int main(int args, char* argv[])
{
    std::string str = "{\"uploadid\": \"teststr\",\"code\": 100,\"msg\": \"\",\"files\": \"\"}";
    std::cout << str << std::endl;
    std::cout << JsonUtil::isValid(str) << std::endl;
    std::cout << JsonUtil::getInt(str,"code") << std::endl;
    std::cout << JsonUtil::getStr(str, "uploadid") << std::endl;

    std::string jsonStr;
    std::string newStr;
    std::cout << "-----set func test---------" << std::endl;
    JsonUtil::setStr(jsonStr, "aaa", "kkkkkkk");
    JsonUtil::setInt(jsonStr, "num", 1024);
    JsonUtil::setObj(newStr, "json", jsonStr);
    std::vector<std::string> testArray;
    testArray.push_back("1");
    testArray.push_back("2");
    JsonUtil::setArray(newStr, "array", testArray);
    JsonUtil::setStr(newStr, "strKey", "xxx");
    JsonUtil::setInt(newStr, "intKey", 102);
    std::cout << newStr << std::endl;
    // get func test
    std::cout << "------get func test---------" << std::endl;
    std::cout << JsonUtil::getInt(newStr, "intKey") << std::endl;
    std::cout << JsonUtil::getStr(newStr, "strKey") << std::endl;
    std::cout << JsonUtil::getObj(newStr, "json") << std::endl;
    std::vector<std::string> testGetArray;
    testGetArray = JsonUtil::getArray(newStr, "array");
    for(auto testStr : testGetArray)
    {
        std::cout << testStr << std::endl;   
    }
    return 0;
}