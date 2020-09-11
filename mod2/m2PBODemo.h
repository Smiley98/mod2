#pragma once
#include "m2TextureDemo.h"
class m2PBODemo :
    public m2TextureDemo
{
public:
    m2PBODemo();
    ~m2PBODemo();
    void render();

private:
    GLuint m_pbo = GL_NONE;
    GLubyte* m_memory = nullptr;
    GLsync m_fence = nullptr;
    
    void fence();
    void wait();
    
    void upload();
};
