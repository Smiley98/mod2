#pragma once
#include "m2Singleton.h"

class m2ShaderStore :
	public m2Singleton<m2ShaderStore>
{
	friend class m2Singleton<m2ShaderStore>;
public:
	m2ShaderStore();
	~m2ShaderStore();

private:

};

