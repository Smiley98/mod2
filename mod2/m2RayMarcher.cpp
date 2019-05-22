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
	//Figure out the smaller of the two screen dimensions, half it so we can use trig rations, speaking of which, take the tangent of half the field of view!
	float projectionDistance = min(resolution.x, resolution.y) * 0.5f * (tanf(glm::radians(45.0f) * 0.5f));
	m2ShaderProgram& program = m2ShaderProgram::getProgram(RAYMARCH_SANDBOX);
	program.bind();
	program.setVec2("u_resolution", resolution);
	program.setFloat("u_projectionDistance", projectionDistance);
	program.setFloat("u_time", m2Timing::instance().elapsedTime());
	render();
}