#include <iostream>
#include <vector>
#include <numeric>
#include <format>

int main() {
    std::vector<double> data(10000);
    std::iota(data.begin(), data.end(), 0);
    
    double res = 0.0;
    #pragma omp parallel shared(res)
    {
        double res_local = 0.0;
        #pragma omp for
        for(size_t i = 0; i < data.size(); i++) {
            res_local += data[i];
        }

        #pragma omp critical
        {
            res += res_local;
        }
    }
    std::cout << std::format("Result: {}\n", res);
    return 0;
}