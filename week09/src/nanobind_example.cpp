#include <nanobind/nanobind.h>
#include <nanobind/stl/string.h>

#include <mpi.h>
#include <iomanip>
#include <cstdint>
#include <vector>

int add(int a, int b) { return a + b; }

// Singleton class
class MPIManager {
private:
    int mpi_rank_;
    int mpi_size_;

    MPIManager() {
        MPI_Comm_size(MPI_COMM_WORLD, &mpi_size_);
        MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank_);
    }
public:
    static MPIManager& get_instance() {
        static MPIManager instance;
        return instance; 
    }

    int mpi_rank() {
        return mpi_rank_;
    }

    int mpi_size() {
        return mpi_size_;
    }
};

std::string check_recv(int source, int tag) {
    MPI_Status status;
    MPI_Probe(source, tag, MPI_COMM_WORLD, &status);
    int msg_size;
    MPI_Get_count(&status, MPI_UINT8_T, &msg_size);
    std::vector<std::uint8_t> data(msg_size);

    MPI_Recv(data.data(), msg_size, MPI_UINT8_T, source, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // Converting recived data to hex string
	std::stringstream ss;
	ss << std::hex << std::setfill('0');
	for(const auto& c: data) {
		ss << std::hex << std::setw(2) << static_cast<int>(c);
	}
	return ss.str();
}

// The module name must be same as that defined in `CMakeLists.txt`
NB_MODULE(nanobind_example, m) {
    m.def("add", &add);
    m.def("get_mpi_size", []() {
        return MPIManager::get_instance().mpi_size();
    });
    m.def("get_mpi_rank", []() {
        return MPIManager::get_instance().mpi_rank();
    });
    m.def("check_recv", &check_recv);
}
