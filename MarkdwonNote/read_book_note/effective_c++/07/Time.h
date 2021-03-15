#ifndef _TIME_H_
#define _TIME_H_

#include<iostream>

using namespace std;

class TimeKeeper
{
    public:
        TimeKeeper();
        virtual ~TimeKeeper();
    private:
        int time;
};

class AtomicClock: public  TimeKeeper
{
    public:
        AtomicClock();
        ~AtomicClock();
};



#endif //_TIME_H_