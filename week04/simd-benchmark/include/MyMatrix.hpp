#include "MyVector.hpp"

#include <cstddef>
#include <random>

enum class Order {RowMajor, ColMajor};

template<Order order_>
struct MyMatrix {
    constexpr static Order order = order_;
    size_t rows_;
    size_t cols_;
    double* data_;

    MyMatrix(size_t rows, size_t cols) : rows_{rows}, cols_{cols} {
        data_ = new double[rows*cols];
    }

    inline double& operator()(size_t i, size_t j) {
        if constexpr (order == Order::RowMajor) {
            return data_[i*cols_ + j];
        } else {
            return data_[j*rows_ + i];
        }
    }

    inline const double& operator()(size_t i, size_t j) const {
        if constexpr (order == Order::RowMajor) {
            return data_[i*cols_ + j];
        } else {
            return data_[j*rows_ + i];
        }
    }

    size_t rows() const {
        return rows_;
    }

    size_t cols() const {
        return cols_;
    }

    ~MyMatrix() {
        delete[] data_;
    }
};

template<Order order, typename RandomEngine>
MyMatrix<order> random_matrix(RandomEngine& re, size_t rows, size_t cols) {
    MyMatrix<order> mat(rows, cols);
    std::normal_distribution ndist;
    for(size_t i = 0; i < rows; i++) {
        for(size_t j = 0; j < cols; j++) {
            mat(i, j) = ndist(re);
        }
    }
    return mat;
}

template<Order order>
MyVectorDouble matrix_vector_prod(const MyMatrix<order>& mat, const MyVectorDouble& vec) {
    assert(mat.cols() == vec.size());

    MyVectorDouble res(mat.rows());
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
MyVectorDouble matrix_vector_prod_omp1(const MyMatrix<order>& mat, const MyVectorDouble& vec);

template<Order order>
MyVectorDouble matrix_vector_prod_omp2(const MyMatrix<order>& mat, const MyVectorDouble& vec);

// Explicit instantiation
extern template MyVectorDouble matrix_vector_prod_omp1<Order::RowMajor>(const MyMatrix<Order::RowMajor>&, const MyVectorDouble&);
extern template MyVectorDouble matrix_vector_prod_omp1<Order::ColMajor>(const MyMatrix<Order::ColMajor>&, const MyVectorDouble&);
extern template MyVectorDouble matrix_vector_prod_omp2<Order::RowMajor>(const MyMatrix<Order::RowMajor>&, const MyVectorDouble&);
extern template MyVectorDouble matrix_vector_prod_omp2<Order::ColMajor>(const MyMatrix<Order::ColMajor>&, const MyVectorDouble&);
