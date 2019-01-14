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
	printf("Destructor called.\n");
	if (m_components[m2ComponentType::TRANSFORM])
		removeComponent<m2Transform>();

	/*if (m_components[m2ComponentType::RENDERER])
		removeComponent<m2Renderer>();

	if (m_components[m2ComponentType::COLLIDER])
		removeComponent<m2Collider>();*/
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
