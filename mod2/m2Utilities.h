#pragma once
#include <glm/glm.hpp>
#include <cstdio>
namespace Utils {
	void printMatrix(glm::mat4& matrix);

	template<typename T = glm::vec3>
	void printVector(T& vector) {
		for (unsigned i = 0; i < vector.length(); i++)
			printf("%f ", vector[i]);
		printf("\n");
	}
}