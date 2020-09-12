#include "m2Texture.h"

//Apparently I only need to define this once, no compiler/link order jank!?
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

namespace {
	const GLbitfield flags = GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
	const GLenum uploadTarget = GL_PIXEL_UNPACK_BUFFER;
	//const GLenum downloadTarget = GL_PIXEL_PACK_BUFFER;
}

GLint m2Texture::s_format;

void m2Texture::queryOptimalFormat()
{
	glGetInternalformativ(GL_TEXTURE_2D, GL_RGBA8, GL_TEXTURE_IMAGE_FORMAT, 1, &s_format);
}

void m2Texture::initialize(const std::string& fileName)
{	//Image loading is taken care of in the xr engine. We just need to transfer data asynchronously.
	static std::string tdir = "Textures/";
	int channels;
	m_image = stbi_load((tdir + fileName).c_str(), &m_width, &m_height, &channels, STBI_rgb_alpha);
	m_imageSize = m_width * m_height * STBI_rgb_alpha;
	
	glGenBuffers(1, &m_uploadPBO);
	glBindBuffer(uploadTarget, m_uploadPBO);
	glBufferStorage(uploadTarget, m_imageSize, nullptr, flags | GL_MAP_WRITE_BIT | GL_DYNAMIC_STORAGE_BIT);
	m_uploadMemory = static_cast<GLubyte*>(glMapBufferRange(uploadTarget, 0, m_imageSize, flags | GL_MAP_WRITE_BIT));
	memcpy(m_uploadMemory, m_image, m_imageSize);

	//glGenBuffers(1, &m_downloadPBO);
	//glBindBuffer(downloadTarget, m_downloadPBO);
	//glBufferStorage(downloadTarget, m_imageSize, nullptr, flags | GL_MAP_READ_BIT | GL_DYNAMIC_STORAGE_BIT);
	//m_downloadMemory = static_cast<GLubyte*>(glMapBufferRange(downloadTarget, 0, m_imageSize, flags | GL_MAP_READ_BIT));
	//memcpy(m_downloadMemory, m_image, m_imageSize);
	//Might have to change to reflect size of framebuffer rather than texture.

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, s_format, GL_UNSIGNED_BYTE, nullptr);
}

void m2Texture::shutdown()
{
	glDeleteTextures(1, &m_texture);
	glDeleteBuffers(1, &m_uploadPBO);
	//glDeleteBuffers(1, &m_downloadPBO);
	stbi_image_free(m_image);
}

void m2Texture::copy()
{
	memcpy(m_uploadMemory, m_image, m_imageSize);
}

void m2Texture::uploadBegin()
{
	//memcpy(m_uploadMemory, m_image, m_imageSize);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glBindBuffer(uploadTarget, m_uploadPBO);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, s_format, GL_UNSIGNED_BYTE, nullptr);
	fence(m_uploadFence);
}

void m2Texture::uploadEnd()
{
	wait(m_uploadFence);
}

void m2Texture::downloadBegin()
{
	//glGetTextureSubImage is also an option, core in GL 4.5
	//glReadPixels(0, 0, m_width, m_height, GL_BGRA, GL_UNSIGNED_BYTE, nullptr);
	//fence(m_downloadFence);
}

void m2Texture::downloadEnd()
{
	//wait(m_downloadFence);
	//memcpy to some external buffer or return data?
	//return m_downloadMemory;//Be careful with this. Be sure to copy rather than reassign this as its GL mapped memory!
}

void m2Texture::fence(GLsync& sync)
{
	//Create a new sync point and delete the old one.
	if (sync)
		glDeleteSync(sync);
	sync = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
}

void m2Texture::wait(const GLsync& sync)
{
	//Stall CPU until fence is signalled.
	if (sync) {
		while (glClientWaitSync(sync, GL_SYNC_FLUSH_COMMANDS_BIT, 1) == GL_TIMEOUT_EXPIRED) {}
	}
}
