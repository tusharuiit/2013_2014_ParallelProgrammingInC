#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define NUM 10000000

void * increment (void * i_void_ptr)
{
  int * i = ( int *) i_void_ptr ;
  for ( int j =0; j < NUM; j++)
  (*i)++;
  return NULL;
}

int main ( )
{
  int i = 0;
  pthread_t thr ;
  pthread_create(&thr , NULL, &increment , &i ) ;
  //for ( int j =0; j < NUM; j++)
  //i++;
  pthread_join ( thr , NULL ) ;
  for ( int j =0; j < NUM; j++)
  i++;
  printf("Value of i = %d \n",i);
}


