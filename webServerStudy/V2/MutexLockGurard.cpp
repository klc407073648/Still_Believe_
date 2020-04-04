class MutexLockGuard
{
public:
    explicit MutexLockGuard();
    ~MutexLockGuard();

private:
    static pthread_mutex_t lock;

private:
    MutexLockGuard(const MutexLockGuard&);//拷贝构造函数
    MutexLockGuard& operator=(const MutexLockGuard&);//赋值构造函数
};

/*通过将拷贝构造函数和赋值函数声明为私有化，使得其不能被调用。

拷贝构造函数的参数一定是引用，不能不是引用，不然会出现无限递归

赋值构造函数必须注意它的函数原型，参数必须是引用类型，
返回值也必须是引用类型，否则在传参和返回的时候都会再次调用一次拷贝构造函数

上述思想类似：
lock_guard对象只是简化了mutex对象的上锁和解锁操作，方便线程对互斥量上锁，
即在某个lock_guard对象的生命周期内，它所管理的锁对象会一直保持上锁状态；
而lock_guard的生命周期结束之后，它所管理的锁对象会被解锁。

参考网址：
https://www.runoob.com/cplusplus/cpp-copy-constructor.html

*/
————————————————————————————————————————————————————————————————
.cpp


pthread_mutex_t MutexLockGuard::lock = PTHREAD_MUTEX_INITIALIZER;

MutexLockGuard::MutexLockGuard()
{
    pthread_mutex_lock(&lock);
}

MutexLockGuard::~MutexLockGuard()
{
    pthread_mutex_unlock(&lock);
}

——————————————————————————————————————————————
调用时，直接使用MutexLockGuard();