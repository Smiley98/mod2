#include "m2PBODemo.h"
#include "m2ScreenQuad.h"

m2PBODemo::m2PBODemo()
{
	initialize();
	GLbitfield flags = GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT;
	glGenBuffers(1, &m_bufferStorage);
	glBufferStorage(GL_PIXEL_UNPACK_BUFFER, m_imageSize, nullptr, flags | GL_DYNAMIC_STORAGE_BIT);
	m_memoryRegion = (unsigned char*)glMapBufferRange(GL_PIXEL_UNPACK_BUFFER, 0, m_imageSize, flags);
	glFlush();
}

m2PBODemo::~m2PBODemo()
{
	shutdown();
}

void m2PBODemo::render()
{
	fence();

	glFlush();
	wait();

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
