#pragma once
#include "m2Renderer.h"
#include <vector>

//Testing grounds where I can write borderline throwaway code that's concerned with the result rather than software design. (No "m2" prefix).
class TestRenderer :
	public m2Renderer
{
public:
	TestRenderer();
	virtual ~TestRenderer();

	void render() override;

private:
	std::vector<glm::vec4> m_colours;
	std::vector<float> m_heights;
	float m_halfScreenHeight;
	//Hardcode this for now. Can eventually do something like num_rays = screen_resolution.x / ray_thickness, or specify a range ie half res for vertical splitscreen.
	const unsigned int m_numRays = 800;
	//I think this works cause initialization depends on order so since numRays is above this is allowed?
	const float m_step = 1.0f / (float)m_numRays;
	GLuint m_hbo;	//Height buffer object.
	GLuint m_cbo;	//Colour buffer object.
	//unsigned int m_icbo;//Inverse colour buffer object.
};

