#pragma once
#include "m2TextureDemo.h"
class m2PBODemo :
    public m2TextureDemo
{
public:
    m2PBODemo();
    virtual ~m2PBODemo() override;
    virtual void render() override;

private:
    GLuint m_bufferStorage = 0;
    unsigned char* m_memoryRegion = nullptr;
    GLsync m_fence = nullptr;

    void fence();
    void wait();
};

