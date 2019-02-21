#pragma once
template<class T>
class m2Singleton
{
public:
	m2Singleton(T&&) = delete;
	m2Singleton(const T&) = delete;
	m2Singleton& operator=(T&&) = delete;
	m2Singleton& operator=(const T&) = delete;

	static T& instance();

protected:
	m2Singleton();
	virtual ~m2Singleton();
};

template<class T>
inline T& m2Singleton<T>::instance()
{
	static T inst;
	return inst;
}

template<class T>
inline m2Singleton<T>::~m2Singleton()
{
}

template<class T>
inline m2Singleton<T>::m2Singleton()
{
}
