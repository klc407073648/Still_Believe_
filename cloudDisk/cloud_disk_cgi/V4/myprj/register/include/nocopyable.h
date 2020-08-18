/**
 * @file noncopyable.h
 * @brief 非拷贝类封装
 * @author klc
 * @date 2020-04-30
 * @copyright Copyright (c) 2020年 klc
 */

#ifndef __STIBEL_NOCOPYABLE_H__
#define __STIBEL_NOCOPYABLE_H__

class noncopyable
{
protected:
    noncopyable() {}
    ~noncopyable() {}
private:
    noncopyable(const noncopyable&);
    const noncopyable& operator=(const noncopyable&);
};

#endif //__STIBEL_NOCOPYABLE_H__