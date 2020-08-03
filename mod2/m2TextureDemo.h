#pragma once
#include "m2ArrayObject.h"
#include <vector>

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
    std::vector<unsigned char*> m_images;
    GLuint m_texture;
    int m_width, m_height, m_channels, m_imageSize;

    static const size_t s_imageCount;
};
