#include "m2Window.h"
//Prevent glfw from including the base gl header (gl.h) so glad can include it.
#define GLFW_INCLUDE_NONE
#pragma comment (lib, "glfw3.lib")
#pragma comment (lib, "opengl32.lib")

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <cstdio>

void error_callback(int error, const char* description)
{
	fprintf(stderr, "GLFW error: %s\n", description);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void framebuffer_callback(GLFWwindow* window, int width, int height) {
	m2Window& w = m2Window::instance();
	w.m_clientWidth = width;
	w.m_clientHeight = height;
	glViewport(w.m_clientX, w.m_clientY, width, height);
	printf("Client resized to %ix%i.\n", width, height);
}

void focus_callback(GLFWwindow* window, int param) {

}

m2Window::m2Window() : m_window(nullptr)
{
	//1. Initialize window and other primary initial glfw code like setting the error callback.
	glfwInit();
	glfwSetErrorCallback(error_callback);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	
	GLFWmonitor& monitor = *glfwGetPrimaryMonitor();
	const GLFWvidmode& vm = *glfwGetVideoMode(&monitor);
	m_window = glfwCreateWindow(vm.width, vm.height, "PRIMEOPS 2", &monitor, nullptr);
	//m_window = glfwCreateWindow(800, 800, "PRIMEOPS 2", nullptr, nullptr);
	//int wwidth, wheight;
	//glfwGetWindowSize(m_window, &wwidth, &wheight);
	//printf("Window size: %ix%i.\n", wwidth, wheight);
	glfwSetWindowPos(m_window, 0, 0);

	//2. Obtain modern gl.
	glfwMakeContextCurrent(m_window);
	gladLoadGL();

	//3. Set set callbacks for events. 
	glfwSetKeyCallback(m_window, key_callback);
	//Causing errors at the moment, as with a lot of other windowing nonsense...
	//glfwSetFramebufferSizeCallback(m_window, framebuffer_callback);
	//glfwSetWindowFocusCallback(m_window, focus_callback);

	//int fwidth, fheight;
	//glfwGetFramebufferSize(m_window, &fwidth, &fheight);
	//printf("Framebuffer size: %ix%i.\n", fwidth, fheight);

	//4. *IMPORTANT* Set the visible area to match the client rather than the window. The client is essentially the "canvas" whereas the window contains stuff we don't want like boarders and scroll bars! 
	glfwGetFramebufferSize(m_window, &m_clientWidth, &m_clientHeight);
	glViewport(0, 0, m_clientWidth, m_clientHeight);

	//5. Set the interval at which the buffers swap. Double buffer by default, but at an interval of 0 (which leads to screen tear).
	glfwSwapInterval(1);
}

m2Window::~m2Window()
{
	glfwTerminate();
}

bool m2Window::isOpen()
{
	return !glfwWindowShouldClose(m_window);
}

void m2Window::close()
{
	glfwSetWindowShouldClose(m_window, 1);
}

void m2Window::swapBuffers()
{
	glfwSwapBuffers(m_window);
}

int m2Window::getClientX()
{
	return m_clientX;
}

int m2Window::getClientY()
{
	return m_clientY;
}

int m2Window::getClientWidth()
{
	return m_clientWidth;
}

int m2Window::getClientHeight()
{
	return m_clientHeight;
}

void m2Window::viewport(int x, int y, int w, int h)
{
	glViewport(x, y, w, h);
	m_clientX = x;
	m_clientY = y;
	m_clientWidth = w;
	m_clientHeight = h;
}

Rect m2Window::getClientRect()
{
	return Rect{ m_clientX, m_clientY, m_clientWidth, m_clientHeight };
}

/*Rect m2Window::getWindowRect()
{
	return Rect{ m_windowX, m_windowY, m_windowWidth, m_windowHeight };
}

int m2Window::getWindowX()
{
	return m_windowX;
}

int m2Window::getWindowY()
{
	return m_windowY;
}

int m2Window::getWindowWidth()
{
	return m_windowWidth;
}

int m2Window::getWindowHeight()
{
	return m_windowHeight;
}*/
