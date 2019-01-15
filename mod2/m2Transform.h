#pragma once
#include "m2Component.h"
class m2Transform :
	public m2Component
{
public:
	m2Transform();
	~m2Transform();

	static m2ComponentType getType();
	void update();

	int* data;
	int otherData;
private:
};

