#pragma once
#include "m2Singleton.h"

struct GLFWwindow;
class m2Window :
	public m2Singleton<m2Window>
{	//Not sure why friendship is necessary, but it is.
	friend class m2Singleton<m2Window>;
public:
	~m2Window();

	bool isOpen();

	//Sends a close event. Does not force-close. Force-closing won't be a thing.
	void close();

	//Polls events and swaps buffers.
	void swapBuffers();

private:
	m2Window();
	GLFWwindow* m_window;
};

