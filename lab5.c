//James Madden and Jason Graham

/* Reader/Writer implementation with writer priority
 * using condition variables
 * Author: Sherri Goings
 * Last Modified: 5/1/2016
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

void* Reader(void*);
void* Writer(void*);
void shuffle(int*, int);
void delay(int);

// initialize synchronization (locks and/or condition vars)
//Ex: pthread_mutex_t <lockVarName>;
//Ex: pthread_cond_t <condVarName>;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t read;
pthread_cond_t write;
// keep track of how many readers/writers are both active and waiting
int activeReaders = 0;
int activeWriters = 0;
int waitingWriters = 0;


int main() {
    /* Initialize mutex and condition variable objects */
    //ex: pthread_mutex_init(&<lockVarName>, NULL);
    //ex: pthread_cond_init(&<condVarName>, NULL);
    pthread_mutex_init(&lock, NULL);
    pthread_cond_init(&read, NULL);
    pthread_cond_init(&write, NULL);
    // create threads for testing here, R readers & W writers
    // you will want to mix up the order of creation for testing!
    int R = 5;
    pthread_t threads[R+1];


    // now create threads
    int i=0;

    for (i=0; i<R; i++)
            pthread_create(&threads[i], NULL, Reader, (void*) i);

    pthread_create(&threads[i], NULL, Writer, (void*) i);

    // join all threads before quitting
    for (i=0; i<R+1; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

/* Algorithm for reader
 * gives priority to writers, if any are currently writing or waiting,
 * reader will wait.
 * args - long id of this thread
 * return 0 on exit
 */
void* Reader(void* args) {
    delay(rand()%10);
    printf("reader %ld created\n", (long) args);
    fflush(stdout);

    //add synchronization such that:
    //  readers can only read if no active or waiting writers
    pthread_mutex_lock(&lock);
    if (activeWriters > 0) pthread_cond_wait(&read, &lock);

    activeReaders++;
    pthread_mutex_unlock(&lock);



    printf("reader %ld begin reading\n", (long) args);
    fflush(stdout);

    delay(rand()%10);

    printf("reader %ld finished reading\n", (long) args);
    fflush(stdout);

    // clean up after readings to ensure progress

    pthread_mutex_lock(&lock);
    activeReaders--;
    if(activeReaders==0)pthread_cond_signal(&write);
    pthread_mutex_unlock(&lock);

    return (void*) 0;
}

/* Algorithm for writer
 * args - long id of this thread
 * return 0 on exit
 */
void* Writer(void* args) {
    delay(rand()%10);
    printf("writer %ld created\n", (long) args);
    fflush(stdout);

    //add synchronization such that:
    //  writers can only write if no active readers
    pthread_mutex_lock(&lock);
    if(activeReaders>0) pthread_cond_wait(&write, &lock);
    activeWriters++;
    pthread_mutex_unlock(&lock);



    printf("writer %ld begin writing\n", (long) args);
    fflush(stdout);

    delay(rand()%10);

    printf("writer %ld finished writing\n", (long) args);
    fflush(stdout);

    // clean up synchronization to ensure progress

    pthread_mutex_lock(&lock);
    activeWriters--;
    pthread_cond_broadcast(&read);
    pthread_mutex_unlock(&lock);



    return (void*) 0;
}

/*
 * NOP function to simply use up CPU time
 * arg limit is number of times to run each loop, so runs limit^2 total loops
 */
void delay( int limit )
{
  int j, k;

  for( j=1; j < limit*1000; j++ )
    {
      for( k=1; k < limit*1000; k++ )
        {
            int x = j*k/(k+j);
            int y = x/j + k*x - (j+5)/k + (x*j*k);
        }
    }
}
