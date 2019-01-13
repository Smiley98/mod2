#include "m2EntityFactory.h"
#include "m2GameObject.h"
#include "m2Transform.h"

TestObject makeTest(m2GameObject& g)
{
	m2Transform& transform = g.addComponent<m2Transform>();
	*transform.data = 5;
	transform.otherData = 7;
	return g;
}

MemeObject makeMeme(m2GameObject& g)
{
	return MemeObject();
}
