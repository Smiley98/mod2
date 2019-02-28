#pragma once
#include "m2Renderer.h"
class m2LineRenderer :
	public m2Renderer
{
public:
	m2LineRenderer();
	~m2LineRenderer();

	void render() override;

private:

};

