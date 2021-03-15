#include"Base.h"

Base::Base()
{
	cout<<"Base()"<<endl;
}

Base::~Base()
{
	cout<<"~Base()"<<endl;
}
		
void Base::mFun1(int i)
{
	cout<<"Base::mFun1() "<<i<<endl;
}

void Base::mFun2()
{
	cout<<"Base::mFun2()"<<endl;
}

void Base::mFun3()
{
	cout<<"Base::mFun3()"<<endl;
}
		
void Base::mFun3(double d)
{
	cout<<"Base::mFun3() "<<d<<endl;
}
