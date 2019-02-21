#include "m2Window.h"
//Prevent glfw from including the base gl header (gl.h) so glad can include it.
#define GLFW_INCLUDE_NONE
#pragma comment (lib, "glfw3.lib")
#pragma comment (lib, "opengl32.lib")

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <cstdio>

m2Window::m2Window() : m_window(nullptr)
{
	glfwInit();
	//glfwWindowHint()
	m_window = glfwCreateWindow(800, 800, "Window", nullptr, nullptr);
	glfwMakeContextCurrent(m_window);

	gladLoadGL();
	printf("Window initialized.\n");
}

m2Window::~m2Window()
{
	glfwTerminate();
	printf("Window terminated.\n");
}

void m2Window::close()
{
	glfwDestroyWindow(m_window);
}

void m2Window::update()
{
	glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
	while (!glfwWindowShouldClose(m_window)) {
		glfwPollEvents();
	}
	glfwSwapBuffers(m_window);
}
