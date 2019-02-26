#pragma once
#include <glm/glm.hpp>

struct m2Line2
{
	m2Line2(const glm::vec2&, const glm::vec2&);
	m2Line2();
	~m2Line2();

	bool intersect(const m2Line2&, const m2Line2&, glm::vec2& poi);

	const glm::vec2 p1;
	const glm::vec2 p2;
};

