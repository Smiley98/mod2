#pragma once
#include "m2ArrayObject.h"
#include <vector>

class m2MappingDemo : 
    public m2ArrayObject
{
public:
    m2MappingDemo();
    ~m2MappingDemo();

    void render();

private:
    unsigned char* m_test;
    std::vector<unsigned char*> m_images;
};
