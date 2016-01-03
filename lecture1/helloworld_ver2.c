#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void * hello(void * ptr)
{
   printf("hello World from pthread %d ! \n" , *((int *) ptr));
   //int *arg = ptr ;printf("hello World from pthread %d ! \n" , *arg); 
   //printf(" i love you 1 %d \t " , *((int *) ptr));
   return NULL ;
}

int main ()
{
  int num_threads = 3 ; pthread_t * thread ; int *thread_arg ;  
  thread = malloc(num_threads * sizeof(*thread)) ;
  thread_arg = malloc(num_threads * sizeof(*thread_arg)) ; 
  for (int i = 0 ; i < num_threads ; i++)
  {
     thread_arg[i] = i ;
     pthread_create(thread+i, NULL , &hello , thread_arg + i );
  }
  
  for (int i = 0 ; i < num_threads ; i++)
     pthread_join(thread[i] , NULL);
  //printf("Hello World from main ! \n ");
  //printf(" i love you 1 \n ");
  //printf(" i love you 2 \n ");
}
