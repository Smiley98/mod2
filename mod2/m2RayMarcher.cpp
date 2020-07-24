#include "m2RayMarcher.h"
#include "m2ScreenQuad.h"
#include "m2Shader.h"
#include "m2Window.h"
#include "m2Timing.h"
#include "m2Utilities.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

//#include "m2MemberDelegate.h"

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

inline glm::mat3 lookAtDirection(const glm::vec3& eye, const glm::vec3& centre, const glm::vec3& up) {
	glm::vec3 front = glm::normalize(centre - eye);
	glm::vec3 right = glm::normalize(glm::cross(front, up));
	glm::vec3 above = glm::cross(right, front);

	return glm::mat3(
		glm::vec3(right),
		glm::vec3(above),
		glm::vec3(-front)
	);
}

void m2RayMarcher::render()
{
	//The distance to the projection plane is the screen resolution * the tangent of half the field of view.
	static const glm::vec2 resolution(window.getClientWidth(), window.getClientHeight());
	static const float fieldOfView = glm::radians(45.0f);
	static const float projectionDistance = -(min(resolution.x, resolution.y) * 0.5f * (tanf(fieldOfView * 0.5f)));

	//printf("%f\n", projectionDistance);-223.
	//Warning, the projection math is wrong! Should be as follows:
	//const float opp = max(resolution.x, resolution.y) * 0.5f;
	//const float tanTheta = tanf(fieldOfView * 0.5f);//Fov should be changed from 45 to something above 60 after this fix.
	//const float invAdj = tanTheta / opp;
	//static const float projectionDistance = 1 / invAdj;

	static const float nearPlane = 0.001f;
	static const float farPlane = 100.0f;

	static const glm::vec3 cameraTranslation(0.0f, 0.0f, 3.0f);	//Eye.
	static const glm::vec3 cameraTarget(0.0f);					//Centre.
	static const glm::vec3 worldUp(0.0f, 1.0f, 0.0f);			//Up (y-axis).
	static const glm::mat3 cameraRotation = lookAtDirection(cameraTranslation, cameraTarget, worldUp);

	float time = m2Timing::instance().elapsedTime();
	float period = sinf(time) * 0.5f + 0.5f;

	glm::mat4 modelTransform(1.0f);
	modelTransform[3] = glm::vec4(2.0f, 0.0f, 0.0f, 1.0f);

	m2ShaderProgram& program = m2ShaderProgram::getProgram(RAYMARCH_SANDBOX);
	program.bind();

	//Models
	program.setMat4("u_modelTransform", modelTransform);

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
	program.setFloat("u_period", period);

	m2ScreenQuad::render();
}
