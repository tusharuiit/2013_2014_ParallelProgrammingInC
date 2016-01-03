#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

#define NUM 10000000
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER ;
void * increment (void * i_void_ptr)
{
  int * i = ( int * ) i_void_ptr ;
  for ( int j =0; j < NUM; j++)
  {
    pthread_mutex_lock(&mutex ) ;
    ( * i )++;
    //if ((*i) == 10000000) {printf(" i love me  \t" );}
    pthread_mutex_unlock(&mutex ) ;
  } 
  return NULL;
}

int main ()
{
int i = 0; pthread_t thr ;
pthread_create(&thr , NULL, &increment , &i ) ;
for ( int j =0; j < NUM; j++)
{
  pthread_mutex_lock(&mutex ) ;
  i++;
  if (i == 10000000) {printf(" i love me  \t \t" );}
  pthread_mutex_unlock(&mutex ) ;
}
pthread_join ( thr , NULL );
printf("Value of i = %d \n" , i);
}

