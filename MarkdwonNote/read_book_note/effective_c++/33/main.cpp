#include"Base.h"
#include"Derived.h"

int main()
{
	Derived m_Derived;
	int x=10;
	m_Derived.mFun1();
	m_Derived.mFun1(x);
	m_Derived.mFun2();
	m_Derived.mFun3();
	m_Derived.mFun3(x);
	m_Derived.mFun4();
	
	return 0;
}