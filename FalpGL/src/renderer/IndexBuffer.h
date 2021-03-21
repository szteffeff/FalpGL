#pragma once


class IndexBuffer
{
private:
	unsigned int m_renderer_id;
	unsigned int m_count;
public:
	IndexBuffer(unsigned int count, const unsigned int* data);
	IndexBuffer(unsigned int count);
	IndexBuffer() {}
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_count; }
};