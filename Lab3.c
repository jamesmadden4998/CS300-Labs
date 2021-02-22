//Lab3 Part 2 Code for Jason Graham and James Madden

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *print_message_function( void *ptr );

int main() {
     pthread_t thread1, thread2;
     char *message1 = "Thread 1";
     char *message2 = "Thread 2";
     int  iret1, iret2;

    /* Create a threads which will execute function print_message_function with message 1 */
    pthread_create(&thread1, NULL, print_message_function, message1);
    /* Create a threads which will execute function print_message_function with message 2 */
    pthread_create(&thread2, NULL, print_message_function, message2);
     /* Wait till threads are complete before main continues. Unless we  */
     /* wait we run the risk of executing an exit which will terminate   */
     /* the process and all threads before the threads have completed.   */
     pthread_join(thread1, thread2);
     //wait for thread 1
     printf("Thread 1 returns ");

     //wait for thread 2
     printf("Thread 2 returns\n");
     exit(0);
}

void *print_message_function( void *ptr ){
     char *message;
     message = (char *) ptr;
     printf("%s \n", message);
     pthread_exit(0);
}
