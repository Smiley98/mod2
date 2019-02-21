#pragma once
#include "m2Singleton.h"

struct GLFWwindow;
class m2Window :
	public m2Singleton<m2Window>
{	//Not sure why friendship is necessary, but it is.
	friend class m2Singleton<m2Window>;
public:
	~m2Window();
	void close();

	void update();

	m2Window();
private:
	GLFWwindow* m_window;
};

