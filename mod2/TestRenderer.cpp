#include "TestRenderer.h"
#include "m2Shader.h"
#include <glad/glad.h>
#include <glm/gtc/random.hpp>
#include <vector>

TestRenderer::TestRenderer()
{
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_cbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_cbo);
	m_colours.resize(m_numElements);
	for(glm::vec3& colour : m_colours)
		colour = glm::linearRand(glm::vec3(0.0f), glm::vec3(1.0f));
	glBufferData(GL_ARRAY_BUFFER, m_numElements, m_colours.data(), GL_STATIC_DRAW);//GL_STREAM_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

	//Check if there's a way to copy vertex buffers cause all this duplication doesn't seem necessary for making a buffer with the same layout but different values.
	//Perhaps I can flyweight this in the sense that I make common buffer formats! Still has some shortcomings like not knowing which index it is so I'll hold off for now.
	glGenBuffers(1, &m_icbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_icbo);
	for (glm::vec3& inverse : m_colours)
		inverse = glm::vec3(1.0f) - inverse;
	glBufferData(GL_ARRAY_BUFFER, m_numElements, m_colours.data(), GL_STATIC_DRAW);//GL_STREAM_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);

	glGenBuffers(1, &m_hbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_hbo);
	m_heights.resize(m_numElements);
	for (float& height : m_heights)
		height = glm::linearRand(75.0f, 100.0f);
	glBufferData(GL_ARRAY_BUFFER, m_numElements, m_heights.data(), GL_STATIC_DRAW);//GL_STREAM_DRAW);
	glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE, sizeof(float), nullptr);

	glBindVertexArray(GL_NONE);
}

TestRenderer::~TestRenderer()
{
}

void TestRenderer::render()
{
	glBindVertexArray(m_vao);
	//glBufferSubData//Not sure how this works.
	auto& lineShader = m2ShaderProgram::getProgram(RAY);
	lineShader.bind();

	glDrawArrays(GL_POINTS, 0, m_numElements);
}
