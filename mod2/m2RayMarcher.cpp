#include "m2RayMarcher.h"
#include "m2Shader.h"
#include "m2Window.h"
#include "m2Timing.h"
#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

static m2Window& window = m2Window::instance();

m2RayMarcher::m2RayMarcher()
{
}

m2RayMarcher::~m2RayMarcher()
{
}

void m2RayMarcher::marchCircle()
{
	glm::vec2 resolution(window.getClientWidth(), window.getClientHeight());
	//I can't normalize this single value, otherwise its rendered useless because it becomes a scalar value of 1.
	//We could get away with some sort of optimization here if we defined a range, but there's no criteria for that.
	//float projectionDistance = -(min(resolution.x, resolution.y) * 0.5f * (tanf(glm::radians(75.0f) * 0.5f)));
	m2ShaderProgram& program = m2ShaderProgram::getProgram(RAYMARCH_SANDBOX);
	program.bind();
	program.setVec2("u_resolution", resolution);
	//program.setFloat("u_projectionDistance", projectionDistance);
	program.setFloat("u_time", m2Timing::instance().elapsedTime());
	render();
}