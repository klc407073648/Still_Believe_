#pragma once
class noncopyable
{
protected:
    noncopyable() {}
    ~noncopyable() {}
private:
    noncopyable(const noncopyable&);
    const noncopyable& operator=(const noncopyable&);
};

/*
C++11标准：

class noncopyable
{
protected:
    //默认的构造函数和析构函数是保护的
    //使用编译器的自动产生的默认实现
    noncopyable() = default;
    ~noncopyable() = default;
 
    //使用delete关键字禁止编译器自动产生复制构造函数和复制赋值操作符
    noncopyable( const noncopyable& ) = delete;
    const noncopyable& operator=( const noncopyable& ) = delete;
};

*/