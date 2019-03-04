#pragma once
#include "m2Singleton.h"

struct Rect {
	union {
		struct {
			int x, y, w, h;
		};
		struct {
			int values[4];
		};
	};
};

struct GLFWwindow;
class m2Window :
	public m2Singleton<m2Window>
{
	friend class m2Singleton<m2Window>;
	friend void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
	friend void framebuffer_callback(GLFWwindow* window, int key, int scancode);
public:
	~m2Window();

	bool isOpen();

	//Sends a close event. Does not force-close. Force-closing won't be a thing.
	void close();

	//Polls events and swaps buffers.
	void swapBuffers();

	//Wrapper for glViewport.
	void viewport(int x, int y, int w, int h);
	Rect getClientRect();
	//Rect getWindowRect();
	//No window or client setters. Our application has no need to do that! Leave modification to callbacks.

	int getClientX();
	int getClientY();
	int getClientWidth();
	int getClientHeight();

	//Our application has no need to manipulate the actual window values. Use the client instead cause we're only concerned with the display.
	//int getWindowX();
	//int getWindowY();
	//int getWindowWidth();
	//int getWindowHeight();

private:
	m2Window();
	GLFWwindow* m_window;

	//int m_windowX		= 0;
	//int m_windowY		= 0;
	//int m_windowWidth	= 0;
	//int m_windowHeight= 0;

	int m_clientX = 0;
	int m_clientY = 0;
	int m_clientWidth = 800;
	int m_clientHeight = 800;
};