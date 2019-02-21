#pragma once
//Forward declaring won't shorten compile times cause nothing includes m2Application, but it does make it clear which dependencies are in use.
class m2Window;
class m2Timing;
class m2Application
{
public:
	m2Application();
	~m2Application();

	void run();

private:
	m2Window& m_window;
	m2Timing& m_timing;

	inline void update();
	inline void render();
	inline void tick(float);
};

