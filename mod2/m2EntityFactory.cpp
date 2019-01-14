#include "m2EntityFactory.h"
#include "m2GameObject.h"
#include "m2Transform.h"

void makeTest(m2GameObject& g)
{
	m2Transform& transform = g.addComponent<m2Transform>();
	*transform.data = 5;
	transform.otherData = 7;
}

void makeMeme(m2GameObject& g)
{
	//return MemeObject();
}
