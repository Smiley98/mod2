#include "TestRenderer.h"
#include "m2Window.h"
#include "m2Shader.h"

#include <glad/glad.h>
#include <glm/gtc/random.hpp>
#include <vector>

//I can't see the window methods cause instance() is base class method so it doesn't know.
TestRenderer::TestRenderer() : m_halfScreenHeight(m2Window::instance().getHeight() / 2)
{
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_cbo);
	glGenBuffers(1, &m_hbo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, m_cbo);
	m_colours.resize(m_numRays);
	for(glm::vec4& colour : m_colours)
		colour = glm::linearRand(glm::vec4(0.0f), glm::vec4(1.0f));
	glBufferData(GL_ARRAY_BUFFER, m_colours.size() * sizeof(glm::vec4), m_colours.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), nullptr);

	glBindBuffer(GL_ARRAY_BUFFER, m_hbo);
	m_heights.resize(m_numRays);
	for (float& height : m_heights)
		height = glm::linearRand(75.0f, 100.0f);
	glBufferData(GL_ARRAY_BUFFER, m_heights.size() * sizeof(float), m_heights.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(float), nullptr);

	glBindVertexArray(GL_NONE);
}

TestRenderer::~TestRenderer()
{
}

void TestRenderer::render()
{
	//Let's do things one at a time. No flashy visuals just yet!
	/*glBindBuffer(GL_ARRAY_BUFFER, m_cbo);
	for (glm::vec3& colour : m_colours)
		colour = glm::linearRand(glm::vec3(0.0f), glm::vec3(1.0f));
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_colours.size() * sizeof(glm::vec3), m_colours.data());*/

	/*glBindBuffer(GL_ARRAY_BUFFER, m_icbo);
	for (glm::vec3& inverse : m_colours)
		inverse = glm::vec3(1.0f) - inverse;
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_colours.size() * sizeof(glm::vec3), m_colours.data());*/

	/*glBindBuffer(GL_ARRAY_BUFFER, m_hbo);
	for (float& height : m_heights)
		height = glm::linearRand(75.0f, 100.0f);
	glBufferSubData(GL_ARRAY_BUFFER, 0, m_heights.size() * sizeof(float), m_heights.data());*/
	//printf("%i %f\n", m_numRays, m_step);

	auto& rayShader = m2ShaderProgram::getProgram(RAY);
	rayShader.bind();

	//rayShader.setFloat("u_halfScreenHeight", m_halfScreenHeight);

	glBindVertexArray(m_vao);
	glDrawArrays(GL_POINTS, 0, m_numRays);

	/*auto& test = m2ShaderProgram::getProgram(MULTILINE_TEST);
	test.bind();
	
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glDrawArrays(GL_POINTS, 1, m_numRays);
	glDeleteVertexArrays(1, &vao);*/
}
