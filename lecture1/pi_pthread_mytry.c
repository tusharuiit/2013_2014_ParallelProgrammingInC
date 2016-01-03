#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
//#include <math.h>
//#include <sys/types.h>
#include <math.h>
#include <unistd.h>
#include <linux/unistd.h>


#define STEPS 1000000

pid_t gettid() { return (pid_t) syscall(__NR_gettid); }

struct pthread_args
{
    double low ;
    double high ;
    double localsum ;
};

void * hello(void * ptr)
{
   double step_size = 1.0/STEPS;
   struct pthread_args *arg = ptr ;
   double low = arg->low ;double high = arg->high ; double tsum = 0.0 ; 
   while ( (low) < (high) )
   {
      tsum += sqrt(1.0- low * low) * step_size;
      low+= step_size ;  
   }
   printf("local sum from pthread %lf of %lf PID = %d TID = %d is %lf \n" , low , high , getpid() , gettid() , tsum);
   arg->localsum = tsum ; 
   return NULL ;
}

int main()
{
  
  long num_threads = 4 ; pthread_t * thread ; struct pthread_args *thread_arg ; double sum = 0.0 ; 
  thread = malloc(num_threads * sizeof(*thread)) ;
  thread_arg = malloc(num_threads * sizeof(*thread_arg)) ; 
  
  for (int i = 0 ; i < num_threads ; i++)
  {
     thread_arg[i].low = ( i*(1.0/(double)num_threads) ) ;
     thread_arg[i].high = ( (i+1)*(1.0/(double)num_threads) ) ;
     pthread_create(thread+i, NULL , &hello , thread_arg+i );
  }
  
  /*for (int i = 0 ; i < num_threads ; i++)
  {
     sum = sum + thread_arg[i].localsum ;
  }*/
  
  for (int i = 0 ; i < num_threads ; i++)
  {
     pthread_join(thread[i] , NULL);
     sum = sum + thread_arg[i].localsum ;
  }
  sum = sum * 4 ; 
  //printf("time pass wasting life = %.10lf Computed PI = %.10lf\n", thread_arg[0].localsum, sum);
  printf("Reference PI = %.10lf Computed PI = %.10lf\n", M_PI, sum);
  printf("Difference to Reference is %.10lf\n", M_PI - sum);
}
