#pragma once
#include <glad/glad.h>
#include <string>

class m2Texture
{
public:
	void initialize(const std::string& fileName);
	void shutdown();

	//Need this as a separate method so I can verify asynchronous dispatch of GL calls...
	//Should be part of uploadBegin() but at the same time can be done in the background so this does serve as a nice reminder.
	void copy();
	void uploadBegin();
	void uploadEnd();

	void downloadBegin();
	void downloadEnd();

private:
	GLuint m_texture = GL_NONE;
	unsigned char* m_image = nullptr;

	int m_width = 0, m_height = 0, m_imageSize = 0;

	GLuint m_uploadPBO = GL_NONE;
	GLsync m_uploadFence = GL_NONE;
	GLubyte* m_uploadMemory = nullptr;

	//Not dealing with downloading atm.
	//GLuint m_downloadPBO = GL_NONE;
	//GLsync m_downloadFence = GL_NONE;
	//GLubyte* m_downloadMemory = nullptr;

	void fence(GLsync& sync);
	void wait(const GLsync& sync);
};