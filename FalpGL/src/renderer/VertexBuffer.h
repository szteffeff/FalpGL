#pragma once

#include <vector>

class VertexBuffer
{
private:
	unsigned int m_renderer_id, size, current_index;
	std::vector<unsigned int> free_indexes;


public:
	VertexBuffer(unsigned int size, const void* data);
	VertexBuffer(unsigned int size);
	VertexBuffer() {}
	~VertexBuffer();

	unsigned int add_quad(const void* data, int count = 1);
	void modify_quad(const void* data, unsigned int index, int count = 1);
	void delete_quad(unsigned int index, int count = 1);

	void Bind() const;
	void Unbind() const;
};