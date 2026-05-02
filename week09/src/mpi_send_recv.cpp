#include <mpi.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
    // Initialize the MPI environment
    MPI_Init(&argc, &argv); // Consume MPI related arguments

    int mpi_size;
    int mpi_rank;

    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

    int number;
	if(mpi_rank == 0) {
		number = -1;
		MPI_Send(&number, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
	} else if(mpi_rank == 1) {
		MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("Process 1 received number %d from process 0\n", number);
	}
	MPI_Finalize();
}
