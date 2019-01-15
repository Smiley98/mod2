#pragma once

#include "m2MemoryManager.h"
//#include "m2Component.h"
//Include a precompiled header containing the majority of the standard template library soon!
#include <algorithm>
#include <vector>
#include <cstdio>

class m2Component;
class m2GameObject
{
	//All GameObject really does is control the lifetime of components. It also makes the code really organized cause it stores
	//any components which can be retrieved later by specifying a template parameter (super powerful). Overall, worth!
public:
	m2GameObject();
	~m2GameObject();

	static void allocateComponentContainers();
	static void deallocateComponentContainers();

	template<typename T>
	static void update();

	template<typename T, typename... Args>
	T& addComponent(Args&&...);

	template<typename T>
	T& getComponent();

	template<typename T>
	void removeComponent();

	bool active = true;

private:
	std::vector<m2Component*> m_components = { nullptr };
	void checkAdd(u_char);
	void checkGet(u_char);
	bool exists(u_char);

	static bool s_componentsAllocated;

};

template<typename T>
inline void m2GameObject::update()
{
	T* components = m2MemoryManager<T>::data();
	for (u_int i = 0U; i < m2MemoryManager<T>::size(); i++) {
		components[i].update();
	}
}

template<typename T, typename... Args>
inline T& m2GameObject::addComponent(Args&&... args)
{
	T* component = new (m2MemoryManager<T>::add()) T(args...);

	u_char index = T::getType();
	checkAdd(index);

	m_components[index] = component;
	component->parent = this;

	return *component;
}

template<typename T>
inline T& m2GameObject::getComponent()
{	//printf("Internal address: %p.\n", m_components[index]);
	u_char index = T::getType();
	checkGet(index);
	return reinterpret_cast<T&>(*m_components[index]);
}

template<typename T>
inline void m2GameObject::removeComponent()
{	//printf("External address: %p.\n", &getComponent<T>());
	u_char index = T::getType();
	if (exists(index)) {
		T* component = reinterpret_cast<T*>(m_components[index]);
		component->~T();
		m2MemoryManager<T>::remove(reinterpret_cast<void*>(component));
		component = nullptr;
	}
#if _DEBUG
	else
		printf("Error: attempted to remove a non-existent Component of type %i\n", index);
#endif
}

inline void m2GameObject::checkAdd(u_char i)
{
#if _DEBUG
	if (m_components[i] != nullptr)
		printf("Error: attempted to insert and additional Component of type %i\n", i);
#endif
}

inline void m2GameObject::checkGet(u_char i)
{
#if _DEBUG
	if (m_components[i] == nullptr)
		printf("Error: attempted to access a non-existent Component of type %i\n", i);
#endif
}

inline bool m2GameObject::exists(u_char i)
{
	if (m_components[i] == nullptr)
		return false;
	return true;
}
