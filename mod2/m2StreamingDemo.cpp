#include "m2StreamingDemo.h"
#include "m2ScreenQuad.h"
#include "m2Shader.h"
#include "m2Utilities.h"

m2StreamingDemo::m2StreamingDemo()
{
	for (size_t i = 0; i < m_textures.size(); i++)
		m_textures[i].initialize("4k/" + std::to_string(i + 1) + ".jpg");

	m2ShaderProgram& sp = m2ShaderProgram::getProgram(TEXTURE_TEST);
	sp.bind();
	sp.setInt("u_texture", 0);
	glActiveTexture(GL_TEXTURE0);
}

m2StreamingDemo::~m2StreamingDemo()
{
}

void m2StreamingDemo::update()
{	//This must happen sequentially since a rendering context cannot be current for more than one thread :(
	for (size_t i = 0; i < m_textures.size(); i++)
		m_textures[i].copy();
}

void m2StreamingDemo::render()
{
	for (size_t i = 0; i < m_textures.size(); i++)
		m_textures[i].uploadBegin();

	for (size_t i = 0; i < m_textures.size(); i++)
		m_textures[i].uploadEnd();

	m2ScreenQuad::render();
}
