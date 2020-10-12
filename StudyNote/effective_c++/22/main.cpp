#include <iostream>
#include <memory>
#include <stdio.h>

using namespace std;


class Test
{
public:
	Test()
	:test1(1),
	 test2(2),
	 test3(3)
	{
		cout<<"Test()"<<endl;
	}
	~Test()
	{
		cout<<"~Test()"<<endl;
	}
	int test1;
protected:
	int test2;
private:
	int test3;
};


int main()
{
	Test m_Test;
	cout<<"test1"<<m_Test.test1<<endl;
	cout<<"test2"<<m_Test.test2<<endl;
	cout<<"test3"<<m_Test.test3<<endl;
	return 0;
}