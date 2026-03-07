#include <iostream>
#include <format>
#include <cmath>
int main() {

	float js[11];
	js[0] = std::cyl_bessel_j(0, 1.0f);
	js[1] = std::cyl_bessel_j(1, 1.0f);

	for(int m = 1; m <=10; m++) {
		js[m+1] = 2*m*js[m] - js[m-1];
	}


	for(int m = 0; m <= 10; m++) {
		float true_val = std::cyl_bessel_j(m, 1.0f);
		std::cout << std::format("{}\t{}\t{}\n", m, true_val, js[m]);
	}
	return 0;
	
}
