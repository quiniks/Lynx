#pragma once
#include "Lynx/Render/Buffer.h"

namespace Lynx {
	class VertexArray {
	public:
		static Ref<VertexArray> Create();

		~VertexArray();

		void Bind() const;
		void Unbind() const;

		void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
		void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& IndexBuffer);

		const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; };
		const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; };
	private:
		VertexArray();
		VertexArray(const VertexArray&) = delete;
		void operator=(const VertexArray&) = delete;

		uint32_t m_VertexArrayID = 0;
		uint32_t m_VertexBufferIndex = 0;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};
}

