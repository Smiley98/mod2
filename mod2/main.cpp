#include "m2EntityFactory.h"
#include "m2GameObject.h"
#include "m2ComponentManager.h"
#include "m2TransformComponent.h"
#include "m2Utilities.h"

#include <Windows.h>
#include <thread>
#include <chrono>
#include <cstdio>

#define FRAMES_PER_SECOND 60.0
#define MILLISECONDS_PER_FRAME 1.0 / FRAMES_PER_SECOND

int main() {
	m2ComponentManager::allocateMaxContainers();

	m2GameObject* tester = new m2GameObject;
	makeTest(*tester);
	m2TransformComponent& transform = tester->getComponent<m2TransformComponent>();
	const glm::mat4& matrix = transform.getLocalTransformation();
	transform.setRotation(45.0f, 30.0f, 60.0f);
	transform.setScale(5.0f);
	m2Utils::printMatrix(matrix);

	transform.setScaleX(1.0f);
	m2Utils::printMatrix(matrix);

	transform.setScaleY(2.0f);
	m2Utils::printMatrix(matrix);

	transform.setScaleZ(3.0f);
	m2Utils::printMatrix(matrix);

	transform.setScale(5.0f);
	m2Utils::printMatrix(matrix);

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