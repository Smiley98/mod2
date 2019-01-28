#pragma once
#include "m2Component.h"
#include "m2Transform.h"
class m2TransformComponent :
	public m2Transform, public m2Component<m2TransformComponent>
{
public:
	m2TransformComponent();
	~m2TransformComponent();
	void update();
	static m2ComponentType getType();
};