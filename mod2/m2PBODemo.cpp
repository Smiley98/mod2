#include "m2PBODemo.h"
#include "m2ScreenQuad.h"

m2PBODemo::m2PBODemo()
{
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
}

void m2PBODemo::render()
{
	//The following runs at the same speed as the naive solution because the driver is doing essentially the same thing.
	//Even with 2k textures, it seems persistent memory improves very little. The only possible benefit to PBOs is streaming which isn't demonstrated here.
	for (size_t i = 0; i < 64; i++) {
		//Waits for fence to become signalled.
		//Fences become signalled once prior commands finish ie draw() fence() signals fence after draw completes.
		wait();
		//Thanks to GL_MAP_COHERENT_BIT, the memory is visible to the driver the moment the CPU finishes writing.
		//Since we called wait() we know its safe to write to the memory since OGL is no longer reading it for rendering.
		memcpy(m_memory, m_images[rand() % s_imageCount], m_imageSize);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_BGR, GL_UNSIGNED_BYTE, nullptr);
		fence();
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
	//Stall CPU until fence is signalled.
	if (m_fence) {
		while (glClientWaitSync(m_fence, GL_SYNC_FLUSH_COMMANDS_BIT, 1) == GL_TIMEOUT_EXPIRED) {}
	}
}
