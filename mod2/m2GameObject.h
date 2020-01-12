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
	T& addComponent(Args&&...);//rvalue reference because constructor calls are usually with literals.
	//I'm kind of confused. What would happen if we passed something other than an rvalue reference? Would it be okay?

	template<typename T>
	T& getComponent();

	template<typename T>
	void removeComponent();

	//These can't exist as far as I'm concerned. Activate/deactivate are based on MM<T> so we can't even cast to something like base or null component.
	//void activate();
	//void deactivate();

	//Makes the passed in object the parent of the callee (*this).
	void setParent(m2GameObject&);
	//Adds the passed in object as a child of the callee (*this).
	void addChild(m2GameObject&);
	//We don't want to give the programmer the ability to remove children.

	//This can work as long as we don't include the transform m_components. Probably better this way cause we're saving space.
	//m2TransformComponent& transform;
	//Actually, just make this a member.
	//m2Transform transform;

private:
	//Pointer to parent, pointer cause references suck.
	m2GameObject* m_parent = nullptr;
	//Best remain a vector cause we don't know how many children we'll have. Moreover, we'll rarely access this so make a pointer to it to reduce size of GameObject.
	std::vector<m2GameObject*>* m_children;
	//Since GameObject is memory-critical as it there is the potential things in massive quantities such as particles will be GameObjects, even vectors are avoided.
	void* m_components[m2ComponentType::NUM_COMPONENTS] = { nullptr };//(Automatically deallocated despite being an array of pointers).
	
	//These are used within templated methods, so their definition must stay within the header too (otherwise scary linker errors)!
	void checkAdd(u_char);
	void checkGet(u_char);
	bool componentExists(u_char);
	
	inline bool isThisChildOf(m2GameObject&);
};

template<typename T, typename... Args>
inline T& m2GameObject::addComponent(Args&&... args)
{	//In addition to weak pointers, we can use the index to choose which array of components (ie colliders vs renderers) to choose from. Just cast internally.
	T* component = new (m2MemoryManager<T>::add()) T(args...);//Instead of mm.add(), vector.push_back. More safe and more legible.

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
	//m_components are the weak pointers. addComponent<T>() and removeComponent<T>() are what the revamp is concerned with.
	return *reinterpret_cast<T*>(m_components[index]);
}

template<typename T>
inline void m2GameObject::removeComponent()
{	//printf("External address: %p.\n", &getComponent<T>());
	u_char index = T::getType();
	if (componentExists(index)) {
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

inline bool m2GameObject::componentExists(u_char i)
{
	return m_components[i] != nullptr;
}