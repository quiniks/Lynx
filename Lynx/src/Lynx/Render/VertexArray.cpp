#include "Lynxpch.h"
#include "VertexArray.h"
#include <glad/glad.h>

namespace Lynx {
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
		switch (type) {
		case ShaderDataType::Float:	return GL_FLOAT;
		case ShaderDataType::Float2:return GL_FLOAT;
		case ShaderDataType::Float3:return GL_FLOAT;
		case ShaderDataType::Float4:return GL_FLOAT;
		case ShaderDataType::Mat3:	return GL_FLOAT;
		case ShaderDataType::Mat4:	return GL_FLOAT;
		case ShaderDataType::Int:	return GL_INT;
		case ShaderDataType::Int2:	return GL_INT;
		case ShaderDataType::Int3:	return GL_INT;
		case ShaderDataType::Int4:	return GL_INT;
		case ShaderDataType::Bool:	return GL_BOOL;
		}
		LX_CORE_ASSERT(false, "Unkown ShaderDataType");
		return 0;
	}

	VertexArray::VertexArray()
	{
		//LX_PROFILE_FUNCTION();
		glCreateVertexArrays(1, &m_VertexArrayID);
		//LX_CORE_INFO("VA created, ID: {0}", m_VertexArrayID);
	}

	Ref<VertexArray> VertexArray::Create()
	{
		return Ref<VertexArray>(new VertexArray);
	}

	VertexArray::~VertexArray()
	{
		//LX_PROFILE_FUNCTION();
		//LX_CORE_INFO("VA destroyed");
		glDeleteVertexArrays(1, &m_VertexArrayID);
	}

	void VertexArray::Bind() const
	{
		//LX_PROFILE_FUNCTION();
		glBindVertexArray(m_VertexArrayID);
	}

	void VertexArray::Unbind() const
	{
		//LX_PROFILE_FUNCTION();
		glBindVertexArray(0);
	}

	void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		//LX_PROFILE_FUNCTION();

		LX_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout");
		glBindVertexArray(m_VertexArrayID);
		vertexBuffer->Bind();


		const auto& layout = vertexBuffer->GetLayout();
		for (const auto& element : layout)
		{
			switch (element.Type)
			{
			case ShaderDataType::Float:
			case ShaderDataType::Float2:
			case ShaderDataType::Float3:
			case ShaderDataType::Float4:
			{
				glEnableVertexAttribArray(m_VertexBufferIndex);
				glVertexAttribPointer(
					m_VertexBufferIndex,
					element.GetElementCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),
					element.Normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)(uint64_t)element.Offset);
				m_VertexBufferIndex++;
				break;
			}
			case ShaderDataType::Int:
			case ShaderDataType::Int2:
			case ShaderDataType::Int3:
			case ShaderDataType::Int4:
			case ShaderDataType::Bool:
			{
				glEnableVertexAttribArray(m_VertexBufferIndex);
				glVertexAttribIPointer(
					m_VertexBufferIndex,
					element.GetElementCount(),
					ShaderDataTypeToOpenGLBaseType(element.Type),
					layout.GetStride(),
					(const void*)(uint64_t)element.Offset);
				m_VertexBufferIndex++;
				break;
			}
			case ShaderDataType::Mat3:
			case ShaderDataType::Mat4:
			{
				uint8_t count = element.GetElementCount();
				for (uint8_t i = 0; i < count; i++)
				{
					glEnableVertexAttribArray(m_VertexBufferIndex);
					glVertexAttribPointer(
						m_VertexBufferIndex,
						count,
						ShaderDataTypeToOpenGLBaseType(element.Type),
						element.Normalized ? GL_TRUE : GL_FALSE,
						layout.GetStride(),
						(const void*)(uint64_t)(element.Offset + sizeof(float) * count * i));
					glVertexAttribDivisor(m_VertexBufferIndex, 1);
					m_VertexBufferIndex++;
				}
				break;
			}
			default:
				LX_CORE_ASSERT(false, "Unknown ShaderDataType!");
			}
		}

		m_VertexBuffers.push_back(vertexBuffer);
	}

	void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		//LX_PROFILE_FUNCTION();
		glBindVertexArray(m_VertexArrayID);
		indexBuffer->Bind();
		m_IndexBuffer = indexBuffer;
	}
}