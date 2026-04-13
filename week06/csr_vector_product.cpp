#include <cstdint>
#include <format>
#include <iostream>
#include <vector>

struct CSRMatrix {
	uint32_t rows;
	uint32_t cols;
	std::vector<double> values;
	std::vector<uint32_t> col_indices;
	std::vector<uint32_t> row_indices;
};

std::vector<double> csr_vector_prod(const CSRMatrix& mat, std::vector<double>& vec) {
	const uint32_t rows = mat.rows;

	std::vector<double> res(rows, 0.0);
	for(uint32_t row = 0; row < rows; row++) {
		for(uint32_t val_idx = mat.row_indices[row]; val_idx < mat.row_indices[row+1]; val_idx++) {
			const uint32_t col = mat.col_indices[val_idx];
			const double value = mat.values[val_idx];
			res[row] += value * vec[col];
		}
	}
	return res;
}

int main() {
	std::vector<double> values{10, 20, 30, 40, 50, 60, 70, 80};
	std::vector<uint32_t> col_indices{0, 1, 1, 3, 2, 3, 4, 5};
	std::vector<uint32_t> row_indices{0, 2, 4, 7, 8};
	CSRMatrix mat{4, 5, values, col_indices, row_indices};

	std::vector<double> vec{1.0, 1.0, 1.0, 1.0, 1.0, 1.0};
	auto res = csr_vector_prod(mat, vec);

	for(auto val: res) {
		std::cout << std::format("{}\t", val);
	}
	std::cout << '\n';
	return 0;
}
