#include <format>
#include <iostream>
#include <mpi.h>
#include <numeric>
#include <vector>

int main(int argc, char* argv[]) {
	MPI_Init(&argc, &argv);

	int mpi_size;
	int mpi_rank;

	MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

	std::vector<double> data(100);
	if(mpi_rank == 0) {
		std::iota(std::begin(data), std::end(data), 0);
		// prepare data
	}
	MPI_Barrier(MPI_COMM_WORLD);

	// Before broadcasting
	std::cout << std::format("Data at rank = {}: [{}, {}, {}, ...]\n", mpi_rank,
	                         data[0], data[1], data[2]);

	// Broadcast from root = 0
	MPI_Bcast(data.data(), 100, MPI_DOUBLE, /* root = */ 0, MPI_COMM_WORLD);
	// After broadcasting
	std::cout << std::format("Data at rank = {}: [{}, {}, {}, ...]\n", mpi_rank,
	                         data[0], data[1], data[2]);

	MPI_Finalize();
	return 0;
}