#include "m2Utilities.h"
#include "m2Window.h"
#include <fstream>

static m2Window& window = m2Window::instance();

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

float m2Utils::bias(float value)
{
	return value * 0.5f + 0.5f;
}

float m2Utils::unbias(float value)
{
	return value * 2.0f - 1.0f;
}

float m2Utils::screenToNdcX(float value)
{
	return (value - window.getClientX()) / (float)window.getClientWidth() * 0.5f - 1.0f;
}

float m2Utils::screenToNdcY(float value)
{
	return (value - window.getClientY()) / (float)window.getClientHeight() * 0.5f - 1.0f;
}

float m2Utils::ndcToScreenX(float value)
{
	return (value + 1.0f) * (float)window.getClientWidth() * 0.5f + window.getClientX();
}

float m2Utils::ndcToScreenY(float value)
{
	return (value + 1.0f) * (float)window.getClientHeight() * 0.5f + window.getClientY();
}

/*float m2Utils::screenToNdc(float value, float range, float offset)
{
	return (value - offset) / range * 0.5f - 1.0f;
}

float m2Utils::ndcToScreen(float value, float range, float offset)
{
	return (value + 1.0f) * range * 0.5f + offset;
}*/

glm::vec2 m2Utils::screenToNdc(const glm::vec2& coordinates)
{
	return glm::vec2(
		(coordinates.x - window.getClientX()) / (float)window.getClientWidth() * 0.5f - 1.0f,
		(coordinates.y - window.getClientY()) / (float)window.getClientHeight() * 0.5f - 1.0f
	);
}

glm::vec2 m2Utils::ndcToScreen(const glm::vec2& coordinates)
{
	return glm::vec2(
		(coordinates.x + 1.0f) * (float)window.getClientWidth() * 0.5f + window.getClientX(),
		(coordinates.y + 1.0f) * (float)window.getClientHeight() * 0.5f + window.getClientY()
	);
}

float m2Utils::cross(const glm::vec2& a, const glm::vec2& b)
{
	return a.x * b.y - a.y * b.x;
}
