#include "m2GameObject.h"
#include "m2MemDef.h"
#include "m2Transform.h"

bool m2GameObject::s_componentsAllocated = false;

m2GameObject::m2GameObject()
{
	m_components.resize(m2ComponentType::NUM_COMPONENTS);
}

m2GameObject::~m2GameObject()
{
	if (m_components[m2ComponentType::TRANSFORM])
		m2MemoryManager<m2Transform>::remove(reinterpret_cast<void*>(&getComponent<m2Transform>()));

	/*if (m_components[m2ComponentType::RENDERER])
		m2MemoryManager<m2Renderer>::remove(reinterpret_cast<void*>(&getComponent<m2Renderer>()));

	if (m_components[m2ComponentType::COLLIDER])
		m2MemoryManager<m2Collider>::remove(reinterpret_cast<void*>(&getComponent<m2Collider>()));*/
}

void m2GameObject::allocateComponentContainers()
{
	assert(!s_componentsAllocated);
	m2MemoryManager<m2Transform>::initialize(MAX_CONTAINER_SIZE);
	s_componentsAllocated = true;
}

void m2GameObject::deallocateComponentContainers()
{
	assert(s_componentsAllocated);
	m2MemoryManager<m2Transform>::destroy();
	s_componentsAllocated = false;
}
