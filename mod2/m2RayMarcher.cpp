#include "m2RayMarcher.h"
#include "m2Shader.h"
#include "m2Window.h"
#include "m2Timing.h"
#include "m2Utilities.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "m2MemberDelegate.h"

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

struct DelegateTest {
	int meme(float a) {
		printf("Called back meme!\n");
		return a * 2;
	}
};

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
	float period = sinf(time) * 0.5f + 0.5f;

	//glm::vec3 modelScale(1.2f);
	//glm::quat modelRotation(glm::angleAxis(time, glm::vec3(0.0f, 1.0f, 0.0f)));

	//It didn't kick in that transformation order is reversed before because we were translating and rotating around the same axis.
	glm::vec3 modelTranslation = glm::vec3(-2.0f * cosf(time), 0.0f, 0.0f) * -1.0f;
	glm::mat3 modelRotation(glm::rotate(glm::mat4(1.0f), -time, glm::vec3(0.0f, 1.0f, 0.0f)));
	glm::mat4 modelTransform(modelRotation);
	modelTransform *= glm::translate(glm::mat4(1.0f), modelTranslation);

	//Takes in a float, returns an int.
	DelegateTest dm;
	//<ClassType, ReturnType, ParameterType...
	m2MemberDelegate<DelegateTest, int, float> callback(dm, &dm.meme, 5.0f);

	//Above (negation and pre-multiplication) is cheaper than inverse and [regular] post-multiplication.
	//glm::mat4 modelTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f * cosf(time), 0.0f, 0.0f));
	//glm::mat4 modelRotation(glm::rotate(glm::mat4(1.0f), time, glm::vec3(0.0f, 1.0f, 0.0f)));
	//glm::mat4 modelTransform = glm::inverse(modelTranslation * modelRotation);

	static bool run = true;
	if (run) {
		run = false;
	}

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

	render();
}