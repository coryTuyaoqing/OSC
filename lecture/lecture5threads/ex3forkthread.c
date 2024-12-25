#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>

#include <unistd.h>
#include <sys/syscall.h>
#include <sys/wait.h>

int value;
void *runner(void *param); // thread calls this function

int main(int argc, char *argv[])
{
    pid_t pid;
    pthread_t tid;
    pthread_attr_t attr;

    pid = fork();

    if (pid == 0)
    { // CHILD
        pthread_attr_init(&attr);
        pthread_create(&tid, &attr, runner, NULL);
        pthread_join(tid, NULL);

        printf("CHILD value = %d\n", value);
    }
    else if (pid > 0)
    {
        wait(NULL); // wait for child to finish
        printf("PARENT value = %d\n", value);
    }
}

void *runner(void *param)
{
    value = 5;

    pthread_exit(0);
}