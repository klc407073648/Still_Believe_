#include <iostream>
#include <fstream>
#include <string>
#include "json/json.h"

using namespace std;

int main()
{
	const char* age;
	Json::Value root;
	Json::FastWriter fast_writer;
	Json::StyledWriter style_writer;
	Json::StyledStreamWriter stream_writer;
	Json::Reader reader;
	Json::Value json_object;

	root["null"] = 0;			//注意此处在输出是显示为0，而非null
	root["message"] = "OK";
	root["age"] = 52;
	root["array"].append("arr");	// 新建一个key为array，类型为数组，对第一个元素赋值为字符串“arr”
	root["array"].append(123);		// 为数组 key_array 赋值，对第二个元素赋值为：1234

	Json::ValueType type = root.type();				//root的类型

	cout << root.toStyledString() << endl;			//格式化输出
	cout << "root_type：" <<type << endl;			//类型为7，即为类型为对象

	// 写到本地文件
	cout << "快速输出：" << endl;
	string str = fast_writer.write(root);
	cout << str << endl;		//快速输出,紧凑型
	ofstream ofs("fast_writer.json");
	ofs << str;
	ofs.close();

	cout << "格式化输出：" << endl;
	str = style_writer.write(root);
	cout << str << endl;		//格式化输出，排版型
	ofs.open("style_writer.json");
	ofs << str;
	ofs.close();


	// 对字符串解析
	//const char* json_document = "{\"age\" : 12, \"name\" : \"weng\"}";
	string json_document = "{\"age\" : 123, \"name\" : \"weng\"}";
	if (!reader.parse(json_document, json_object)){
		cout << "error" << endl;
		return 0;
	}
	else{
		cout <<"age:" <<json_object["age"] << " name" << json_object["name"] << endl;
	}
	
	// 对文件解析
	ifstream ifs("E:\\demo\\JsonTest\\JsonTest\\example.json", ios::binary);
	if (!reader.parse(ifs, json_object)){
		cout << "open error" << endl;
		ifs.close();
	}else{
		cout << "encoding:" << json_object["encoding"].asString() << endl;
		cout << "age:" <<json_object["age"].asInt() << endl;
		int num = json_object["plug"].size();
		for (int i = 0; i < num; i++){
			cout << json_object["plug"][i].asString() << " ";
		}
		ifs.close();
		cout << endl;
	}

	// 判断key是否存在
	bool is_age =  root.isMember("age");
	cout << "isMember:" <<is_age << endl;

	// 判断是否为null
	//bool is_null = root["null"].isNull();			//注意此处被赋值为0，而非null
	bool is_null = json_object["null"].isNull();
	cout << "is_null:" << is_null << endl;

	// 得到所有的key
	for (auto elem : root.getMemberNames()){
		cout << elem << " ";
	}
	cout << endl;

	// 删除成员
	cout << "remove_age:" << root.removeMember("age") << endl;

	return 0;
}
