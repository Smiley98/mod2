#include "m2GameObject.h"
#include "m2MemDef.h"
#include "m2Transform.h"

bool m2GameObject::s_componentsAllocated = false;

m2GameObject::m2GameObject()
{
	m_components.resize(m2ComponentType::NUM_COMPONENTS);
}

//*Have to delete before deallocateComponentContainers() is called otherwise MM will call memmove() on garbage.
m2GameObject::~m2GameObject()
{
	if (exists(m2ComponentType::TRANSFORM))
		removeComponent<m2Transform>();

	/*if (exists(m2ComponentType::RENDERER))
		removeComponent<m2Renderer>();

	if (exists(m2ComponentType::COLLIDER))
		removeComponent<m2Collider>();*/
}

void m2GameObject::allocateComponentContainers()
{
	assert(!s_componentsAllocated);
	m2MemoryManager<m2Transform>::initialize(MAX_CONTAINER_SIZE);
	//m2MemoryManager<m2Renderer>::initialize(MAX_CONTAINER_SIZE);
	//m2MemoryManager<m2Collider>::initialize(MAX_CONTAINER_SIZE);
	s_componentsAllocated = true;
}

void m2GameObject::deallocateComponentContainers()
{
	assert(s_componentsAllocated);
	m2MemoryManager<m2Transform>::deallocate();
	//m2MemoryManager<m2Renderer>::deallocate();
	//m2MemoryManager<m2Collider>::deallocate();
	s_componentsAllocated = false;
}
