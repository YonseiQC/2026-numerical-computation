#include <cmath>
#include <iostream>
#include <format>

int main() {

	double y = std::log(11) - std::log(10);
	for(int n = 1; n <= 30; n++) {
		y = 1.0/n - 10*y;
		std::cout << std::format("y_{}: {:9f}\n", n, y);
	}
	return 0;
}
