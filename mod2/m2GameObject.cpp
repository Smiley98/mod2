#include "m2GameObject.h"
#include "m2MemDef.h"
#include "m2Transform.h"

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