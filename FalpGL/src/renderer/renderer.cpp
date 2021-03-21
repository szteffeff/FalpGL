#include "renderer.h"
#include <iostream>

void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall()
{
    while (GLenum error = glGetError())
    {
        std::cout << "[OpenGL Error]" << "{ " << error << " }  "<<std::endl;
        return false;
    }
    return true;
}

void Renderer::clear() const
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::draw(const VertexArray& vertex_array, const IndexBuffer& index_buffer, const Shader& shader) const
{
    vertex_array.Bind();
    index_buffer.Bind();
    shader.Bind();
    GLCall(glDrawElements(GL_TRIANGLES, index_buffer.GetCount() , GL_UNSIGNED_INT, nullptr));


}
