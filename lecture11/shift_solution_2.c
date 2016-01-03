#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>

main(int argc, char *argv[]) {
	int myid, np, ierr, lnbr, rnbr, shifts, i, j;
	int *values, *all_values;
	MPI_Status status[2];
	MPI_Request request[2];

	ierr = MPI_Init(&argc, &argv);
	if (ierr != MPI_SUCCESS) {
		printf("Cannot initialize MPI!\n");
		MPI_Finalize();
		exit(0);
	}

	MPI_Comm_size(MPI_COMM_WORLD, &np);
	MPI_Comm_rank(MPI_COMM_WORLD, &myid);
	if (100 % np != 0) {
		if (myid == 0) {
			printf("Error: cannot execute with %d processors!\n", np);
		}
		MPI_Finalize();
		exit(0);
	}

	if (myid == 0) {
		lnbr = np - 1;
		rnbr = myid + 1;
	} else if (myid == np - 1) {
		lnbr = myid - 1;
		rnbr = 0;
	} else {
		lnbr = myid - 1;
		rnbr = myid + 1;
	}

	shifts = atoi(argv[1]);
	values = (int *) calloc(100 / np, sizeof(int));

	if (myid == 0) {
		values[0] = 1;
	}

	for (i = 0; i < shifts; i++) {
		int buf = values[0];
		int buf_recv;

		MPI_Isend(&buf, 1, MPI_INT, lnbr, 10, MPI_COMM_WORLD, &request[0]);
		MPI_Irecv(&buf_recv, 1, MPI_INT, rnbr, 10, MPI_COMM_WORLD, &request[1]);
        
        //MPI_Isend(&values[0], 1, MPI_INT, lnbr, 10, MPI_COMM_WORLD, &request[0]);
		//MPI_Irecv(&values[100 / np - 1], 1, MPI_INT, rnbr, 10, MPI_COMM_WORLD, &request[1]);
		
		for (j = 1; j < 100 / np; j++) {
			values[j - 1] = values[j];
		}

		MPI_Waitall(2, request, status);
		values[100 / np - 1] = buf_recv;
	}

	if (myid == 0) {
		all_values = (int *) calloc(100, sizeof(int));
	}

	MPI_Gather(values, 100 / np, MPI_INT, all_values, 100 / np, MPI_INT, 0, MPI_COMM_WORLD);

	if (myid == 0) {
		for (i = 0; i < 100; i++)
			printf(" %2d:%4d\n", i, all_values[i]);
	}

	MPI_Finalize();
}
