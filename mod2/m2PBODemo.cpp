#include "m2PBODemo.h"
#include "m2ScreenQuad.h"

m2PBODemo::m2PBODemo()
{	//initialize() called implicitly.
	GLbitfield flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
	glGenBuffers(1, &m_pbo);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pbo);
	glBufferStorage(GL_PIXEL_UNPACK_BUFFER, m_imageSize, nullptr, flags | GL_DYNAMIC_STORAGE_BIT);
	m_memory = static_cast<GLubyte*>(glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0, m_imageSize, flags));
	printf("Pointer: %p\n", m_memory);
}

m2PBODemo::~m2PBODemo()
{
	glDeleteBuffers(1, &m_pbo);
}	//shutdown() called implicitly.

void m2PBODemo::render()
{
	//The following runs at the same speed as the naive solution because the driver is doing essentially the same thing.
	//for (size_t i = 0; i < 64; i++) {
	//	wait();
	//	memcpy(m_memory, m_images[rand() % s_imageCount], m_imageSize);
	//	fence();
	//	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_BGR, GL_UNSIGNED_BYTE, nullptr);
	//}
	//m2ScreenQuad::render();

	//However, if we can asynchronously transfer data into the PBOs we see a significant performance improvement.
	//Simulate parallelism by memcpying once instead of on a per-image basis as a real application would've streamed images into the PBO in the background.
	memcpy(m_memory, m_images[rand() % s_imageCount], m_imageSize);
	for (size_t i = 0; i < 64; i++)
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_BGR, GL_UNSIGNED_BYTE, nullptr);
	//We have to call finish otherwise the driver may not schedule the uploads till future draw calls!
	glFinish();
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
	//Wait until the fence is signalled.
	if (m_fence) {
		while (glClientWaitSync(m_fence, GL_SYNC_FLUSH_COMMANDS_BIT, 1) == GL_TIMEOUT_EXPIRED) {}
	}
}
