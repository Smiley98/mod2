#pragma once
template<class T>
class Singleton
{
public:
	//Delete the move constructor and all that crap if you're anal.
	~Singleton() {

	}

	static T& instance() {
		static T inst;
		return inst;
	}

//Gotta be protected so base class can call constructor.
protected:
	Singleton() {}
};

