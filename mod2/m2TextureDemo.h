#pragma once
#include "m2ArrayObject.h"
#include <vector>
#include <chrono>

class m2TextureDemo : 
    public m2ArrayObject
{
public:
    m2TextureDemo();
    ~m2TextureDemo();

    void initialize();
    virtual void render();
    void shutdown();

protected:
    GLuint m_texture = GL_NONE;
    unsigned char* m_image = nullptr;
    int m_width = 0, m_height = 0, m_imageSize = 0;

    void upload();
    void elapsed(std::chrono::steady_clock::time_point start);
};
