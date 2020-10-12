#ifndef _BASE_H_
#define _BASE_H_

#include<iostream>

using namespace std;

class Base
{
    public:
        Base();
        ~Base();
		virtual void mFun1() = 0;
		virtual void mFun1(int i);
		virtual void mFun2();
		void mFun3();
		void mFun3(double d);
    private:
        int x;
};


#endif //_BASE_H_