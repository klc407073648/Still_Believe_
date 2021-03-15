#ifndef _POINT_H_
#define _POINT_H_

#include<iostream>

using namespace std;

class Point
{
    public:
        Point(int x,int y);
        ~Point();
		void printPoint();
    private:
        int m_x,m_y;
};


#endif //_POINT_H_