#include "m2LineRenderer.h"
#include "m2Shader.h"

m2LineRenderer::m2LineRenderer()
{
}

m2LineRenderer::~m2LineRenderer()
{
}

void m2LineRenderer::render()
{
	m2ShaderProgram& sp = m2ShaderProgram::getProgram(LINE);
	sp.bind();
	sp.setVec3("u_colour", glm::vec3(1.0f));
	glBindVertexArray(m_vao);
	glDrawArrays(GL_POINTS, 0, 1);
}