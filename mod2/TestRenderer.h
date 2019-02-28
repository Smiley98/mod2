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
	std::vector<glm::vec3> m_colours;
	std::vector<float> m_heights;
	GLsizei m_numElements = 128;
	unsigned int m_cbo;	//Colour buffer object.
	unsigned int m_icbo;//Inverse colour buffer object.
	unsigned int m_hbo;	//Height buffer object.
};

