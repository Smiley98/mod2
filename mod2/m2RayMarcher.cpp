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

inline glm::mat4 getView(const glm::vec3& eye, const glm::vec3& centre, const glm::vec3& up) {
	glm::vec3 front = glm::normalize(centre - eye);
	glm::vec3 right = glm::normalize(glm::cross(front, up));
	glm::vec3 above = glm::cross(right, front);
	return glm::mat4(
		glm::vec4( right, 0.0f),
		glm::vec4( above, 0.0f),
		glm::vec4(-front, 0.0f),
		/*
		glm::vec4(
			-glm::dot(right, eye),
			-glm::dot(above, eye),
			 glm::dot(front, eye),
			 1.0
		)//*/
		glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
	);
}

glm::mat3 getView3(const glm::vec3& eye, const glm::vec3& centre, const glm::vec3& up) {
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
	glm::vec2 resolution(window.getClientWidth(), window.getClientHeight());
	//The distance to the projection plane is the screen resolution * the tangent of half the field of view.
	static const float fieldOfView = glm::radians(120.0f);
	static const float projectionDistance = -(min(resolution.x, resolution.y) * 0.5f * (tanf(fieldOfView * 0.5f)));
	static const glm::vec3 eye(8.0f, 5.0f, 7.0f);
	static const glm::vec3 centre(0.0f);
	static const glm::vec3 up(0.0f, 1.0f, 0.0f);
	
	//Will probably need this later on if we want to transform our models.
	//glm::mat4 view = getView(eye, centre, up);
	glm::mat3 view3 = getView3(eye, centre, up);

	static bool run = true;
	if (run) {
		run = false;
	}

	m2ShaderProgram& program = m2ShaderProgram::getProgram(RAYMARCH_SANDBOX);
	program.bind();
	//program.setMat4("u_view", view);
	program.setMat3("u_cameraRotation", view3);
	program.setVec2("u_resolution", resolution);
	program.setFloat("u_projectionDistance", projectionDistance);
	program.setFloat("u_time", m2Timing::instance().elapsedTime());
	render();
}