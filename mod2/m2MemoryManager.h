#include "m2TypeDef.h"
//#include "m2MemDef.h"//MemDef not needed since everything has been reverted from non-static to static.
#include <memory>
#include <assert.h>

template<typename T>
class m2MemoryManager {
public:
	/*m2MemoryManager() {
		initialize(MIN_CONTAINER_SIZE);
	}

	m2MemoryManager(u_int capacity) {
		initialize(capacity);
	}

	~m2MemoryManager() {
		destroy();
	}*/

	static void initialize(u_int capacity) {
		assert(capacity > 0);
		m_bytesPerElement = sizeof(T);
		_resize(capacity);
	}

	static void destroy() {
		_clear();
		free(m_memory);
		printf("Removed all %s.\n", typeid(T).name());
	}

	static void resize(u_int capacity) {
		assert(capacity > 0);
		//If we want to shrink the container:
		if (m_index > capacity) {
			u_int difference = m_index - capacity;
			for (; difference > 0; difference--)
				m_memory[m_index - difference].~T();
			m_index = capacity;
		}
		_resize(capacity);
	}

	static T* data() {
		return m_memory;
	}

	static T& at(u_int index) {
		return m_memory[index];
	}

	//Clears all data (calls destructors) and marks the entire container as unoccupied.
	static void clear() {
		_clear();
		m_index = 0U;
	}

	static void* add() {
		if (m_index >= m_capacity)
			_resize(m_capacity * 2U);
		//printf("Adding object at index %p.\n", m_memory + m_index);
		return m_memory + m_index++;
	}
	
	static void remove(void* memory) {
		//Overwrites the passed-in location with the last occupied element.
		//printf("Removing object at index %p.\n(Passed in address = %p)\n", m_memory + m_index - 1, memory);
		memmove(memory, m_memory + --m_index, m_bytesPerElement);
	}

	static u_int capacity() {
		return m_capacity;
	}

	static u_int size() {
		return m_index;
	}

private:
	static T* m_memory;		//Generic array.
	static size_t m_bytesPerElement;
	static u_int m_index;		//Next open slot.
	static u_int m_capacity;	//Size of the array.

	static inline void _resize(u_int capacity) {
		m_memory = reinterpret_cast<T*>(realloc(m_memory, capacity * m_bytesPerElement));
		m_capacity = capacity;
	}
	
	static inline void _clear() {
		for (u_int i = 0; i < m_index; i++)
			m_memory[i].~T();
	}
};

template<typename T>
T* m2MemoryManager<T>::m_memory;

template<typename T>
size_t m2MemoryManager<T>::m_bytesPerElement;

template<typename T>
u_int m2MemoryManager<T>::m_index;

template<typename T>
u_int m2MemoryManager<T>::m_capacity;