#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int sum;                   /* this data is shared by the thread(s) */
void *runner(void *param); /* threads call this function */

int main(int argc, char *argv[])
{

    pthread_t tid;       
    pthread_attr_t attr; 
    pthread_attr_init(&attr);
    pthread_create(&tid, &attr, runner, argv[1]);
  
    sleep(5);
    pthread_cancel(tid);
    sleep(5);

    pthread_join(tid, NULL);

    printf("sum = %d\n", sum);
}


void *runner(void *param)
{
    int i, upper = atoi(param);
    sum = 0;
    for (i = 1; i <= upper; i++){
        sleep(1);
        pthread_testcancel();

    
        sum += i; 
           printf("threadsum=%d\n", sum);
    }
    pthread_exit(0);
}
