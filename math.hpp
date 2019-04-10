#pragma once

#include <iostream>

namespace Cardiography {

	using namespace std;

	// Sigmoid
	double sigmoid(double input) {
		return 1 / (1 + exp(-input));
	}

	// Derivate of Sigmoid
	double dsigmoid(double input) {
		return (1 - input) * input;
	}
}