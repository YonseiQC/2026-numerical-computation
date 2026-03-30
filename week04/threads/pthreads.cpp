#include <iostream>
#include <vector>
#include <numeric>
#include <format>
#include <pthread.h>

pthread_mutex_t print_mutex;

constexpr int NUM_THREADS = 4;
struct ThreadArgs {
    int id;
    const std::vector<double>& data;
    double& res;
};

void* thread_func(void* arg) {
    ThreadArgs* args = static_cast<ThreadArgs*>(arg);
    pthread_mutex_lock(&print_mutex);
    std::cout << "Summing in thread " << args->id << "\n";
    pthread_mutex_unlock(&print_mutex);

    double res_local = 0.0;
    for(size_t i = args->id; i < args->data.size(); i+=NUM_THREADS) {
        res_local += args->data[i];
    }
    args->res = res_local;
    return nullptr;
}

int main() {
    pthread_t threads[NUM_THREADS];

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
    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&threads[i], nullptr, thread_func, &args[i]) != 0) {
            std::cerr << "Failed to create thread " << i << "\n";
            return 1;
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], nullptr);
    }

    std::cout << std::format("Result: {}\n", res[0] + res[1] + res[2] + res[3]);
    return 0;
}
