#include "m2Utilities.h"

void Utils::printMatrix(glm::mat4 & matrix)
{
	for (int i = 0; i < 4; i++)
		printf("%f %f %f %f\n", matrix[0][i], matrix[1][i], matrix[2][i], matrix[3][i]);
}