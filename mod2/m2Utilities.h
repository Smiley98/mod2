#pragma once
#include <glm/glm.hpp>
#include <string>
#include <cstdio>

namespace m2Utils {
	std::string loadTextFile(const std::string& path);
	void printMatrix(const glm::mat4& matrix);

	template<typename T = glm::vec3>
	void printVector(const T& vector) {
		for (unsigned i = 0; i < vector.length(); i++)
			printf("%f ", vector[i]);
		printf("\n");
	}
}