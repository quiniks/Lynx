#pragma once

namespace Lynx {
	class Texture {
	public:
		Texture() = default;
		~Texture();

		void Load(const std::string& path);

		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }
		uint32_t GetRendererID() const { return m_TextureID; }

		void Bind(uint32_t slot = 0) const;

		bool operator==(const Texture& other) const {
			return m_TextureID == other.m_TextureID;
		}
	private:
		std::string m_Path;
		uint32_t m_Width = 0, m_Height = 0;
		uint32_t m_TextureID = 0;
	};
}