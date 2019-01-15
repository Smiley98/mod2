#include "m2EntityFactory.h"
#include "m2GameObject.h"
#include "m2Transform.h"
#include <Windows.h>
#include <thread>
#include <chrono>
#include <cstdio>
#include <functional>

#define FRAMES_PER_SECOND 60.0
#define MILLISECONDS_PER_FRAME 1.0 / FRAMES_PER_SECOND

void test(){}

int main() {
	m2GameObject::allocateComponentContainers();

	m2GameObject tester;
	makeTest(tester);
	printf("Transform data: %i %i\n", *tester.getComponent<m2Transform>().data, tester.getComponent<m2Transform>().otherData);

	while (true) {
		if (GetAsyncKeyState(VK_ESCAPE)) break;
		using namespace std::chrono;
		high_resolution_clock::time_point start = high_resolution_clock::now();
		
		std::thread transforms(m2GameObject::update<m2Transform>);
		transforms.join();

		double frameTime = duration_cast<duration<double>>(high_resolution_clock::now() - start).count();
		//printf("Frame time: %f.\n", frameTime);
		//Remove this / set to 0 if we want an uncapped frame rate.
		while (frameTime < MILLISECONDS_PER_FRAME) {
			//Render over and over? Buffer stuff for next frame?
			frameTime = duration_cast<duration<double>>(high_resolution_clock::now() - start).count();
		}
	}
	printf("Loop exited!\n");
	m2GameObject::deallocateComponentContainers();
	printf("Press enter to terminate the program.\n");
	getchar();
	return 0;
}