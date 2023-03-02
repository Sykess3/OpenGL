#include "Texture.h"
#include "Renderer.h"
#include "stb_image/stb_image.h"

Texture::Texture(const std::string& filePath)
	: m_RendererID(0), m_LocalBuffer(nullptr), m_FilePath(filePath), m_Width(0), m_Height(0), m_BytesPerPixel(0)
{
	stbi_set_flip_vertically_on_load(true);
	m_LocalBuffer = stbi_load(filePath.c_str(), &m_Width, &m_Height, &m_BytesPerPixel, 4);

	GlCall(glGenTextures(1, &m_RendererID));
	GlCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
	GlCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

	if (m_LocalBuffer)
	{
		GlCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
		GlCall(glGenerateMipmap(GL_TEXTURE_2D));
		GlCall(glBindTexture(GL_TEXTURE_2D, 0));
		stbi_image_free(m_LocalBuffer);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
}

Texture::~Texture()
{
	GlCall(glDeleteTextures(1, &m_RendererID));
}

void Texture::Bind(unsigned int slot) const
{
	GlCall(glActiveTexture(GL_TEXTURE0 + slot));
	GlCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));
}

void Texture::Unbind() const
{
	GlCall(glBindTexture(GL_TEXTURE_2D, 0));
}
