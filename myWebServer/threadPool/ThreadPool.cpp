#include "ThreadPool.h" 
#include <stdio.h>
#include <stdlib.h>


ThreadPool::ThreadPool(int _threadNum , int _queueSize)
:m_locker(),m_cond(m_locker),isRunning(true)
{
	createThreads(_threadNum,_queueSize);
}

ThreadPool::~ThreadPool()
{
	destroyThreads();	
}

int ThreadPool::addTask(const Task& task)
{
	printLog("addTask() begin");
	int err = 0;
	if( m_locker.lock() == false )
	{
		printLog("addTask() lock  failure");
		return THREADPOOL_LOCK_FAILURE;
	}
	do
	{
		// 请求队列满
		if( taskQueue.size() >= queueSize )
		{
			m_locker.unlock();
			err = THREADPOOL_QUEUE_FULL;
			break;
		}

		// 线程池停止
		if(!isRunning)
		{
			err = THREADPOOL_SHUTDOWN;
            break;
		}
		printLog("addTask() push_back  ");
		taskQueue.push_back(task);

	} while(false);

	if(m_locker.unlock()!=0)
	{
		return THREADPOOL_LOCK_FAILURE;
	}

	//唤醒至少一个阻塞在条件变量上的线程
	if(m_cond.notify() !=0 )
	{
		err = THREADPOOL_NOTIFY_FAILURE;      
	}
	
	printLog("addTask() end");
	return err;
}

int ThreadPool::getTaskQueueSize()
{
	m_locker.lock();
	int size = taskQueue.size();
	m_locker.unlock();
	return size;
}

int ThreadPool::destroyThreads()
{
	printLog("destroyThreads() begin");
    int err = 0;

	if( m_locker.lock() == false )
	{
		return THREADPOOL_LOCK_FAILURE;
	}

	do
	{
		if(!isRunning)
		{
			err = THREADPOOL_SHUTDOWN;
            break;
		}

		if( m_cond.notifyAll() !=0 || m_locker.unlock() == true)
		{
			err = THREADPOOL_LOCK_FAILURE;
            break;      
		}

		//回收进程
		for (int i = 0; i < threadNum; i++)
		{
			if(pthread_join(threads[i], NULL) != 0)
            {
                err = THREADPOOL_THREAD_FAILURE;
            }
		}

	} while (false);
	
	if(!err) 
    {
        free(threads);
		threads = NULL;
    }
	printLog("destroyThreads() end");
    return err;
}

int ThreadPool::createThreads(int _threadNum , int _queueSize)
{
	printLog("createThreads() begin");
	if(_threadNum <= 0 || _threadNum > MAX_THREADS || _queueSize <= 0 || _queueSize > MAX_QUEUE) 
    {
        printLog("please check the value of threadNum and queueSize");
		return -1;
    }

	threadNum = _threadNum;
	queueSize = _queueSize;

	threads = (pthread_t*)malloc(sizeof(pthread_t) * threadNum);
	for (int i = 0; i < threadNum; i++)
	{
		if(pthread_create(&threads[i], NULL, threadFunc, this)!= 0) //线程创建完成，并去执行对应函数threadFunc
		{
			return -1;
		}
	}
	
	printLog("createThreads() end");

	return 0;
}

void ThreadPool::printLog(std::string str)
{
	if( openLog == 1 )
	std::cout<<str<<std::endl;
}

void* ThreadPool::threadFunc(void* arg)
{
	ThreadPool* pool = static_cast<ThreadPool*>(arg);
	pool->run();
    return pool;
}

void ThreadPool::run()
{
	printLog("run() begin");
	while (true)
    {
		Task task = NULL;
		m_locker.lock();
		

		while (taskQueue.empty() && isRunning)
		{
			//刚开始所有任务都阻塞等待m_cond
			m_cond.wait();
		}

		if (!isRunning && taskQueue.empty())
		{
			break;
		}

		task = taskQueue.front();
		taskQueue.pop_front();
		m_locker.unlock();
		task();
	}
	
	m_locker.unlock();
	printLog("run() end");

}
