#include <mpi.h>
#include <iostream>
#include <random>
#include <format>

int main(int argc, char* argv[]) {
	// Initialize the MPI environment
	MPI_Init(&argc, &argv); // Consume MPI related arguments

	int mpi_size;
	int mpi_rank;

	MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
	MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

	constexpr size_t elements_per_proc = 100;

	std::vector<double> random_numbers;
	if(mpi_rank == 0) {
		random_numbers.reserve(elements_per_proc * mpi_size);
		std::mt19937_64 re{1557};
		std::uniform_real_distribution<double> dist(0.0, 1.0);
		for(size_t i = 0; i < elements_per_proc * mpi_size; i++) {
			random_numbers.emplace_back(dist(re));
		}
	}

	// Create a buffer that will hold a subset of the random numbers
	std::vector<double> sub_random_numbers(elements_per_proc, 0.0);

	// Scatter the random numbers to all processes
	MPI_Scatter(random_numbers.data(), elements_per_proc, MPI_DOUBLE,
	            sub_random_numbers.data(), elements_per_proc, MPI_DOUBLE, 0,
	            MPI_COMM_WORLD);

	// Compute the average of your subset
	double sub_avg = std::accumulate(std::begin(sub_random_numbers),
	                                 std::end(sub_random_numbers), 0.0)
	                 / sub_random_numbers.size();
	// Gather all partial averages down to the root process
	std::vector<double> sub_avgs;
	double* recv_data = nullptr;
	if(mpi_rank == 0) {
		sub_avgs.resize(mpi_size);
		recv_data = sub_avgs.data();
	}
	MPI_Gather(/* send_data = */ &sub_avg,
	           /* send_count =*/1,
	           /* send_datatype = */ MPI_DOUBLE,
	           /* recv_data = */ recv_data,
	           /* recv_count = */ 1,
	           /* recv_datatype = */ MPI_DOUBLE,
	           /* root = */ 0, MPI_COMM_WORLD);

	// Compute the total average of all numbers.
	if(mpi_rank == 0) {
		double avg
			= std::accumulate(std::begin(sub_avgs), std::end(sub_avgs), 0.0)
		      / sub_avgs.size();
		std::cout << std::format("Average of {} random numbers = {}\n",
		                         mpi_size * elements_per_proc, avg);
	}

	// Finalize the MPI environment.
	MPI_Finalize();
}