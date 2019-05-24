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
	static const float fieldOfView = glm::radians(120.0f);
	static const float projectionDistance = -(min(resolution.x, resolution.y) * 0.5f * (tanf(fieldOfView * 0.5f)));
	static const float nearPlane = 0.001f;
	static const float farPlane = 100.0f;

	static const glm::vec3 cameraTranslation(8.0f, 5.0f, 7.0f);	//Eye.
	static const glm::vec3 cameraTarget(0.0f);					//Centre.
	static const glm::vec3 worldUp(0.0f, 1.0f, 0.0f);			//Up (y-axis).
	static const glm::mat3 cameraRotation = rotateCamera(cameraTranslation, cameraTarget, worldUp);

	static bool run = true;
	if (run) {
		run = false;
	}

	m2ShaderProgram& program = m2ShaderProgram::getProgram(RAYMARCH_SANDBOX);
	program.bind();

	//Easier to work with separately because its nice to be able to multiply mat3 by vec3 instead of extract from a 3x4 or 4x4 camera matrix.
	program.setMat3("u_cameraRotation", cameraRotation);
	program.setVec3("u_cameraTranslation", cameraTranslation);

	program.setVec2("u_resolution", resolution);
	program.setFloat("u_projectionDistance", projectionDistance);
	program.setFloat("u_nearPlane", nearPlane);
	program.setFloat("u_farPlane", farPlane);

	program.setFloat("u_time", m2Timing::instance().elapsedTime());

	render();
}