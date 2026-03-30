#include "MyVector.hpp"
#include "MyMatrix.hpp"

template<Order order>
MyVectorDouble matrix_vector_prod_omp1(const MyMatrix<order>& mat, const MyVectorDouble& vec) {
    assert(mat.cols() == vec.size());

    MyVectorDouble res(mat.rows());
#pragma omp parallel for schedule(static, 8)
    for(size_t i = 0; i < mat.rows(); i++) {
        double val = 0.0;
        for(size_t j = 0; j < mat.cols(); j++) {
            val += mat(i,j) * vec[j];
        }
        res[i] = val;
    }

    return res;
}

template<Order order>
MyVectorDouble matrix_vector_prod_omp2(const MyMatrix<order>& mat, const MyVectorDouble& vec) {
    assert(mat.cols() == vec.size());

    MyVectorDouble res(mat.rows());
    for(size_t i = 0; i < mat.rows(); i++) {
        double val = 0.0;
#pragma omp parallel for schedule(static, 8) reduction(+:val)
        for(size_t j = 0; j < mat.cols(); j++) {
            val += mat(i,j) * vec[j];
        }
        res[i] = val;
    }

    return res;
}

// Explicit instantiation
template MyVectorDouble matrix_vector_prod_omp1<Order::RowMajor>(const MyMatrix<Order::RowMajor>&, const MyVectorDouble&);
template MyVectorDouble matrix_vector_prod_omp1<Order::ColMajor>(const MyMatrix<Order::ColMajor>&, const MyVectorDouble&);
template MyVectorDouble matrix_vector_prod_omp2<Order::RowMajor>(const MyMatrix<Order::RowMajor>&, const MyVectorDouble&);
template MyVectorDouble matrix_vector_prod_omp2<Order::ColMajor>(const MyMatrix<Order::ColMajor>&, const MyVectorDouble&);
