#include "m2GameObject.h"
#include "m2MemDef.h"
#include "m2Transform.h"
#include <algorithm>

m2GameObject::m2GameObject()
{
	//m_components.resize(m2ComponentType::NUM_COMPONENTS);
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

void m2GameObject::activate()
{
}

void m2GameObject::deactivate()
{
}

void m2GameObject::setParent(m2GameObject& parent)
{
	if (isThisChildOf(parent)) {
#if _DEBUG	//This log could be more descriptive.
		printf("Tried to parent this but it was already a child of the target.\n");
#endif
		return;
	}
	*m_parent = parent;
	parent.m_children->push_back(this);
}

void m2GameObject::addChild(m2GameObject& child)
{
	if (child.isThisChildOf(*this)) {
#if _DEBUG	//This log could be more descriptive.
		printf("Tried to add child to parent but the parent already owns the desired child.\n");
#endif
		return;
	}
	child.m_parent = this;
	m_children->push_back(&child);
}

inline bool m2GameObject::isThisChildOf(m2GameObject& gameObject)
{	//Idk why but I couldn't get this to work without creating the "childList" variable. No complaints though.
	std::vector<m2GameObject*>& childrenList = *gameObject.m_children;
	for (u_char i = 0; i < childrenList.size(); i++) {
		if(childrenList[i] == this)
			return true;
	}
	return false;
}