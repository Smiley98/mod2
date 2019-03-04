#pragma once
#include "m2Renderer.h"
#include <vector>

//The values of m2RayCaster are immutable. Must instantiate another instance if you want different values.
//Reason being is making setters means I'll have to come up with logic for buffer redefinition, which is verbose.
//Moreover, changing these values shouldn't happen frequently so making that functionality isn't a good use of time.
class TestRenderer :
	public m2Renderer
{
public:
	TestRenderer(float xMin, float xMax, unsigned int thickness = 1);
	virtual ~TestRenderer();

	void render() override;

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

