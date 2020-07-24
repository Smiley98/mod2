#include "m2MappingDemo.h"
#include "m2ScreenQuad.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <string>

m2MappingDemo::m2MappingDemo()
{
	std::string tdir = "Textures/";

	int width, height, channels;
	m_test = stbi_load((tdir + "1.jpg").c_str(), &width, &height, &channels, 0);

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


	//Bind the vertex array, then do what OpenGL does best (draw triangles) ;)
	m2ScreenQuad::render();
}
