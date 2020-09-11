#include "m2PBODemo.h"
#include "m2ScreenQuad.h"
#include "m2Utilities.h"

m2PBODemo::m2PBODemo()
{
	GLbitfield flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
	glGenBuffers(1, &m_pbo);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pbo);
	glBufferStorage(GL_PIXEL_UNPACK_BUFFER, m_imageSize, nullptr, flags | GL_DYNAMIC_STORAGE_BIT);
	m_memory = static_cast<GLubyte*>(glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0, m_imageSize, flags));
	memcpy(m_memory, m_image, m_imageSize);
}

m2PBODemo::~m2PBODemo()
{
	glDeleteBuffers(1, &m_pbo);
}

void m2PBODemo::render()
{
	std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
	upload();
	//Lift elapsed(start); to here for proof that the upload itself is being done asynchronously!
	wait();
	m2Utils::elapsed(start);
	m2ScreenQuad::render();
}

void m2PBODemo::fence()
{
	//Create a new sync point and delete the old one.
	if (m_fence)
		glDeleteSync(m_fence);
	m_fence = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);
}

void m2PBODemo::wait()
{
	//Stall CPU until fence is signalled.
	if (m_fence) {
		while (glClientWaitSync(m_fence, GL_SYNC_FLUSH_COMMANDS_BIT, 1) == GL_TIMEOUT_EXPIRED) {}
	}
}

void m2PBODemo::upload()
{
	memcpy(m_memory, m_image, m_imageSize);	//Aids, but can do this in a separate thread!
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_BGRA, GL_UNSIGNED_BYTE, nullptr);
	fence();
}