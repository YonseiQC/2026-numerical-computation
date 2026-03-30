#include <iostream>
#include <vector>
#include <numeric>
#include <format>
#include <thread>
#include <mutex>

std::mutex print_mutex;

constexpr int NUM_THREADS = 4;
struct ThreadArgs {
    int id;
    const std::vector<double>& data;
    double& res;
};

void thread_func(ThreadArgs& args) {
    {
        std::lock_guard lock(print_mutex);
        std::cout << "Summing in thread " << args.id << "\n";
    }

    double res_local = 0.0;
    for(size_t i = args.id; i < args.data.size(); i+=NUM_THREADS) {
        res_local += args.data[i];
    }
    args.res = res_local;
}

int main() {
    std::vector<double> data(10000);
    std::iota(data.begin(), data.end(), 0);

    double res[NUM_THREADS];
    std::vector<ThreadArgs> args;
    for(int i = 0; i < NUM_THREADS; i++) {
        args.emplace_back(
            /* id = */ i,
            /* data = */ data,
            /* res = */ res[i]);
    }

    std::vector<std::thread> threads;
    for (int i = 0; i < NUM_THREADS; i++) {
        threads.emplace_back(thread_func, std::ref(args[i]));
    }
    for (auto& t : threads) {
        t.join();
    }

    std::cout << std::format("Result: {}\n", res[0] + res[1] + res[2] + res[3]);
    return 0;
}