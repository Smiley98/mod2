#include "m2PBODemo.h"
#include "m2ScreenQuad.h"
#include <thread>

m2PBODemo::m2PBODemo()
{
	initialize();

	GLbitfield flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
	glGenBuffers(1, &m_pbo);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pbo);
	glBufferStorage(GL_PIXEL_UNPACK_BUFFER, m_imageSize, nullptr, flags | GL_DYNAMIC_STORAGE_BIT);
	m_memory = static_cast<GLubyte*>(glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0, m_imageSize, flags));
	printf("Pointer: %p\n", m_memory);
	glFlush();
	glFinish();

	//We can actually cache our data in s_imageCount number of PBOs so the transfer is exclusively on the GPU.
	//for all images, copy to PBO once, then bind PBO corresponding to random number at runtime!
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, GL_NONE);
}

m2PBODemo::~m2PBODemo()
{
	glDeleteBuffers(1, &m_pbo);
	shutdown();
}

void m2PBODemo::render()
{
	//Make sure the GPU doesn't read while we're writing to here.
	//wait();
	//std::thread clientTransfer([this] { memcpy(m_memory, m_images[rand() % s_imageCount], m_imageSize); });
	//clientTransfer.join();
	//
	////Add fence here before issuing calls we'll want to wait for before next frame's write.
	//fence();
	//for (size_t i = 0; i < 100; i++)
	//	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_BGR, GL_UNSIGNED_BYTE, nullptr);
	//
	//m2ScreenQuad::render();

	for (size_t i = 0; i < 100; i++) {
		memcpy(m_images[0], m_images[rand() % s_imageCount], m_imageSize);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_BGR, GL_UNSIGNED_BYTE, m_images[0]);
	}
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
