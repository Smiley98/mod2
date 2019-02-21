#include "m2Timing.h"

m2Timing::m2Timing()
{
}

m2Timing::~m2Timing()
{
}

float m2Timing::elapsedTime()
{
	return m_elapsedTime;
}

float m2Timing::frameTime()
{
	return m_frameTime;
}

float m2Timing::frameTimeSmoothed()
{
	return m_frameTimeSmoothed;
}
