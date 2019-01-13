#pragma once
#include "m2ComponentDef.h"
class m2GameObject;
class m2Component {
public:
	//Currently no actual reason to make a pure virtual method cause all derived classes share are members. Behaviours are not polymorphic.
	//(Templates / data-oriented programming ftw!)
	//Don't think I can do an assertion or throw an unhandled exception to guarentee this is never instantiated cause I'm pretty sure derived
	//classes implicitly call the base class constructor.
	m2Component() {}
	~m2Component() {}

	m2GameObject* parent = nullptr;
	bool active = true;

private:
	//virtual void makeMeAbstract() = 0;
};