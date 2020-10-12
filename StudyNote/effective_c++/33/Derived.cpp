#include"Derived.h"

Derived::Derived()
{
	cout<<"Derived()"<<endl;
}

Derived::~Derived()
{
	cout<<"~Derived()"<<endl;
}
		
void Derived::mFun1()
{
	cout<<"Derived::mFun1()"<<endl;
}

void Derived::mFun3()
{
	cout<<"Derived::mFun3()"<<endl;
}

void Derived::mFun4()
{
	cout<<"Derived::mFun4()"<<endl;
}
		
