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

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

m2Window::m2Window() : m_window(nullptr)
{
	glfwInit();
	//glfwWindowHint()
	glfwSetErrorCallback(error_callback);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	m_window = glfwCreateWindow(800, 800, "Window", nullptr, nullptr);
	glfwMakeContextCurrent(m_window);
	glfwSetWindowPos(m_window, 0, 0);
	
	gladLoadGL();

	glfwSetKeyCallback(m_window, key_callback);
	int width, height;
	glfwGetFramebufferSize(m_window, &width, &height);
	glViewport(0, 0, width, height);
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
