#include "m2MappingDemo.h"
#include "m2ScreenQuad.h"
#include "m2Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <string>

m2MappingDemo::m2MappingDemo()
{
	std::string tdir = "Textures/";

	int width, height, channels;
	m_test = stbi_load((tdir + "1.jpg").c_str(), &width, &height, &channels, 0);
	glGenTextures(1, &test_texture);
	glBindTexture(GL_TEXTURE_2D, test_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, m_test);
	//glGenerateMipmap(GL_TEXTURE_2D);//No need.
	
	glActiveTexture(GL_TEXTURE0);
	
	//glGenTextures(5, m_textures);
	//for (size_t i = 1; i < 6; i++) {
	//	m_images.push_back(stbi_load((tdir + std::to_string(i) + ".jpg").c_str(), &width, &height, &channels, 0));
	//}
}

m2MappingDemo::~m2MappingDemo()
{
	//for (auto& i : m_images) {
	//	stbi_image_free(i);
	//}
	stbi_image_free(m_test);
}

void m2MappingDemo::render()
{
	m2ShaderProgram& sp = m2ShaderProgram::getProgram(TEXTURE_TEST);
	sp.bind();
	sp.setInt("u_texture", 0);

	//Bind the vertex array, then do what OpenGL does best (draw triangles) ;)
	m2ScreenQuad::render();
}
