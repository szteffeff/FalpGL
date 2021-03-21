#include "IndexBuffer.h"
#include "renderer.h"
#include <vector>
#include <iostream>

IndexBuffer::IndexBuffer(unsigned int count, const unsigned int* data) // count: number of integers in data, data: pointer to buffer
    : m_count(count)
{

    GLCall(glGenBuffers(1, &m_renderer_id));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::IndexBuffer(unsigned int count) //count = number of quads indexed in buffer
    : m_count(count * 6)
{
    std::vector<unsigned int> temp_data;

    for (unsigned int i = 0; i < count * 4; i += 4)
    {
        temp_data.push_back(i);
        temp_data.push_back(i + 1);
        temp_data.push_back(i + 2);
        temp_data.push_back(i + 2);
        temp_data.push_back(i + 3);
        temp_data.push_back(i);
    }

    GLCall(glGenBuffers(1, &m_renderer_id));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * 6 * sizeof(unsigned int), temp_data.data(), GL_DYNAMIC_DRAW));
}
 
IndexBuffer::~IndexBuffer()
{
    GLCall(glDeleteBuffers(1, &m_renderer_id));
}

void IndexBuffer::Bind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_renderer_id));
}

void IndexBuffer::Unbind() const
{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
