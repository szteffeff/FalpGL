#include "VertexArray.h"
#include "renderer.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_renderer_id));
	console_log("[INFO] Created vertex array: " + std::to_string(m_renderer_id));
}

VertexArray::~VertexArray()
{
	console_log("[INFO] Destroyed vertex array: " + std::to_string(m_renderer_id));
	GLCall(glDeleteVertexArrays(1, &m_renderer_id));
}

void VertexArray::Bind() const
{
	GLCall(glBindVertexArray(m_renderer_id));
}

void VertexArray::Unbind() const
{
	GLCall(glBindVertexArray(0));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++)
	{
		const auto& element = elements[i];
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count * VertxBufferElement::GetSizeOfType(element.type);
	}

}
