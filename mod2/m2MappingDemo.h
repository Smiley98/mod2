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
    std::vector<unsigned char*> m_images;
    GLuint m_texture;
    //GLuint m_textures[5];
    int m_width, m_height;
};
