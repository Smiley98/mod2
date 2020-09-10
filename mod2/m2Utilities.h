#pragma once
#include <glm/glm.hpp>
#include <string>
#include <chrono>
#include <cstdio>

namespace m2Utils {
	std::string loadTextFile(const std::string& path);

	//Converts from (-1.0, 1.0) to (0.0, 1.0);
	float bias(float);
	//Converts from (0.0, 1.0) to (-1.0, 1.0);
	float unbias(float);

	float screenToNdcX(float value);
	float screenToNdcY(float value);
	float ndcToScreenX(float value);
	float ndcToScreenY(float value);
	//float screenToNdc(float value, float range, float offset = 0.0f);
	//float ndcToScreen(float value, float range, float offset = 0.0f);
	glm::vec2 screenToNdc(const glm::vec2&);
	glm::vec2 ndcToScreen(const glm::vec2&);

	float cross(const glm::vec2&, const glm::vec2&);

	void printMatrix(const glm::mat4& matrix);

	template<typename T = glm::vec3>
	void printVector(const T& vector) {
		for (unsigned i = 0; i < vector.length(); i++)
			printf("%f ", vector[i]);
		printf("\n");
	}

	//Benchmark timer. Don't use in more than once place otherwise data won't be useful.
	void elapsed(std::chrono::steady_clock::time_point start);
}