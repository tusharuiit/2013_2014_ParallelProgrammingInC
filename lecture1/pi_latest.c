#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
//#include <math.h>
//#include <sys/types.h>
#include <unistd.h>
#include <linux/unistd.h>

struct pthread_args
{
    long thread_id ;
    long num_threads ;
};

pid_t gettid() { return (pid_t) syscall(__NR_gettid); }

void * hello(void * ptr)
{
   struct pthread_args *arg = ptr ; 
   printf("hello World from pthread %ld of %ld PID = %d TID = %d \n" , arg->thread_id , arg->num_threads , getpid() , gettid());
   return NULL ;
}

int main()
{
  long num_threads = 3 ; pthread_t * thread ; struct pthread_args *thread_arg ;  
  thread = malloc(num_threads * sizeof(*thread)) ;
  thread_arg = malloc(num_threads * sizeof(*thread_arg)) ; 
  
  for (int i = 0 ; i < num_threads ; i++)
  {
     thread_arg[i].thread_id = i ;
     thread_arg[i].num_threads = num_threads ;
     pthread_create(thread+i, NULL , &hello , thread_arg + i );
  }
  
  for (int i = 0 ; i < num_threads ; i++)
     pthread_join(thread[i] , NULL);
  //printf("Hello World from main ! \n ");
  //printf(" i love you 1 \n ");
  //printf(" i love you 2 \n ");
}
