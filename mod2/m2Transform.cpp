#include "m2Transform.h"
#include <cstdio>

m2Transform::m2Transform()
{
	printf("Transform constructor called.\n");
	data = new int(5);
}

m2Transform::~m2Transform()
{
	printf("Transform destructor called.\n");
	delete data;
}

m2ComponentType m2Transform::getType()
{
	return m2ComponentType::TRANSFORM;
}

void m2Transform::update()
{
	printf("Values: %i %i.\n", *data, otherData);
}
