#include "m2Application.h"
//Prevent glfw from including the base gl header (gl.h) so glad can include it.
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <thread>
#include <chrono>
#include <cstdio>

#include "m2Window.h"
#include "m2Timing.h"

#include "m2EntityFactory.h"
#include "m2GameObject.h"
#include "m2ComponentManager.h"
#include "m2TransformComponent.h"
#include "m2Utilities.h"

#include "m2Shader.h"
#include "m2Effects.h"
#include "m2RayRenderer.h"
#include "m2RayMarcher.h"

#define FRAMES_PER_SECOND 60.0
#define MILLISECONDS_PER_FRAME 1.0 / FRAMES_PER_SECOND
#define LIMIT_FPS true

float hault(std::chrono::high_resolution_clock::time_point start) {
	using namespace std::chrono;
	float frameTime = duration_cast<duration<float>>(high_resolution_clock::now() - start).count();
#if LIMIT_FPS
	while (frameTime < MILLISECONDS_PER_FRAME) {
		frameTime = duration_cast<duration<float>>(high_resolution_clock::now() - start).count();
	}
#endif
	return frameTime;
}

m2Application::m2Application() :
	m_window(m2Window::instance()),
	m_timing(m2Timing::instance())
{
	m2ComponentManager::init();
	m2ShaderProgram::init();
	m2Effects::init();
}

m2Application::~m2Application()
{
	m2ComponentManager::shutdown();
	m2ShaderProgram::shutdown();
	m2Effects::shutdown();
}

void m2Application::run()
{	//1. Update 2. Render 3. Swap.
	while (m_window.isOpen()) {
		std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();

		update();
		render();
		
		tick(hault(start));
		//Swapping at a fixed interval ensures a smooth framerate. Simply comment above for uncapped fps.
		m_window.swapBuffers();
	}
}

inline void m2Application::update()
{	//Can't run this cause Transform doesn't update.
	//std::thread transforms(m2ComponentManager::updateComponentsOfType<m2TransformComponent>);
	//transforms.join();
	glfwPollEvents();
}

inline void m2Application::render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//m_scene->render();//Scenes should internally switch renderers if I implement such capabilities.

	//Draws a single line. Not yet flexible (doesn't take point arguments).
	//m2ShaderProgram::drawLine();

	static m2RayRenderer rayRenderer(0, m_window.getClientWidth(), 1);
	//rayRenderer.render();
	m2RayMarcher::marchCircle();
}

inline void m2Application::tick(float frameTime)
{
	static float frameTimeAverage = 0.0f;
	static u_int counter = 0;
	frameTimeAverage += frameTime;
	if (counter >= 10) {
		frameTimeAverage /= (float)counter;
		m_timing.m_frameTimeSmoothed = frameTimeAverage;
		frameTimeAverage = 0.0f;
		counter = 0;
	}
	counter++;

	m_timing.m_frameTime = frameTime;
	m_timing.m_elapsedTime += frameTime;
}
