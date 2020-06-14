
#include "MsgServiceUtil.h"

using namespace ServiceSpace;

//"{\"EVENT\" : 1001, \"MSG\" : \"hello world\"}"
void TypeUtil::jsonStrParse(string str)
{
	Json::Reader reader;
	Json::Value json_object;
	string json_document = str;
	if (!reader.parse(json_document, json_object)){
		cout << "error" << endl;
		
	}
	else{
		cout <<"EVENT:" <<json_object["EVENT"] << " MSG:" << json_object["MSG"] << endl;
	}
	
}