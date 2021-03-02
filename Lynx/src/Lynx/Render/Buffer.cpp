#include "Lynxpch.h"
#include "Buffer.h"
#include <glad/glad.h>

namespace Lynx {

	uint32_t BufferElement::GetElementCount() const
	{
		switch (Type) {
		case ShaderDataType::Float:		return 1;
		case ShaderDataType::Float2:	return 2;
		case ShaderDataType::Float3:	return 3;
		case ShaderDataType::Float4:	return 4;
		case ShaderDataType::Mat3:		return 3 * 3;
		case ShaderDataType::Mat4:		return 4 * 4;
		case ShaderDataType::Int:		return 1;
		case ShaderDataType::Int2:		return 2;
		case ShaderDataType::Int3:		return 3;
		case ShaderDataType::Int4:		return 4;
		case ShaderDataType::Bool:		return 1;
		}
		return 0;
	}

	void BufferLayout::CalculateOffsetAndStride()
	{
		uint32_t offset = 0;
		m_Stride = 0;
		for (auto& element : m_Elements) {
			element.Offset = offset;
			offset += element.Size;
			m_Stride += element.Size;
		}
	}

	/////////////////////////////////////////////////////////////////
	// VertexBuffer /////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////

	VertexBuffer::VertexBuffer(size_t size)
	{
		//LX_PROFILE_FUNCTION();
		glCreateBuffers(1, &m_VertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
	}

	VertexBuffer::VertexBuffer(const void* vertices, size_t size)
	{
		//LX_PROFILE_FUNCTION();
		glCreateBuffers(1, &m_VertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

	}

	Ref<VertexBuffer> VertexBuffer::Create(size_t size)
	{
		return Ref<VertexBuffer>(new VertexBuffer(size));
	}

	Ref<VertexBuffer> VertexBuffer::Create(const void* vertices, size_t size)
	{
		return Ref<VertexBuffer>(new VertexBuffer(vertices, size));
	}

	VertexBuffer::~VertexBuffer()
	{
		//LX_PROFILE_FUNCTION();
		glDeleteBuffers(1, &m_VertexBufferID);
	}

	void VertexBuffer::Bind() const
	{
		//LX_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
	}

	void VertexBuffer::Unbind() const
	{
		//LX_PROFILE_FUNCTION();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::SetData(const void* data, size_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
	}

	void VertexBuffer::Test(const void* vertices, size_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
	}

	/////////////////////////////////////////////////////////////////
	// IndexBuffer //////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////

	IndexBuffer::IndexBuffer(int* indices, int count)
		: m_Count(count)
	{
		//LX_PROFILE_FUNCTION();
		glCreateBuffers(1, &m_IndexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, m_IndexBufferID);
		glBufferData(GL_ARRAY_BUFFER, count * (int)sizeof(int), indices, GL_STATIC_DRAW);

	}

	Ref<IndexBuffer> IndexBuffer::Create(int* indices, int count)
	{
		return Ref<IndexBuffer>(new IndexBuffer(indices, count));
	}

	IndexBuffer::~IndexBuffer()
	{
		//LX_PROFILE_FUNCTION();
		glDeleteBuffers(1, &m_IndexBufferID);
	}

	void IndexBuffer::Bind() const
	{
		//LX_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexBufferID);
	}

	void IndexBuffer::Unbind() const
	{
		//LX_PROFILE_FUNCTION();
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}