#pragma once

#include <vector>

class VertexBuffer
{
private:
	unsigned int m_renderer_id, size, current_index;
	std::vector<unsigned int> free_indexes;
	bool exists = false;

public:
	VertexBuffer(unsigned int size, const void* data);
	VertexBuffer(unsigned int size);
	VertexBuffer() {}
	~VertexBuffer();

	void init(unsigned int _size);

	unsigned int add_quad(const void* data, int count = 1);
	void modify_quad(const void* data, unsigned int index, int count = 1);
	void delete_quad(unsigned int index, int count = 1);

	void buffer_data(int offset, std::size_t size, const void* data);

	void Bind() const;
	void Unbind() const;
};