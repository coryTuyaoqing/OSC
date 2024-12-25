#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/syscall.h>

#define NUM_THREADS 5

void *runner(void *param)
{
    int counter = 0;
    srand(time(NULL));   // Initialization, should only be called once.
    while(counter < 10){
            counter ++;
            int r = rand();      // Returns a pseudo-random integer between 0 and RAND_MAX.
            r = r % 2;
            sleep(1 + r);
            pid_t tid = syscall(SYS_gettid);
            printf("Thread = %d - Counter = %d\n", tid, counter); //printf is thread safe
    }
    sleep(1);
    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    int i;
    pthread_t tid[NUM_THREADS]; 
    pthread_attr_t attr;
    /* get the default attributes */
    pthread_attr_init(&attr);
    /* create the threads */
    for (i = 0; i < NUM_THREADS; i++)
        pthread_create(&tid[i], &attr, runner, NULL);
    /*Now join on each thread */
    for (i = 0; i < NUM_THREADS; i++)
        pthread_join(tid[i], NULL);
}
