#include <iostream>
#include <memory>
#include <stdio.h>

using namespace std;

struct Day
{
   explicit Day(int d)
	       :val(d)
			{}
   int val;
};

struct Month
{
   explicit Month(int d)
	       :val(d)
			{}
   int val;
};

struct Year
{
   explicit Year(int d)
	       :val(d)
			{}
   int val;
};

class Date
{
public:
	Date(const Month& m, const Day& d,const Year& y)
	:month(m),
	 day(d),
	 year(y)
	{
		cout<<"Date()"<<endl;
	}
	~Date()
	{
		cout<<"~Date()"<<endl;
	}
	void printDate()
	{
		printf("%d-%d-%d\n",year.val,month.val,day.val);
	}
private:
	Month month;
	Day day;
	Year year;
};


int main()
{
	Date m_date(Month(12),Day(20),Year(2020));
	
	Date m_date1(Day(20),Month(12),Year(2020));
	
	m_date.printDate();
	m_date1.printDate();
	
	return 0;
}