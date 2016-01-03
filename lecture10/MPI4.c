#include<mpi.h>
#include<stdio.h>
#include<math.h>

int main ( int argc , char * argv[])
{
  int rank , size , tmp;

  MPI_Init(&argc , &argv ) ; /* starts MPI */
  MPI_Comm_rank(MPI_COMM_WORLD, &rank ) ; /* process id */
  MPI_Comm_size(MPI_COMM_WORLD, &size ) ; /* number processes */
  
if (rank != 0)
{
  //MPI_Send(&rank , 1 , MPI_INT , ( rank+1)%size , 0 ,MPI_COMM_WORLD) ;
  MPI_Recv(&tmp , 1 , MPI_INT , (abs(rank-1))%size , 0 , MPI_COMM_WORLD , MPI_STATUS_IGNORE);
  printf( " i love maths  %d of %d \n" , tmp , size ) ;
}
else
{ 
  //MPI_Recv(&tmp , 1 , MPI_INT , (abs(rank-1))%size , 0 , MPI_COMM_WORLD , MPI_STATUS_IGNORE);
  MPI_Send(&rank , 1 , MPI_INT , ( rank+1)%size , 0 , MPI_COMM_WORLD) ;
  //printf( " i love maths  %d of %d of %d \n" , rank , tmp , size ) ;
  printf( " i love all  %d of %d \n" , rank , size ) ; 
}

  MPI_Finalize ( ) ;
  return 0;
}

