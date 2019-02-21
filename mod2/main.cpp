#include "m2EntityFactory.h"
#include "m2GameObject.h"
#include "m2ComponentManager.h"
#include "m2TransformComponent.h"
#include "m2Utilities.h"

#include <Windows.h>
#include <thread>
#include <chrono>
#include <cstdio>

#include <glm/gtx/euler_angles.hpp>

#include "m2Window.h"

#define FRAMES_PER_SECOND 60.0
#define MILLISECONDS_PER_FRAME 1.0 / FRAMES_PER_SECOND

int main() {
	m2ComponentManager::allocateMaxContainers();
	m2GameObject* tester = new m2GameObject;
	makeTest(*tester);

	/*float a, b, c;
	m2TransformComponent& transform = tester->getComponent<m2TransformComponent>();
	glm::eulerAngleXYZ(1.0f, 2.0f, 3.0f);			//Pre-computed xyz matrix (thing full of sin's and cos').
	glm::extractEulerAngleXYZ(glm::mat4(), a, b, c);//Atan2 all around!
	
	//Stuff fails after 90+ degrees.
	m2Utils::printVector(glm::degrees(glm::eulerAngles(glm::quat(glm::radians(glm::vec3(89.0f))))));
	transform.setDeltaRotationX(5.0f);
	transform.setDeltaRotationY(80.0f);
	transform.setDeltaRotationZ(10.0f);
	m2Utils::printVector(transform.getLocalRotation());*/

	m2Window& window = m2Window::instance();
	window.update();

	/*while (true) {
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
	}*/

	delete tester;
	m2ComponentManager::deallocateContainers();
	window.~m2Window();

	printf("Press enter to terminate the program.\n");
	getchar();
	return 0;
}