#include "m2PBODemo.h"
#include "m2ScreenQuad.h"
#include <chrono>
#include <thread>

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
	//wait();
	//memcpy(m_memory, m_images[rand() % s_imageCount], m_imageSize);
	//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_BGR, GL_UNSIGNED_BYTE, nullptr);
	//fence();

	wait();
	memcpy(m_memory, m_images[rand() % s_imageCount], m_imageSize);

	using namespace std::chrono;
	steady_clock::time_point start = steady_clock::now();

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_BGR, GL_UNSIGNED_BYTE, nullptr);
	fence();

	steady_clock::time_point stop = steady_clock::now();

	float elapsed = duration_cast<milliseconds>(stop - start).count();
	printf("%f\n", elapsed);

	std::this_thread::sleep_for(seconds(1));
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
