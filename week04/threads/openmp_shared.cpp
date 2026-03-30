#include <iostream>
#include <omp.h>

int main() {
    int counter = 0;

    // BAD: race condition — multiple threads write to counter simultaneously
    #pragma omp parallel for shared(counter) num_threads(4)
    for (int i = 0; i < 100; i++) {
        counter++;  // not atomic!
    }
    std::cout << "Race condition result:  " << counter << " (expected 100)\n";

    return 0;
}
