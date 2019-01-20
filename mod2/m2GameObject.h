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

	//Not in use. Might wanna consider Making a method that does this. Between this and the destructor, referencing each Component<T> seems to be a
	//frequent occurance. Perhaps I can do some template nonsense where I somehow pass the method name as a parameter and each specialization executes it??
	//Maybe take in a function pointer which takes in a variadic template so I can do fptr<T>(args...); for each component.
	//Search, "How to make a function pointer that takes a variadic template".
	//bool active = true;

private:
	//void* memes = { nullptr };	//Can't use [] to make a variable without also using new. Dynamic arrays may be less legible, but also less ambiguous.
	std::vector<void*> m_components = { nullptr };//*NOTE* If I switch to dynamic arrays, I will need to make void** cause its an array of pointers.
	void checkAdd(u_char);
	void checkGet(u_char);
	bool exists(u_char);
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
