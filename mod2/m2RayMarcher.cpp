#include "m2RayMarcher.h"
#include "m2Shader.h"
#include "m2Window.h"
#include "m2Timing.h"
#include "m2Utilities.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

//I can't believe I can't figure this out on my own...
#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

static m2Window& window = m2Window::instance();

m2RayMarcher::m2RayMarcher()
{
}

m2RayMarcher::~m2RayMarcher()
{
}

inline glm::mat3 rotateCamera(const glm::vec3& eye, const glm::vec3& centre, const glm::vec3& up) {
	glm::vec3 front = glm::normalize(centre - eye);
	glm::vec3 right = glm::normalize(glm::cross(front, up));
	glm::vec3 above = glm::cross(right, front);

	return glm::mat3(
		glm::vec3(right),
		glm::vec3(above),
		glm::vec3(-front)
	);
}

void m2RayMarcher::marchCircle()
{
	//The distance to the projection plane is the screen resolution * the tangent of half the field of view.
	static const glm::vec2 resolution(window.getClientWidth(), window.getClientHeight());
	static const float fieldOfView = glm::radians(45.0f);
	static const float projectionDistance = -(min(resolution.x, resolution.y) * 0.5f * (tanf(fieldOfView * 0.5f)));
	static const float nearPlane = 0.001f;
	static const float farPlane = 100.0f;

	static const glm::vec3 cameraTranslation(2.0f);				//Eye.
	static const glm::vec3 cameraTarget(0.0f);					//Centre.
	static const glm::vec3 worldUp(0.0f, 1.0f, 0.0f);			//Up (y-axis).
	static const glm::mat3 cameraRotation = rotateCamera(cameraTranslation, cameraTarget, worldUp);

	float time = m2Timing::instance().elapsedTime();

	//glm::vec3 cubeScale(1.2f);
	//glm::quat cubeRotation(glm::angleAxis(time, glm::vec3(0.0f, 1.0f, 0.0f)));

	glm::vec3 cubeTranslation(0.0f, 2.2f * (sinf(time) * 0.5f + 0.5f), 0.0f);
	glm::mat3 cubeRotation(glm::transpose(glm::rotate(glm::mat4(1.0f), time, glm::vec3(0.0f, 1.0f, 0.0f))));
	glm::mat4 cubeTransform(cubeRotation);
	cubeTransform[3] = glm::vec4(cubeTranslation, 1.0);

	static bool run = true;
	if (run) {
		run = false;
	}

	m2ShaderProgram& program = m2ShaderProgram::getProgram(RAYMARCH_SANDBOX);
	program.bind();

	//Models
	program.setMat4("u_cubeTransform", cubeTransform);
	//program.setMat3("u_rotation", cubeRotation);

	//View
	program.setMat3("u_cameraRotation", cameraRotation);
	program.setVec3("u_cameraTranslation", cameraTranslation);

	//Projection
	program.setVec2("u_resolution", resolution);
	program.setFloat("u_projectionDistance", projectionDistance);
	program.setFloat("u_nearPlane", nearPlane);
	program.setFloat("u_farPlane", farPlane);

	//Utility
	program.setFloat("u_time", time);

	render();
}