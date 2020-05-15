/**
 * @file ThreadPool.h
 * @brief  线程池类定义
 * @author klc
 * @date 2020-05-13
 * @copyright Copyright (c) 2020年 klc
 */

#ifndef __STIBEL_THREADPOOL_H__
#define __STIBEL_THREADPOOL_H__

#include <deque>
#include <pthread.h>
#include <functional>
#include <string>
#include <iostream>
#include "MutexLock.h"
#include "Condition.h"

const int THREADPOOL_INVALID = -1;
const int THREADPOOL_LOCK_FAILURE = -2;
const int THREADPOOL_QUEUE_FULL = -3;
const int THREADPOOL_SHUTDOWN = -4;
const int THREADPOOL_THREAD_FAILURE = -5;
const int THREADPOOL_NOTIFY_FAILURE = -6;
const int THREADPOOL_GRACEFUL = 1;

const int MAX_THREADS = 1024;
const int MAX_QUEUE = 65535;

#define openLog 0

class ThreadPool
{
public:
	//定义处理任务函数的类型
	typedef std::function<void()> Task; 

	//threadNum为线程池中线程数量，queueSize是请求队列中的最大请求数量
	ThreadPool(int _threadNum = 10 ,int _queueSize = 10000);

	//析构函数，释放申请的资源
	~ThreadPool();

	//添加处理任务到线程池
	int addTask(const Task &task);
	
	//获取任务队列大小
	int getTaskQueueSize();
	
	//销毁线程
	int destroyThreads();
	
private:
	//创建线程
	int createThreads(int _threadNum , int _queueSize);

	//打印日志
	void printLog(std::string str);

	/*线程入口函数
	pthread_create() 函数要求的线程函数必须满足如下格式：void *ThreadFunc(void *args);
	对于普通类成员函数、虚函数，他们实际上都是包含了调用他们的对象的this指针，即：经过编译器优化，线程函数变为如下所示：
	void *（类名）+ ThreadFunc(this, void *args);
	就导致了该函数的格式是不符合 pthread_create() 对线程函数的要求的。故，如果类成员函数做为线程函数时，必须是静态的。
	*/
	static void *threadFunc(void *arg);

	//线程处理函数
	void run();

	//私有化，禁止赋值
	ThreadPool &operator=(const ThreadPool &);

	//私有化，禁止拷贝
	ThreadPool(const ThreadPool &);

private:
	volatile bool isRunning;  //线程运行状态，volatile关键词影响编译器编译的结果，用volatile声明的变量表示该变量随时可能发生变化，与该变量有关的运算，不要进行编译优化，以免出错
	int threadNum; 	 //线程池中的线程数
    int queueSize; 	 //请求队列中允许的最大请求数
	pthread_t *threads; //描述线程池的数组，其大小为threadNum
	std::deque<Task> taskQueue; //请求队列
	MutexLock m_locker; //互斥锁
	Condition m_cond; //条件变量
};


#endif //__STIBEL_THREADPOOL_H__