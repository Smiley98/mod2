#pragma once
#include "m2Component.h"
class m2ComponentManager {
public:
	static void allocateContainers(u_int);
	static void allocateMinContainers();
	static void allocateMaxContainers();
	static void deallocateContainers();
	//We don't want a destroyContainers() method because Component<T> destruction should be soley the responsibility of GameObject.

	template<typename T>
	static void updateComponentsOfType();

	//We don't want an "updateAllComponents()" method because we want to be able to create different scenes which may require different update sequences.

private:
	static bool s_allocated;
};

template<typename T>
inline void m2ComponentManager::updateComponentsOfType()
{
	T* components = m2MemoryManager<T>::data();
	for (u_int i = 0U; i < m2Component<T>::getActiveCount(); i++) {
		components[i].update();
	}
}