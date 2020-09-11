#pragma once
#include <glad/glad.h>
class m2TextureDemo 
{
public:
    m2TextureDemo();
    ~m2TextureDemo();

    void initialize();
    void render();
    void shutdown();

protected:
    GLuint m_texture = GL_NONE;
    unsigned char* m_image = nullptr;
    int m_width = 0, m_height = 0, m_imageSize = 0;

private:
    void upload();
};