#pragma once
#define GLM_FORCE_SWIZZLE
#include "m2Effects.h"
class m2RayMarcher :
	public m2Effects
{
public:
	m2RayMarcher();
	~m2RayMarcher();

	static void marchCircle();
};

