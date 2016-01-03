#include<mpi.h>
#include<stdio.h>
#include<math.h>

int main ( int argc , char * argv[] )
{
  int rank , size , tmp;
  int var = 0 ; 
  MPI_Status status;

  MPI_Init(&argc , &argv ) ; /* s t a r t s MPI */
  MPI_Comm_rank(MPI_COMM_WORLD, &rank ) ; /* process id */
  MPI_Comm_size(MPI_COMM_WORLD, &size ) ; /* number processes */
  
  printf( " rank %d var %d size %d \n" , rank , var , size ) ;

  MPI_Barrier(MPI_COMM_WORLD);
  printf( " \n BARRIER \n"  ) ;
  MPI_Barrier(MPI_COMM_WORLD);
  if ( rank == 1 )
  {var = 5 ;}  
  if(rank == 1){
  MPI_Send(&var , 1 , MPI_INT , 3 , 0 ,MPI_COMM_WORLD);
  }

  //printf( " i love maths  %d of %d of %d \n" , rank , tmp , size ) ;
  
  if(rank == 3){
  MPI_Recv(&var , 1 , MPI_INT , 1 , 0 , MPI_COMM_WORLD , &status);
  }

    printf( " rank %d var %d size %d \n" , rank , var , size ) ;
  
  MPI_Finalize() ;
  return 0;
}

