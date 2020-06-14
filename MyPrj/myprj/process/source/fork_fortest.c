#include<unistd.h>
#include <stdio.h>
#include"myfun.h"


int main()
{
   myfun(5);
   printf("Test Func begin!\n");
   pid_t m_pid;
   int i;
   for(i=0;i<5;i++)
   {
	  m_pid= fork();
	  if( m_pid == 0)
		 break;
   }
    sleep(i);
	if( i < 5)
	{
		printf("I'm child %d id:\n",i,getpid());
	}
	else
	{
	   
	   printf("I'm parent  id:\n",getpid());
	   printf("Test Func end!\n");
	}

 return 0;
}

