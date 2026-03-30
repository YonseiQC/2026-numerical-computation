#include <iostream>
#include <vector>
#include <numeric>
#include <format>

int main() {
    std::vector<double> data(10000);
    std::iota(data.begin(), data.end(), 0);
    
    double res = 0.0;
    #pragma omp parallel for reduction(+:res)
    for(size_t i = 0; i < data.size(); i++) {
        res += data[i];
    }
    std::cout << std::format("Result: {}\n", res);
    return 0;
}