#include "m2Transform.h"

m2Transform::m2Transform()
{
	data = new int(5);
}

m2Transform::~m2Transform()
{
	delete data;
}

m2ComponentType m2Transform::getType()
{
	return m2ComponentType::TRANSFORM;
}
