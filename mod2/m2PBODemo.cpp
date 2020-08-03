#include "m2PBODemo.h"
#include "m2ScreenQuad.h"

m2PBODemo::m2PBODemo()
{
	initialize();
}

m2PBODemo::~m2PBODemo()
{
	shutdown();
}

void m2PBODemo::render()
{
	m2ScreenQuad::render();
}
