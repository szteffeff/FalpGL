 #pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define DEBUG

bool GLLogCall();

#ifdef DEBUG
    #define ASSERT(x) if (!(x)) __debugbreak();
    #define GLCall(x) GLClearError();\
        x;\
        ASSERT(GLLogCall())\
//#else

    //#define GLCall(x) x

#endif // DEBUG




void GLClearError();




class Renderer
{
public:
    void clear() const;
    void draw(const VertexArray& vertex_array, const IndexBuffer& index_buffer, const Shader& shader) const;
};