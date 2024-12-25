#include <pthread.h>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include <time.h>

#define NUM_THREADS 5

pthread_mutex_t mutex;
pthread_cond_t condvar;
int condition = 0;
int counter = 0;

/* the thread runs in this function */
void *runner(void *param);

int main(int argc, char *argv[])
{
    int i, scope;
    pthread_t tid[NUM_THREADS];
    pthread_attr_t attr;

    pthread_attr_init(&attr);
    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&condvar, NULL);

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
        pthread_mutex_lock(&mutex);
        while (condition >= 3)
            pthread_cond_wait(&condvar, &mutex);
        condition++; //
        printf("condition: %d (Thread %ld)\n", condition, info);
        pthread_mutex_unlock(&mutex);

        srand(time(NULL));
        double s = rand01() * (double)3;
        sleep(s);

        pthread_mutex_lock(&mutex);
        condition--;

        pthread_cond_signal(&condvar);
        if (counter > 10)
        {
            printf(" Limit reached: %d (%ld)\n", counter, info);
            pthread_mutex_unlock(&mutex);
            break;
        }
        counter++;
        printf(" Counter: %d (%ld)\n", counter, info);
        pthread_mutex_unlock(&mutex);

        sleep(1); // spinlocks are very agressive in regaining the control of the same thread
    }

    pthread_exit(0);
}
