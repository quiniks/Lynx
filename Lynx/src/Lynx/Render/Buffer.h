#pragma once
namespace Lynx {
	enum class ShaderDataType {
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, PackedInt, UInt, Bool
	};

	static uint32_t ShaderDataTypeSize(ShaderDataType type) {
		switch (type) {
		case ShaderDataType::Float:			return 4;
		case ShaderDataType::Float2:		return 4 * 2;
		case ShaderDataType::Float3:		return 4 * 3;
		case ShaderDataType::Float4:		return 4 * 4;
		case ShaderDataType::Mat3:			return 4 * 3 * 3;
		case ShaderDataType::Mat4:			return 4 * 4 * 4;
		case ShaderDataType::Int:			return 4;
		case ShaderDataType::Int2:			return 4 * 2;
		case ShaderDataType::Int3:			return 4 * 3;
		case ShaderDataType::Int4:			return 4 * 4;
		case ShaderDataType::PackedInt:		return 4;
		case ShaderDataType::UInt:			return 4;
		case ShaderDataType::Bool:			return 1;
		}
		LX_CORE_ASSERT(false, "Unkown ShaderDataType");
		return 0;
	}

	struct BufferElement {
		std::string Name;
		ShaderDataType Type = ShaderDataType::None;
		uint32_t Size = 0;
		uint32_t Offset = 0;
		bool Normalized = false;

		BufferElement() = default;

		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized) {}

		uint32_t GetElementCount() const;
	};

	class BufferLayout {
	public:
		BufferLayout() {
			//LX_CORE_INFO("Buffer layout created");
		}
		BufferLayout(const std::initializer_list<BufferElement>& elements)
			: m_Elements(elements)
		{
			CalculateOffsetAndStride();
		}

		uint32_t GetStride() const { return m_Stride; }
		const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }

		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	private:
		void CalculateOffsetAndStride();
	private:
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride = 0;
	};

	class VertexBuffer {
	public:
		static Ref<VertexBuffer> Create(size_t size);
		static Ref<VertexBuffer> Create(const void* vertices, size_t size);
		~VertexBuffer();
		void Bind() const;
		void Unbind() const;
		void SetSubData(const void* data, size_t size);
		void SetData(const void* vertices, size_t size);

		const BufferLayout& GetLayout() const { return m_Layout; }
		void SetLayout(const BufferLayout& layout) { m_Layout = layout; }
	private:
		VertexBuffer(size_t size);
		VertexBuffer(const void* vertices, size_t size);
		uint32_t m_VertexBufferID;
		BufferLayout m_Layout;
	};

	class IndexBuffer {
	public:
		static Ref<IndexBuffer> Create(int* indices, int count);
		~IndexBuffer();
		void Bind() const;
		void Unbind() const;
		int GetCount() const { return m_Count; }
	private:
		IndexBuffer(int* indices, int count);
		uint32_t m_IndexBufferID;
		int m_Count;
	};
}

