#include "m2RayMarcher.h"
#include "m2Shader.h"
#include "m2Window.h"

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
	m2ShaderProgram& program = m2ShaderProgram::getProgram(RAYMARCH_SANDBOX);
	program.bind();
	program.setVec2("u_resolution", resolution);
	render();
}