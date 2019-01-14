#include "m2EntityFactory.h"
#include "m2GameObject.h"
#include "m2Transform.h"
#include <cstdio>

int main() {
	m2GameObject::allocateComponentContainers();

	m2GameObject tester;
	makeTest(tester);
	printf("Transform data: %i %i\n", *tester.getComponent<m2Transform>().data, tester.getComponent<m2Transform>().otherData);

	m2GameObject::deallocateComponentContainers();
	getchar();
	return 0;
}