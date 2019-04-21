#include "m2RayRenderer.h"
#include "m2Window.h"
#include "m2Shader.h"
#include "m2Utilities.h"

#include <glad/glad.h>
#include <glm/gtc/random.hpp>
#include <vector>

static m2Window& window = m2Window::instance();

m2RayRenderer::m2RayRenderer(float xMin, float xMax, unsigned int thickness) :
	m_xMin(xMin), m_xMax(xMax), m_thickness(thickness), m_count(xMax - xMin / thickness),
	//Step is 2 / count cause we're going from -1 to +1 which is a range of 2.
	m_step(2.0f / (float)m_count), m_halfScreenHeight(m2Window::instance().getClientHeight() / 2)
{
	m_colours.resize(m_count);
	m_heights.resize(m_count);

	assert(thickness > 0);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_cbo);
	glGenBuffers(1, &m_hbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, m_cbo);
	for (unsigned int i = 0; i < m_count; i++)
		m_colours[i] = glm::linearRand(glm::vec4(0.0f), glm::vec4(1.0f));
	glBufferData(GL_ARRAY_BUFFER, m_count * sizeof(glm::vec4), m_colours.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, m_hbo);
	for (unsigned int i = 0; i < m_count; i++)
		m_heights[i] = glm::linearRand(500.0f, (float)window.getClientHeight() / 2.0f);
	glBufferData(GL_ARRAY_BUFFER, m_count * sizeof(float), m_heights.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(float), nullptr);

	glBindVertexArray(GL_NONE);
}

m2RayRenderer::~m2RayRenderer()
{
	glDeleteBuffers(1, &m_cbo);
	glDeleteBuffers(1, &m_hbo);
}

void m2RayRenderer::render()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_cbo);
	for(unsigned int i = 0; i < m_count; i++)
		m_colours[i] = glm::linearRand(glm::vec4(0.0f), glm::vec4(1.0f));
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_count * sizeof(glm::vec4), m_colours.data());

	glBindBuffer(GL_ARRAY_BUFFER, m_hbo);
	for (unsigned int i = 0; i < m_count; i++)
		m_heights[i] = glm::linearRand(0.0f, (float)window.getClientHeight() / 2.0f);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_heights.size() * sizeof(float), m_heights.data());

	m2ShaderProgram& rayShader = m2ShaderProgram::getProgram(RAY);
	rayShader.bind();
	
	rayShader.setFloat("u_start", m2Utils::screenToNdcX(m_xMin));
	rayShader.setFloat("u_step", m_step);
	rayShader.setFloat("u_rayOrigin", (float)window.getClientHeight() / 2.0f);
	rayShader.setFloat("u_clientY", (float)window.getClientY());
	rayShader.setFloat("u_clientHeight", (float)window.getClientHeight());

	glBindVertexArray(m_vao);
	glDrawArrays(GL_POINTS, 0, m_count);
}