#include "m2ComponentManager.h"
#include "m2TransformComponent.h"

bool m2ComponentManager::s_allocated = false;

void m2ComponentManager::allocateContainers(u_int capacity)
{
	assert(!s_allocated);
	m2MemoryManager<m2TransformComponent>::allocate(capacity);
	//m2MemoryManager<m2Renderer>::allocate(capacity);
	//m2MemoryManager<m2Collider>::allocate(capacity);
	s_allocated = true;
}

void m2ComponentManager::allocateMinContainers()
{
	assert(!s_allocated);
	m2MemoryManager<m2TransformComponent>::allocateMin();
	//m2MemoryManager<m2Renderer>::allocateMin();
	//m2MemoryManager<m2Collider>::allocateMin();
	s_allocated = true;
}

void m2ComponentManager::allocateMaxContainers()
{
	assert(!s_allocated);
	m2MemoryManager<m2TransformComponent>::allocateMax();
	//m2MemoryManager<m2Renderer>::allocateMax();
	//m2MemoryManager<m2Collider>::allocateMax();
	s_allocated = true;
}

void m2ComponentManager::deallocateContainers()
{
	assert(s_allocated);
	m2MemoryManager<m2TransformComponent>::deallocate();
	//m2MemoryManager<m2Renderer>::deallocate();
	//m2MemoryManager<m2Collider>::deallocate();
	s_allocated = false;
}
