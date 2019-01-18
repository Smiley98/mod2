#pragma once
//This header isn't necessary but all derived components need it so including it here makes sense.
#include "m2ComponentDef.h"
#include "m2MemoryManager.h"
#include <utility>
#include <cstdio>

class m2GameObject;
template<class T>
class m2Component {
public:
	m2Component()  { ++s_activeCount; }
	~m2Component() { --s_activeCount; }

	m2GameObject* parent = nullptr;

	bool active();
	void activate();
	void deactivate();

	static u_int getActiveCount() { return s_activeCount; }

private:
	static u_int s_activeCount;
};

template<class T>
u_int m2Component<T>::s_activeCount;

template<class T>
inline bool m2Component<T>::active()
{
	return this < &m2MemoryManager<T>::at(s_activeCount);
}

template<class T>
inline void m2Component<T>::activate()
{	//Swap this with the first inactive component (s_active count points to the 1st inactive component, so we need postfix increment), increment the counter.
/*#if _DEBUG
	if (active<T>()) {
		printf("Tried to activate a currently active component of %s.\n", typeid(T).name());
		return;
	}
#endif*/
	std::swap(&m2MemoryManager<T>::at(s_activeCount++), this);
}

template<class T>
inline void m2Component<T>::deactivate()
{	//Swap this with the last active component (s_active count points to the 1st inactive component, so we need prefix decrement), decrement the counter.
/*#if _DEBUG
	if (!active<T>()) {
		printf("Tried to deactivate a currently inactive component of %s.\n", typeid(T).name());
		return;
	}
#endif*/
	std::swap(&m2MemoryManager<T>::at(--s_activeCount), this);
}