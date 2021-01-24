#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int func(int a)
{
    //pthread_exit(NULL);
    exit(1);
}

void *tfn(void *arg)
{
    int i;

    i = (int)arg;
    sleep(i);
    if (i == 2)
        func(8888);

    printf("I'm %dth thread, Thread_ID = %lu\n", i+1, pthread_self());


    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int n = 5, i;
    pthread_t tid;

    if (argc == 2)
        n = atoi(argv[1]);

    for (i = 0; i < n; i++) {
        pthread_create(&tid, NULL, tfn, (void *)i);
    }
    printf("I am main, and I am not a process, I'm a thread!\n" 
            "main_thread_ID = %lu\n", pthread_self());

    pthread_exit(NULL); 
    //exit(1);
    //return 0;
}
