#include "m2Utilities.h"
#include <fstream>

void m2Utils::printMatrix(const glm::mat4 & matrix)
{
	for (int i = 0; i < 4; i++)
		printf("%f %f %f %f\n", matrix[0][i], matrix[1][i], matrix[2][i], matrix[3][i]);
	printf("\n");
}

std::string m2Utils::loadTextFile(const std::string& path) {
	std::ifstream file(path);
	return std::string(
		(std::istreambuf_iterator<char>(file)),
		std::istreambuf_iterator<char>()
	);
}

float m2Utils::cross(const glm::vec2& a, const glm::vec2& b)
{	//(_x * point2._y) - (_y * point2._x)
	return a.x * b.y - a.y * b.x;
}
