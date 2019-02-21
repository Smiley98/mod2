#include "m2Application.h"

int main() {
	m2Application application;
	application.run();

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

	return 0;
}