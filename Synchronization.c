/*include header files*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <semaphore.h>


//compile with command
//gcc -o main *.c -lpthread -lrt

/*semaphores*/
sem_t rope;
sem_t east_mutex;
sem_t west_mutex;
sem_t deadlock_protection;
sem_t counter;

/*global variables*/
int east = 0;
int west = 0;
int travel_time;

/*function prototypes*/
void crossing(int x);
void* east_side(void*);
void* west_side(void*);

/*main function*/
int main(int argc, char *argv[])
{

    char c;
    int baboonCnt=0;
    char temp[100];

    sem_init(&rope,0,1);                        //ensure mutual exclusion on rope ownership
    sem_init(&east_mutex,0,1);                  //east side on travel
    sem_init(&west_mutex,0,1);                  //west side on travel
    sem_init(&deadlock_protection,0,1);         //used to prevent deadlocks while using semaphores
    sem_init(&counter,0,3);                     //ensure only 3 baboons are allowed on the rope

    //ensure all input arguements are entered
    if ( argc == 3 )
    {
        travel_time = atoi(argv[2]);
        FILE *file;
        int baboonCnt=0;
        if (file = fopen(argv[1], "r") )
        {
            while((c=getc(file))!=EOF)
            {
                if(c == 'L'|| c == 'R')
                {
                    temp[baboonCnt] = c;
                    baboonCnt++;
                }
            }
        }
        else
        {
            printf("Unable to read data from the input file.");
            return 0;
        }
        printf("The input is\n");
        int j=0;
        for(j;j<baboonCnt;++j)
        {
            printf("%c ",temp[j]);
        }
        printf("\n");
        int id[baboonCnt];
        pthread_t eastern[baboonCnt],western[baboonCnt];

        int eastcnt=0, westcnt=0, eastid[eastBab], westid[westBab], i=0;
        for(i;i<baboonCnt;++i)
        {
            sleep(1);
            if(temp[i]=='L')
            {
                eastid[eastcnt]=i;
                printf("Baboon %d wants to cross left to right\n",i);
                pthread_create(&eastern[eastcnt],NULL, (void *) &east_side,(void *) &eastid[eastcnt] );
                ++eastcnt;
            }
            else if(temp[i]=='R')
            {
                westid[westcnt]=i;
                printf("Baboon %d wants to cross right to left\n",i);
                pthread_create(&western[westcnt],NULL, (void *) &west_side,(void *) &westid[westcnt] );
                ++westcnt;
            }
        }

        int k =0;
        for(k;k<westBab;++k)
        {
            pthread_join(western[k],NULL);
        }
        k=0;
        for(k;k<eastBab;++k)
        {
            pthread_join(eastern[k],NULL);
        }

        //destroy all semaphores
        sem_destroy (&rope);
        sem_destroy (&east_mutex);
        sem_destroy (&west_mutex);
        sem_destroy (&deadlock_protection);
        sem_destroy (&counter);
        return 0;
    }
    else
    {
        printf("Proper command line usage is: \n<name> <filename> <cross time>\n");
    }
}
//thread handling the east to west to travel
void* east_side(void*arg)
{
    int baboon = *(int*)arg;
    int on_rope;
    sem_wait(&deadlock_protection);
    sem_wait(&east_mutex);
    east++;
    if (east == 1)
    {
        sem_wait(&rope);
        printf("Baboon %d: waiting\n", baboon);
    }
    sem_post(&east_mutex);
    sem_post(&deadlock_protection);
    sem_wait(&counter);
    sem_getvalue(&counter, &on_rope);
    printf("Baboon %d: Cross rope request granted (Current crossing: left to right, Number of baboons on rope: %d)\n", baboon,3-on_rope);
    sleep(travel_time);
    sem_getvalue(&counter, &on_rope);
    printf("Baboon %d: Exit rope (Current crossing: left to right, Number of baboons on rope: %d)\n", baboon, 2-on_rope);
    sem_post(&counter);
    sem_wait(&east_mutex);
    east--;
    if (east == 0)
        sem_post(&rope);
    sem_post(&east_mutex);
}

//thread handling west to east travel
void* west_side(void*arg)
{
    int baboon = *(int*)arg;
    int on_rope;
    sem_wait(&deadlock_protection);
    sem_wait(&west_mutex);
    west++;
    if (west == 1)
    {
        sem_wait(&rope);
        printf("Baboon %d: waiting\n", baboon);
    }
    sem_post(&west_mutex);
    sem_post(&deadlock_protection);
    sem_wait(&counter);
    sem_getvalue(&counter, &on_rope);
    printf("Baboon %d: Cross rope request granted (Current crossing: right to left, Number of baboons on rope: %d)\n", baboon, 3-on_rope);
    sleep(travel_time);
    sem_getvalue(&counter, &on_rope);
    printf("Baboon %d: Exit rope (Current crossing: right to left, Number of baboons on rope: %d)\n", baboon, 2-on_rope);
    sem_post(&counter);
    sem_wait(&west_mutex);
    west--;
    if (west == 0)
        sem_post(&rope);
    sem_post(&west_mutex);
}
