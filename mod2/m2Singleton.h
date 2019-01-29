#pragma once
template<class T>
class m2Singleton
{
public:
	//Delete the move constructor and all that crap if you're anal.
	~m2Singleton() {}

	static T& instance() {
		static T inst;
		return inst;
	}

protected:
	//Protected so base class can call constructor.
	m2Singleton() {}
};

