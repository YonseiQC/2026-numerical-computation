from mpi4py import MPI
import nanobind_example as ne

comm = MPI.COMM_WORLD
rank = comm.Get_rank()

if rank == 0:
    data = {'a': 7, 'b': 3.14}
    comm.send(data, dest=1, tag=11)
elif rank == 1:
    # data = comm.recv(source=0, tag=11)
    print(ne.get_mpi_rank()) # print 1
    print(ne.check_recv(0, 11))