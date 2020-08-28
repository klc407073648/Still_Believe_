#include "CMyRedis.h"
#include "CErrorMsg.h"
#include <stdio.h>
#include <iostream>
using std::cout;
int main()
{
	CMyRedis myRedis;
	try{
		myRedis.Connect("127.0.0.1",6379);
		cout<<"connect success!"<<endl;
		cout<<myRedis.CheckConenct()<<endl;
		myRedis.Connect("127.0.0.1", 6379);
		cout<<"check connect: "<<myRedis.CheckConenct()<<endl;
		myRedis.Set("hello", "world");
		cout<<myRedis.Get("Work")<<endl;
		myRedis.Set("set", "get");
		cout<<myRedis.Get("hello")<<endl;
		
	
		
		cout<<"**************zadd****************"<<endl;
		cout<<myRedis.ZAdd("zset-key", 1, "member1")<<endl;
		cout<<myRedis.ZAdd("zset-key", 4, "member4")<<endl;
		cout<<myRedis.ZAdd("zset-key", 3, "member3")<<endl;
		cout<<myRedis.ZAdd("zset-key", 2, "member2")<<endl;
		cout<<myRedis.ZAdd("zset-key", 5, "member5")<<endl;

		
		
		cout<<"**************zrange(3,6)****************"<<endl;
		string strArray[10];
		int nCount = 10;
		myRedis.ZRange("zset-key", 3, 6, strArray, nCount);
		for(int i = 0; i<nCount; i++)
		{
			cout<<strArray[i]<<endl;
		}
		cout<<"check connect: "<<myRedis.CheckConenct()<<endl;
		cout<<"*************zrangebysocre(0,2)***************"<<endl;
		myRedis.ZRangeByScore("zset-key",0,2,strArray,nCount);
		for(int i = 0; i<nCount; i++)
		{
			cout<<strArray[i]<<endl;
		}
		cout<<"check connect: "<<myRedis.CheckConenct()<<endl;
		cout<<"***************zrem*************************"<<endl;
		cout<<myRedis.ZRem("zset-key","member1")<<endl;
		cout<<myRedis.ZRem("zset-key","member2")<<endl;
		cout<<"check connect: "<<myRedis.CheckConenct()<<endl;
		myRedis.Set("hello","world");
		string str = myRedis.Get("hello");
		cout<<"hello:"<<str<<endl;
		str = myRedis.Get("null");
		cout<<"null"<<str<<endl;
		cout<<myRedis.Del("hello")<<endl;
		cout<<myRedis.Del("world")<<endl;

		cout<<myRedis.Push("list-keytest","item0")<<endl;
		cout<<myRedis.Push("list-keytest","item1")<<endl;
		cout<<myRedis.Push("list-keytest","item2")<<endl;
		for(int i = 0; i<8; i++)
		{
			cout<<myRedis.Pop("list-keytest")<<endl;
		}
		cout<<myRedis.Lindex("list-keytest", 1)<<endl;
		cout<<myRedis.Lindex("list-keytest", 10)<<endl;
		string array[10];
		nCount =10;
		myRedis.Lrange("list-keytest",200,201,array,nCount);
		for(int i = 0; i<nCount; i++)
		{
			cout<<array[i]<<endl;
		}
		cout<<myRedis.Sadd("set-key", "test1")<<endl;
		cout<<myRedis.Sadd("set-key", "test2")<<endl;
		cout<<myRedis.Sadd("set-key", "test3")<<endl;
		cout<<myRedis.Sadd("set-key", "test4")<<endl;
		cout<<"***********smember:*************"<<endl;
		string strArray1[20];
		nCount = 20;
		myRedis.Smembers("set-key",strArray1,nCount);
		for(int i = 0; i<nCount; i++)
		{
			cout<<strArray1[i]<<endl;
		}
		cout<<myRedis.Sadd("set-key", "hello")<<endl;
		cout<<myRedis.Srem("set-key","world")<<endl;
		cout<<myRedis.Srem("set-key","test")<<endl;
		cout<<myRedis.SisMember("set-key","world")<<endl;
		cout<<myRedis.SisMember("set-key","test")<<endl;
		cout<<myRedis.HSet("hash-key","key1","value1")<<endl;
		cout<<myRedis.HSet("hash-key","key2","value2")<<endl;
		cout<<myRedis.HSet("hash-key","key1","value1")<<endl;
		cout<<myRedis.HSet("hash-key","key3","value3")<<endl;
		string strArray2[30];
		nCount = 30;
		myRedis.HGetAll("hash-key", strArray2, nCount);
		for(int i = 0; i<nCount; i++)
		{
			cout<<strArray2[i]<<endl;

		}
		cout<<myRedis.HGet("hash-key","key1")<<endl;
		cout<<myRedis.HGet("hash-key","key3")<<endl;
		cout<<myRedis.HGet("hash-key","key8")<<endl;
		cout<<myRedis.HDel("hash-key","key1")<<endl;
		cout<<myRedis.HDel("hash-key","key11")<<endl;
	}catch(CErrorMsg& cErr)
	{
		printf("error code = %d, errmsg=%s\r\n", cErr.get_errorCode(), cErr.get_errorMsg().c_str());
		return 0;
	}
}
