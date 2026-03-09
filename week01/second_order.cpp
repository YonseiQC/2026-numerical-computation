#include <cmath>
#include <format>
#include <iostream>

int main() {
	// f(x) = ax^2 + 2bx + c = 0
	
	float a = 1.0;
	float b = 10000.0;
	float c = 1.0;

	float x0 = -b+std::sqrt(b*b-a*c);
	float x1 = -b-std::sqrt(b*b-a*c);

	std::cout << std::format("x0: {:.6f}\tx1: {:.6f}\n", x0, x1); // wrong..
	std::cout << std::format("x0: {:.6f}\tx1: {:.6f}\n", c/x1, x1); // close to true sol
	
	std::cout << std::format("f(x0): {:.6f}\n", a*x0*x0 + 2*b*x0 + c);
	std::cout << std::format("f(c/x1): {:.6f}\n", a*(c/x1)*(c/x1) + 2*b*(c/x1) + c);
	return 0;
}
