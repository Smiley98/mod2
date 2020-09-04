#pragma once
#include "m2ArrayObject.h"
#include <vector>

class m2RayRenderer :
	public m2ArrayObject
{
public:
	m2RayRenderer(float xMin, float xMax, unsigned int thickness = 1);
	virtual ~m2RayRenderer();

	void render();

private:
	std::vector<glm::vec4> m_colours;
	std::vector<float> m_heights;

	const float m_xMin, m_xMax;
	const unsigned int m_thickness;
	const unsigned int m_count;
	const float m_step;
	const float m_halfScreenHeight;

	GLuint m_cbo;	//Colour buffer object.
	GLuint m_hbo;	//Height buffer object.
};

