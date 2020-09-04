#include "m2ScreenQuad.h"

static GLuint screenQuadVao;
static GLuint screenQuadVbo;

void m2ScreenQuad::render()
{
	glBindVertexArray(screenQuadVao);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void m2ScreenQuad::init()
{
	float quadData[] =
	{
		1.0f, 1.0f,// 0.0f,
		-1.0f, 1.0f,// 0.0f,
		1.0f, -1.0f,// 0.0f,

		-1.0f, -1.0f,// 0.0f,
		1.0f, -1.0f,// 0.0f,
		-1.0f, 1.0f,// 0.0f
	};

	glGenVertexArrays(1, &screenQuadVao);
	glGenBuffers(1, &screenQuadVbo);

	glBindVertexArray(screenQuadVao);
	glBindBuffer(GL_ARRAY_BUFFER, screenQuadVbo);
	glEnableVertexAttribArray(0);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadData), quadData, GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, 6 * 2 * sizeof(float), quadData, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, GL_NONE);
	glBindVertexArray(GL_NONE);
}

void m2ScreenQuad::shutdown()
{
	glDeleteVertexArrays(1, &screenQuadVao);
	glDeleteBuffers(1, &screenQuadVbo);
}