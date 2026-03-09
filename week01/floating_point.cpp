#include <limits>
#include <iostream>
#include <format>

int main() {
    const float a = 10.0f;

    unsigned char* p = (unsigned char*)&a;

    std::cout << std::format("{:08b}\n", *p);
    std::cout << std::format("{:08b}\n", *(p+1));
    std::cout << std::format("{:08b}\n", *(p+2));
    std::cout << std::format("{:08b}\n", *(p+3));

    return 0;
}

