#include "CUpload.h"
#include "memory.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <string>
#include "json/json.h"
#include "TEncode.h"

using namespace EncodeFun;

CREATE_STATIC_LOGGERMANAGER(Upload);


Response Upload::upload(Request req)
{
	LogDebug("Register::reg() method : %s,  url: %s,  params: %s, token: %s",
        req.getMethod().c_str(),
        req.getUrl().c_str(),
        req.getUrl().c_str(),
        req.getToken().c_str());
	Response res;
	string data;
	string method = req.getMethod();
	
	//[{"name":"IMG_20171020_ ....}],去掉头尾的[]
	string jsonStr=(req.getParams()).substr(1,(req.getParams().length()-2));
	
	Json::Reader reader;
	Json::Value root;
	string pictureName;
	string srcStr;
	//"name":"IMG_20170304_205535.png","base64":"data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAxMAAAH6C
	if(reader.parse(jsonStr,root))
    {
		pictureName = root["name"].asString();
		srcStr = root["base64"].asString();
		int pos = srcStr.find_first_of(',');
		srcStr =srcStr.substr(pos+1);
	}
	
	LogInfo("pictureName=%s",pictureName.c_str());
	//LogError("srcStr=%s",srcStr.c_str());
	
	srcStr=TEncode::base64Decode(srcStr);

	string fileName=uploadPath+pictureName;

	//生成图片文件
	char *p =srcStr.c_str();
	int fd = 0;
	fd = open(fileName.c_str(), O_CREAT | O_WRONLY, 0644);
	if (fd < 0)
	{
		LogError("fd open error");
		res.setResData("fd open error");
		return res;
	}
	
	write(fd, p, srcStr.length());
	close(fd);
	//生成图片文件
	
	if ("GET" == method )
	{
	  //上传无GET方法
	}
	else if ("POST" == method)
	{
		data = "upload file success , path is " + fileName;
		res.setResData("fd open error");
	}

	LogDebug("upload end");
	return res;
}
