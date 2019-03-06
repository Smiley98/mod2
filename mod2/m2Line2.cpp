#include "m2Line2.h"
#include "m2Utilities.h"

m2Line2::m2Line2(const glm::vec2& a, const glm::vec2& b) : p1(a), p2(b)
{
}

m2Line2::m2Line2()
{
}

m2Line2::~m2Line2()
{
}

bool m2Line2::intersect(const m2Line2& a, const m2Line2& b, glm::vec2 & poi)
{
	using namespace m2Utils;
	//r and s 
	glm::vec2 r(a.p2.x - a.p1.x, a.p2.y - a.p2.y);
	glm::vec2 s(b.p2.x - b.p1.x, b.p2.y - b.p2.y);
	
	float d = cross(r, s);

	//u and t are scalar values of parameterics
	float u = (cross(b.p1, r) - cross(a.p1, r)) / d;
	float t = (cross(b.p1, s) - cross(a.p1, s)) / d;

	if ((0 <= u) && (u <= 1) && (0 <= t) && (t <= 1)) {
		poi = glm::vec2(r.x * t + a.p1.x, r.y * t + a.p1.y);
		return true;
	}

	return false;
}