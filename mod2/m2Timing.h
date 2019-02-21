#pragma once
#include "m2Singleton.h"
class m2Timing :
	public m2Singleton<m2Timing>
{
	friend class m2Singleton<m2Timing>;
	friend class m2Application;
public:
	~m2Timing();
	float elapsedTime();
	float frameTime();
	float frameTimeSmoothed();

private:
	m2Timing();
	float m_elapsedTime;
	float m_frameTime;
	float m_frameTimeSmoothed;
};

