#pragma once
#include <glad/glad.h>
//Put stuff like bloom in here. For now I'm just testing stuff that makes use of full screen quads (soon to be full screen triangles)!
class m2Application;
class m2Effects
{
	friend class m2Application;
protected:
	static void render();
private:
	static void init();
	static void shutdown();
};

