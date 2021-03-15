#ifndef _Derived_H_
#define _Derived_H_

#include<iostream>
#include"Base.h"

using namespace std;

class Derived:public Base
{
    public:
		//using Base::mFun1;
		//using Base::mFun3;
        Derived();
        ~Derived();
		virtual void mFun1();
		void mFun3();
		void mFun4();
    private:
};


#endif //_Derived_H_