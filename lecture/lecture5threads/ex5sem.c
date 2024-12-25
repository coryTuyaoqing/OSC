#include <pthread.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <time.h>

#include <semaphore.h>

#define NUM_THREADS 5

sem_t sem;
int counter = 0;

/* the thread runs in this function */
void *runner(void *param);

int main(int argc, char *argv[])
{
    int i;
    pthread_t tid[NUM_THREADS]; /* the thread identifier */
    pthread_attr_t attr;        /* set of attributes for the thread */
    pthread_attr_init(&attr);

    sem_init(&sem, 0, 3);

    srand(time(NULL)); 

    for (i = 0; i < NUM_THREADS; i++)
        pthread_create(&tid[i], &attr, runner, NULL);
    for (i = 0; i < NUM_THREADS; i++)
        pthread_join(tid[i], NULL);
}
double rand01()
{
    return (double)rand() / (double)RAND_MAX;
}

/**
 * The thread will begin control in this function.
 */
void *runner(void *param)
{
    while (1)
    {
        pthread_t info = pthread_self();
        double s = rand01() * (double)2;

        sem_wait(&sem);

        sleep(s);
        int v = 0;
        sem_getvalue(&sem, &v);
        printf(" v: %d (%ld)\n", v, info);

        sem_post(&sem);

        sleep(1);
    }

    pthread_exit(0);
}
