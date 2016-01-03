#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>


#define NUM 10000000
pthread_spinlock_t spinlock ;
void * increment ( void * i_void_ptr )
{
int * i = ( int *) i_void_ptr ;
for ( int j =0; j < NUM; j++)
{
  pthread_spin_lock(&spinlock ) ;
  ( * i )++;
  pthread_spin_unlock(&spinlock ) ;
}
return NULL;
}

int main ( )
{
  int i = 0; pthread_t thr ;
  pthread_spin_init(&spinlock , PTHREAD_PROCESS_PRIVATE ) ;
  pthread_create(&thr , NULL, &increment , &i ) ;
  for ( int j =0; j < NUM; j++)
{
  pthread_spin_lock(&spinlock ) ;
  i++;
  pthread_spin_unlock(&spinlock ) ;
}
 pthread_join ( thr , NULL ) ;
 printf("Value of i = %d \n" , i ) ;
}
