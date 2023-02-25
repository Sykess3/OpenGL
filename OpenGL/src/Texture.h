#pragma once
#include <string>

class Texture
{
private:
	unsigned int m_RendererID;

	unsigned char* m_LocalBuffer;
	std::string m_FilePath;
	int m_Width;
	int m_Height;
	int m_BytesPerPixel;


public:
	Texture(const std::string& filePath);
	~Texture();

	void Bind(unsigned int slot) const;
	void Unbind() const;

	inline int GetWidth() const { return m_Width; };
	inline int GetHeight() const { return m_Height; }
};

