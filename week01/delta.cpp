#include <limits>
#include <iostream>
#include <format>
int main() {
    const float a = 1.0f;
    float b = 0x1p-24+0x1p-25;

    float c = a+b;

    unsigned char* p = (unsigned char*)&c;

    std::cout << std::format("{:08b}\n", *p);
    std::cout << std::format("{:08b}\n", *(p+1));
    std::cout << std::format("{:08b}\n", *(p+2));
    std::cout << std::format("{:08b}\n", *(p+3));

    return a == (a+b);
}

