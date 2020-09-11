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
{
	for (size_t i = 0; i < m_textures.size(); i++)
		m_threads[i] = std::thread([this, i] { m_textures[i].copy(); });

	for (std::thread& t : m_threads)
		t.join();
}

void m2StreamingDemo::render()
{
	for (size_t i = 0; i < m_textures.size(); i++)
		m_textures[i].uploadBegin();

	for (size_t i = 0; i < m_textures.size(); i++)
		m_textures[i].uploadEnd();

	m2ScreenQuad::render();
}
