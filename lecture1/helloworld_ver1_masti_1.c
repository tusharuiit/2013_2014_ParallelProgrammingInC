#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void * hello()
{
   printf("hello World from pthread! \n");
   return NULL ;
}

void * hello1()
{
   printf("hello World from time pass thread  \n");
   return NULL ;
}

int main()
{
  long num_threads = 3 ; pthread_t * thread ; 
  thread = malloc (num_threads * sizeof(*thread)) ; 
  for (int i = 0 ; i < 2 ; i++)
     pthread_create(thread+i, NULL , &hello , NULL );
  for (int i = 2 ; i < num_threads ; i++)
     pthread_create(thread+i, NULL , &hello1 , NULL );   
  for (int i = 0 ; i < num_threads ; i++)
     pthread_join(thread[i] , NULL);
  //printf("Hello World from main ! \n ");
  //printf(" i love you 1 \n ");
  //printf(" i love you 2 \n ");
}
