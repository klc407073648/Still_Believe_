/**
 * @file Singleton.h
 * @brief 单例类封装
 * @author klc
 * @date 2020-04-30
 * @copyright Copyright (c) 2020年 klc
 */

#ifndef __STIBEL_SINGLETON_H_
#define __STIBEL_SINGLETON_H_

#include <iostream>
#include <memory>
#include <mutex> 

using namespace std;

/**
 * @brief 单例模式智能指针封装类
 * @details T 类型
 *          X 为了创造多个实例对应的Tag
 *          N 同一个Tag创造多个实例索引
 */
template<class T, class X = void, int N = 0>
class SingletonPtr 
{
	public:
		/**
		 * @brief 返回单例智能指针
		 */
		static std::shared_ptr<T> GetInstance()
		{
			static std::shared_ptr<T> v(new T);
			return v;
        }
};

class Singleton{
public:
    typedef std::shared_ptr<Singleton> Ptr;
    
    Singleton(Singleton&)=delete;//禁止拷贝构造
    Singleton& operator=(const Singleton&)=delete;//禁止赋值
    static Ptr getInstance();
	~Singleton();
	
private:
    Singleton();
	
private:
    static Ptr m_instance_ptr;
    static std::mutex m_mutex;
};



#endif //__STIBEL_SINGLETON_H_