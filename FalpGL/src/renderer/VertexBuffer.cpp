#include "VertexBuffer.h"
#include "renderer.h"

#include <algorithm>

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

unsigned int VertexBuffer::add_quad(const void* data, int count) /* only accepts count of 1 quad for now...*/
{
    unsigned int index = 0;

    Bind();
    if (!(free_indexes.empty())) /*needs to check if consectutive free quads are availible for count > 1*/
    {
        index = free_indexes[0];
        free_indexes.erase(free_indexes.begin());
    }
    else {
        index = current_index;
        current_index++;
    }

    GLCall(glBufferSubData(GL_ARRAY_BUFFER, index * 24 * sizeof(float), 24 * sizeof(float) * count, data));


    return (index);
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

    std::sort(free_indexes.begin(), free_indexes.end());
}