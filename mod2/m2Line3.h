#pragma once
#include <glm/glm.hpp>

struct m2Line3
{
	m2Line3(const glm::vec3&, const glm::vec3&);
	m2Line3();
	~m2Line3();

	const glm::vec3 p1;
	const glm::vec3 p2;
};

