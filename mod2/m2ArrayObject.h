#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

struct m2ArrayObject
{
	m2ArrayObject() { glGenVertexArrays(1, &m_vao); }
	~m2ArrayObject() { glDeleteVertexArrays(1, &m_vao); }
protected:
	GLuint m_vao;
};
