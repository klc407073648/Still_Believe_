#include <iostream>
#include <memory>

using namespace std;

class MyTest
{
public:
	MyTest()
	:n(0)
	{
		cout<<"MyTest()"<<endl;
	}
	~MyTest()
	{
		cout<<"~MyTest()"<<endl;
	}
	MyTest(const MyTest& m)
	{
		cout<<"copying MyTest"<<endl;
		this->n = m.n;
	}
	void printMyTest(MyTest* m)
	{
		cout<<"printMyTest "<<m->n<<endl;
	}
private:
	int n;
};


int main()
{
	std::shared_ptr<MyTest> m_test = new MyTest();
	std::shared_ptr<MyTest> m_test(new MyTest());
	m_test->printMyTest(m_test.get());
	m_test->printMyTest(m_test);//error: cannot convert ‘std::shared_ptr<MyTest>’ to ‘MyTest*’ ,通过get方法获取raw指针
	return 0;
}