#include "Lynxpch.h"
#include "Lynx/Render/Texture.h"
#include "stb_image.h"
#include <glad/glad.h>

namespace Lynx {
	Texture::~Texture()
	{
		//URSA_PROFILE_FUNCTION();
		glDeleteTextures(1, &m_TextureID);
	}

	void Texture::Load(const std::string& path)
	{
		m_Path = path;
		//LX_PROFILE_FUNCTION();
		int width, height, channels;
		stbi_set_flip_vertically_on_load(0);
		stbi_uc* data = nullptr;
		data = stbi_load(path.c_str(), &width, &height, &channels, 0);
		LX_CORE_ASSERT(data, "Failed to load image");

		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4) {
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3) {
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}

		LX_CORE_ASSERT(internalFormat & dataFormat, "Format not supported");

		glCreateTextures(GL_TEXTURE_2D, 1, &m_TextureID);

		glTextureParameteri(m_TextureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(m_TextureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTextureParameteri(m_TextureID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTextureStorage2D(m_TextureID, 1, internalFormat, width, height);
		glTextureSubImage2D(m_TextureID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		stbi_image_free(data);
	}

	void Texture::Bind(uint32_t slot) const
	{
		//URSA_PROFILE_FUNCTION();
		glBindTextureUnit(slot, m_TextureID);
	}
}