//Include a precompiled header containing the majority of the standard template library soon!
#pragma once
#include "m2Component.h"
#include <vector>
#include <cstdio>

//GameObject controls the lifetime of components and provides the familiarity of OOP.
class m2GameObject {
public:
	m2GameObject();
	~m2GameObject();

	template<typename T, typename... Args>
	T& addComponent(Args&&...);

	template<typename T>
	T& getComponent();

	template<typename T>
	void removeComponent();

	void activate();
	void deactivate();

	//Makes the passed in object the parent of the callee (*this).
	void setParent(m2GameObject&);
	//Adds the passed in object as a child of the callee (*this).
	void addChild(m2GameObject&);

private:
	//Pointer to parent, pointer cause references suck.
	m2GameObject* m_parent = nullptr;
	//Best remain a vector cause we don't know how many children we'll have. Moreover, we'll rarely access this so make a pointer to it to reduce size of GameObject.
	std::vector<m2GameObject*>* m_children;
	//Since GameObject is memory-critical as it there is the potential things in massive quantities such as particles will be GameObjects, even vectors are avoided.
	void* m_components[m2ComponentType::NUM_COMPONENTS] = { nullptr };
	
	//These are used within templated methods, so their definition must stay within the header too (otherwise scary linker errors)!
	void checkAdd(u_char);
	void checkGet(u_char);
	bool exists(u_char);
	
	inline bool isThisChildOf(m2GameObject&);
};

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
	return *reinterpret_cast<T*>(m_components[index]);
}

template<typename T>
inline void m2GameObject::removeComponent()
{	//printf("External address: %p.\n", &getComponent<T>());
	u_char index = T::getType();
	if (exists(index)) {
		T* component = reinterpret_cast<T*>(m_components[index]);
		component->~T();						//Necessary for cleaning up associated data.
		m2MemoryManager<T>::remove(component);	//Necessary for keeping active components together at the front.
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