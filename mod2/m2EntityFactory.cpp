#include "m2EntityFactory.h"
#include "m2GameObject.h"
#include "m2TransformComponent.h"

void makeTest(m2GameObject& g)
{
	m2TransformComponent& transform = g.addComponent<m2TransformComponent>();
}

void makeMeme(m2GameObject& g)
{
	//return MemeObject();
}
