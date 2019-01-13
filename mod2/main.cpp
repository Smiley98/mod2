#include "m2EntityFactory.h"
#include "m2GameObject.h"
#include "m2Transform.h"
#include <cstdio>

int main() {
	m2GameObject::initComponentContainers();

	m2GameObject* tester = new m2GameObject;
	makeTest(*tester);
	printf("Transform data: %i %i\n", *tester->getComponent<m2Transform>().data, tester->getComponent<m2Transform>().otherData);

	getchar();
	printf("Deleting test object!\n");
	delete tester;
	printf("Deleted test object.\n");

	m2GameObject::killComponentContainers();
	getchar();
	return 0;
}