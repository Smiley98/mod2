#pragma once
#define RTTI false

#include "m2MemoryManager.h"
//#include "m2Component.h"
#include <algorithm>
#if RTTI
#include <unordered_map>
#else
#include <vector>
#endif
#if _DEBUG
#include <cstdio>
#endif

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

	template<typename T, typename... Args>
	T& addComponent(Args&&...);

	template<typename T>
	T& getComponent();

	template<typename T>
	void removeComponent();

	bool active = true;

private:
#if RTTI
	std::unordered_map<const type_info*, m2Component*> m_components;
	void addCheck_r(const type_info*);
	void getCheck_r(const type_info*);
#else
	std::vector<m2Component*> m_components = { nullptr };
	void addCheck_c(u_char);
	void getCheck_c(u_char);
#endif

	static bool s_componentsAllocated;

};

template<typename T, typename... Args>
inline T& m2GameObject::addComponent(Args&&... args)
{
	T* component = new (m2MemoryManager<T>::add()) T(args...);
#if RTTI
	const type_info* index = &typeid(T);
	addCheck_r(index);
#else
	u_char index = T::getType();
	addCheck_c(index);
#endif
	m_components[index] = component;
	//component.parent = this;
	return *component;
}

template<typename T>
inline T& m2GameObject::getComponent()
{
#if RTTI
	const type_info* index = &typeid(T);
	getCheck_r(index);
#else
	u_char index = T::getType();
	getCheck_c(index);
#endif
	return reinterpret_cast<T&>(*m_components[index]);
}

template<typename T>
inline void m2GameObject::removeComponent()
{	//Don't think the address of a reference is equal to the actual address. More research needed.
	//Google, "Is the address of a reference the same as the address of its variable"
	m2MemoryManager<T>::remove(reinterpret_cast<void*>(&getComponent<T>()));
}

#if RTTI
inline void m2GameObject::addCheck_r(const type_info* i)
{
#if _DEBUG
	if (m_components.count(i) != 0)
		printf("Error: attempted to insert an additional %s\n", i->name());
#endif
}

inline void m2GameObject::getCheck_r(const type_info* i)
{
#if _DEBUG
	if (m_components.count(i) == 0)
		printf("Error: attempted to access a non-existent %s\n", i->name());
#endif
}

#else
inline void m2GameObject::addCheck_c(u_char i)
{
#if _DEBUG
	if (m_components[i] != nullptr)
		printf("Error: attempted to insert and additional Component of type %i\n", i);
#endif
}

inline void m2GameObject::getCheck_c(u_char i)
{
#if _DEBUG
	if (m_components[i] == nullptr)
		printf("Error: attempted to access a non-existent Component of type %i\n", i);
#endif
}
#endif
