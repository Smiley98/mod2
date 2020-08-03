#include "m2TextureDemo.h"
#include "m2ScreenQuad.h"
#include "m2Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <string>
#include <ctime>

const size_t m2TextureDemo::s_imageCount = 5;

m2TextureDemo::m2TextureDemo()
{
	initialize();
	//Internal format (first format parameter) = image format (CPU). Format (last format parameter) = pixel format (GPU).
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_BGR, GL_UNSIGNED_BYTE, nullptr);
}

m2TextureDemo::~m2TextureDemo()
{
	shutdown();
}

void m2TextureDemo::initialize()
{
	std::string tdir = "Textures/";
	for (size_t i = 1; i < s_imageCount + 1; i++)
		m_images.push_back(stbi_load((tdir + std::to_string(i) + ".jpg").c_str(), &m_width, &m_height, &m_channels, 0));
	m_imageSize = m_width * m_height * m_channels;

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glActiveTexture(GL_TEXTURE0);

	m2ShaderProgram& sp = m2ShaderProgram::getProgram(TEXTURE_TEST);
	sp.bind();
	sp.setInt("u_texture", 0);
}

void m2TextureDemo::render()
{
	//Naive approach that doesn't take advantage of hardware accelerated transfering.
	for (int i = 0; i < 100; i++)
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_BGR, GL_UNSIGNED_BYTE, m_images[(rand() % s_imageCount)]);
	m2ScreenQuad::render();
}

void m2TextureDemo::shutdown()
{
	for (auto& i : m_images)
		stbi_image_free(i);
	glDeleteTextures(1, &m_texture);
}
