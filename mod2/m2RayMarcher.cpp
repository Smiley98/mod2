#include "m2RayMarcher.h"
#include "m2Shader.h"

m2RayMarcher::m2RayMarcher()
{
}

m2RayMarcher::~m2RayMarcher()
{
}

void m2RayMarcher::marchCircle()
{
	m2ShaderProgram& program = m2ShaderProgram::getProgram(QUAD_TEST);
	program.bind();
	render();
}
