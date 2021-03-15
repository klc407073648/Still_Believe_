#include"Point.h"

Point::Point(int x,int y)
:m_x(x),
 m_y(y)
{
	cout<<"Point()"<<endl;
}

Point::~Point()
{
	cout<<"~Point()"<<endl;		
}

void Point::printPoint()
{
	cout<<"printPoint() x: "<<m_x<<" , y: "<<m_y<<endl;
}
