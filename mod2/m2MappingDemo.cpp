#include "m2MappingDemo.h"
#include "m2ScreenQuad.h"
#include "m2Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <string>
#include <ctime>

m2MappingDemo::m2MappingDemo()
{
	srand(time(0));
	std::string tdir = "Textures/";
	int channels;

	for (size_t i = 1; i < 6; i++)
		m_images.push_back(stbi_load((tdir + std::to_string(i) + ".jpg").c_str(), &m_width, &m_height, &channels, 0));

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_images[(rand() % 5)]);
	glActiveTexture(GL_TEXTURE0);

	m2ShaderProgram& sp = m2ShaderProgram::getProgram(TEXTURE_TEST);
	sp.bind();
	sp.setInt("u_texture", 0);
}

m2MappingDemo::~m2MappingDemo()
{
	for (auto& i : m_images)
		stbi_image_free(i);
	glDeleteTextures(1, &m_texture);
}

void m2MappingDemo::render()
{
	for(int i=0; i<100; i++)
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGB, GL_UNSIGNED_BYTE, m_images[(rand() % 5)]);

	//Bind the vertex array, then do what OpenGL does best (draw triangles) ;)
	m2ScreenQuad::render();
}
