#include"Time.h"

int main()
{
	TimeKeeper* m = new  AtomicClock();
	delete m;
	return 0;
}