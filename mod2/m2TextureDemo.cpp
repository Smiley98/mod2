#include "m2TextureDemo.h"
#include "m2ScreenQuad.h"
#include "m2Shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <cstdio>
#include <array>
#include <thread>
#include <string>

m2TextureDemo::m2TextureDemo()
{
	initialize();
}

m2TextureDemo::~m2TextureDemo()
{
	shutdown();
}

void m2TextureDemo::initialize()
{
	std::string tdir = "Textures/";
	int channels;
	m_image = stbi_load((tdir + "big_texture.jpg").c_str(), &m_width, &m_height, &channels, 0);
	m_imageSize = m_width * m_height * channels;

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Internal format (first format parameter) = image format (CPU). Format (last format parameter) = pixel format (GPU).
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_BGR, GL_UNSIGNED_BYTE, nullptr);
	glActiveTexture(GL_TEXTURE0);

	m2ShaderProgram& sp = m2ShaderProgram::getProgram(TEXTURE_TEST);
	sp.bind();
	sp.setInt("u_texture", 0);
}

void m2TextureDemo::render()
{
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();

	upload();

	elapsed(start);
	m2ScreenQuad::render();
}

void m2TextureDemo::shutdown()
{
	if (m_image) {
		stbi_image_free(m_image);
		m_image = nullptr;
	}

	if (m_texture) {
		glDeleteTextures(1, &m_texture);
		m_texture = GL_NONE;
	}
}

void m2TextureDemo::upload()
{
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_BGR, GL_UNSIGNED_BYTE, m_image);
}

void m2TextureDemo::elapsed(std::chrono::steady_clock::time_point start)
{
	static short index;
	static std::array<long long, 16> counters;

	using namespace std::chrono;
	steady_clock::time_point stop = steady_clock::now();
	counters[index] = duration_cast<milliseconds>(stop - start).count();
	index++;
	if (index > counters.size() - 1) {
		index = 0;
		long long average = 0;
		for (long long i : counters)
			average += i;
		average /= counters.size();
		printf("%I64d\n", average);
	}
}