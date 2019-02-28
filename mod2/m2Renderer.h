#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
class m2Renderer
{
public:
	m2Renderer() { glGenVertexArrays(1, &m_vao); }
	virtual ~m2Renderer() { glDeleteVertexArrays(1, &m_vao); }

	//Keep this signature clean. If I need to pass information, do it through a setter cause its incredibly constraining to have all renderers make use of the same information in their overloaded method.
	virtual void render() = 0;

protected:
	GLuint m_vao;
};