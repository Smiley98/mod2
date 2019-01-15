#pragma once
//This header isn't necessary but all derived components need it so including it here makes sense.
#include "m2ComponentDef.h"
class m2GameObject;
class m2Component {
public:
	//Currently no actual reason to make any pure virtual methods because all that derived classes share are members. Behaviours are not polymorphic.

	m2Component() {}			//Pretty sure this gets called by the derived class constructor so I can't throw an assert in here.
	/*virtual*/~m2Component() {}//Virtual destructor currently unnecessary cause no inherited behaviours.

	m2GameObject* parent = nullptr;
	bool active = true;

private:
	//Can't have a static pure virtual method (getType()), so just don't instantiate this.
	//virtual void makeMeAbstract() = 0;
};