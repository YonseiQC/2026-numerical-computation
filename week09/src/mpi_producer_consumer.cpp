#include <mpi.h>
#include <format>
#include <random>
#include <vector>

int mpi_size;
int mpi_rank;

constexpr size_t length_per_chunk = 100;

double producer() {
	// Divide 1,000,000 random numbers into chunks. Each chunk's length is 100.
	const size_t total_length = 1'000'000;
	std::mt19937_64 re{1557};
	std::vector<double> vec;
	vec.reserve(total_length);
	std::uniform_real_distribution<double> dist(0.0, 1.0);

	for(size_t i = 0; i < total_length; i++) {
		vec.emplace_back(dist(re));
	}

	int rank_to_send = 1;
	for(size_t i = 0; i < total_length; i+=100) {
		std::cout << std::format("Sending chunk data to rank = {}\n", rank_to_send);
		MPI_Send(vec.data() + i, 100, MPI_DOUBLE, rank_to_send, 0, MPI_COMM_WORLD);
		rank_to_send = ((rank_to_send + 1) % mpi_size) == 0?1:(rank_to_send+1);
	}
	for(int rank = 1; rank < mpi_size; rank++) {
		const uint32_t op = 1;
		std::cout << std::format("Sending finish op to rank = {}\n", rank);
		MPI_Send(&op, 1, MPI_UNSIGNED, rank, /* tag = */1, MPI_COMM_WORLD);
	}
	double sum = 0.0;
	for(int rank = 1; rank < mpi_size; rank++) {
		double local_sum;
		std::cout << std::format("Recieving the result from rank = {}\n", rank);
		MPI_Recv(&local_sum, 1, MPI_DOUBLE, rank, /* tag = */2, MPI_COMM_WORLD, nullptr);
		sum += local_sum;
	}
	return sum;
}

void consumer() {
	std::vector<double> buf(length_per_chunk);
	uint32_t op;
	double local_sum = 0.0;
	MPI_Status status;
	while(true) {
		MPI_Probe(0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
		if(status.MPI_TAG == 1) { 
			// Operation message... We still need to recv
			MPI_Recv(&op, 1, MPI_UNSIGNED, /* source = */ 0, /* tag = */ 1, MPI_COMM_WORLD, nullptr);
			std::cout << std::format("Recieved operation, mpi_rank = {}\n", mpi_rank);
			if(op == 1) { // Finish
				break;
			}
		} else {
			// We use a fixed size, but one can use a dynamic size too.
			MPI_Recv(buf.data(), length_per_chunk, MPI_DOUBLE, /* source = */ 0, /* tag = */ 0, MPI_COMM_WORLD, nullptr);
			std::cout << std::format("Recieved chunk data, mpi_rank = {}\n", mpi_rank);
			local_sum += std::accumulate(std::begin(buf), std::end(buf), 0.0);
		}
	}

	std::cout << std::format("Sending local summation, mpi_rank = {}\n", mpi_rank);
	MPI_Send(&local_sum, 1, MPI_DOUBLE, 0, /* tag = */2, MPI_COMM_WORLD); // use tag == 2 to send the final result
}

int main(int argc, char* argv[]) {
    // Initialize the MPI environment
    MPI_Init(&argc, &argv); // Consume MPI related arguments

    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

	if(mpi_rank == 0) {
		double sum = producer();
		std::cout << std::format("Total sum: {}\n", sum);
	} else {
		consumer();
	}
	MPI_Finalize();
}
