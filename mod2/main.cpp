#include "m2EntityFactory.h"
#include "m2GameObject.h"
#include "m2ComponentManager.h"
#include "m2TransformComponent.h"
#include <Windows.h>
#include <thread>
#include <chrono>
#include <cstdio>
#include <functional>

#define FRAMES_PER_SECOND 60.0
#define MILLISECONDS_PER_FRAME 1.0 / FRAMES_PER_SECOND

int main() {
	m2ComponentManager::allocateMaxContainers();

	m2GameObject* tester = new m2GameObject;
	makeTest(*tester);

	while (true) {
		if (GetAsyncKeyState(VK_ESCAPE)) break;
		using namespace std::chrono;
		high_resolution_clock::time_point start = high_resolution_clock::now();
		
		//Don't actually update transforms cause they don't need to be.
		std::thread transforms(m2ComponentManager::updateComponentsOfType<m2TransformComponent>);
		transforms.join();

		double frameTime = duration_cast<duration<double>>(high_resolution_clock::now() - start).count();
		//printf("Frame time: %f.\n", frameTime);
		//Remove this / set to 0 if we want an uncapped frame rate.
		while (frameTime < MILLISECONDS_PER_FRAME) {
			//Render over and over? Buffer stuff for next frame?
			frameTime = duration_cast<duration<double>>(high_resolution_clock::now() - start).count();
		}
	}

	delete tester;
	printf("Loop exited!\n");
	m2ComponentManager::deallocateContainers();

	printf("Press enter to terminate the program.\n");
	getchar();
	return 0;
}