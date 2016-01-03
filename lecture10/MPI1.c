#include<mpi.h>
#include<stdio.h>
#include<math.h>

int main ( int argc , char * argv[] )
{
  int rank , size , tmp;
  MPI_Init(&argc , &argv ) ; /* s t a r t s MPI */
  MPI_Comm_rank(MPI_COMM_WORLD, &rank ) ; /* process id */
  MPI_Comm_size(MPI_COMM_WORLD, &size ) ; /* number processes */
  MPI_Send(&rank , 1 , MPI_INT , ( rank+1)%size , 0 ,MPI_COMM_WORLD);
  //printf( " i love maths  %d of %d of %d \n" , rank , tmp , size ) ;
  MPI_Recv(&tmp , 1 , MPI_INT , (abs(rank-1))%size , 0 , MPI_COMM_WORLD , MPI_STATUS_IGNORE);
  printf( " i love myself  %d of %d of %d \n" , rank , tmp , size ) ;
  
  MPI_Finalize() ;
  return 0;
}

