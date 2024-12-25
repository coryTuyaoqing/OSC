#include <pthread.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <time.h>

#define NUM_THREADS 5

pthread_mutex_t mutex;
int counter = 0;

void *runner(void *param)
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
     
        if(counter > 1000){
             pthread_mutex_unlock(&mutex);
             break;
        }  
        counter ++;
        printf("Counter= %d\n", counter);
        sleep(1);
        pthread_mutex_unlock(&mutex);

        sleep(1); 
    }
    pthread_exit(0);
}

int main(int argc, char *argv[])
{
    int i;
    pthread_t tid[NUM_THREADS]; 
    pthread_attr_t attr;        

    /* get the default attributes */
    pthread_attr_init(&attr);
    pthread_mutex_init(&mutex, NULL);

    /* create the threads */
    for (i = 0; i < NUM_THREADS; i++)
        pthread_create(&tid[i], &attr, runner, NULL);

    /*Now join on each thread */
    for (i = 0; i < NUM_THREADS; i++)
        pthread_join(tid[i], NULL);
}
