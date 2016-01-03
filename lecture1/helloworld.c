#include <stdio.h>
#include <pthread.h>

void * hello()
{
   printf("hello World from pthread! \n");
   return NULL ;
}

int main ()
{
  pthread_t thread ; 
  pthread_create(&thread , NULL , &hello , NULL );
  printf("Hello World from main ! \n ");
  printf(" i love you 1 \n ");
  pthread_join(thread , NULL);
  printf(" i love you 2 \n ");
}
