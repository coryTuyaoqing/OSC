#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define FREQ 1
#define MAXT 35
#define MINT -10

float randomTemp(){
    return (rand()%((MAXT-MINT+1)*100))/100.0 + MINT;
}

int main(void){
    srand(time(NULL));
    time_t raw_time;
    struct tm *time_info;
    char buffer[80];

    while(1){
        time(&raw_time);
        time_info = localtime(&raw_time);
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", time_info);
        printf("Temperature = %1.2f @ %s\n", randomTemp(), buffer);
        sleep(FREQ);
    }
    
    return 0;
}
