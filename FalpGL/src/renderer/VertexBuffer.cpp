#include "VertexBuffer.h"
#include "renderer.h"

VertexBuffer::VertexBuffer(unsigned int size, const void* data) // static buffer
    : current_index(0), size(size)
{
    GLCall(glGenBuffers(1, &m_renderer_id));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::VertexBuffer(unsigned int size) // dynamic buffer
    : current_index(0), size(size)
{
    GLCall(glGenBuffers(1, &m_renderer_id));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, NULL, GL_DYNAMIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_renderer_id));
}

void VertexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_renderer_id));
}

void VertexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

unsigned int VertexBuffer::add_quad(const void* data, int count)
{
    Bind();
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, current_index * 24 * sizeof(float), 24 * sizeof(float) * count, data));
    
    unsigned int temp_index = current_index;
    current_index++;
    return temp_index;
}

void VertexBuffer::modify_quad(const void* data, unsigned int index, int count)
{
    Bind();
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * 24 * sizeof(float), 24 * sizeof(float) * count, data));
}

void VertexBuffer::delete_quad(unsigned int index, int count)
{
    Bind();
    GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * 24 * sizeof(float), 24 * sizeof(float) * count, NULL));

    free_indexes.push_back(index);
}

/*
Standard vertex buffer quad:

4 * ([f f] [f f] [f])
size, texture coords, texture index
20 floats

*/