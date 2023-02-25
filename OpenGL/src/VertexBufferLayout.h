#pragma once
#include <cassert>
#include <vector>
#include <GL/glew.h>
#include "Renderer.h"

struct VertexBufferLayoutElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	VertexBufferLayoutElement(unsigned int _type, unsigned int _count, unsigned char _normalized)
		: type(_type), count(_count), normalized(_normalized) 
	{

	}

	static unsigned int GetTypeSize(unsigned int type)
	{
		switch (type)
		{
		case GL_FLOAT: return sizeof(float);
		case GL_UNSIGNED_INT: return sizeof(unsigned int);
		case GL_UNSIGNED_BYTE: return sizeof(char);
		}
		ASSERT(false);
		return 0;
	}
};
class VertexBufferLayout
{
private:
	unsigned int m_Stride;
	std::vector<VertexBufferLayoutElement> m_Elements;

public:
	VertexBufferLayout()
		: m_Stride(0)
	{

	}
	template<typename T>
	void Push(unsigned int count)
	{
		assert(false);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.emplace_back(GL_FLOAT, count, GL_FALSE);
		m_Stride += VertexBufferLayoutElement::GetTypeSize(GL_FLOAT) * count;
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.emplace_back(GL_UNSIGNED_INT, count, GL_FALSE);
		m_Stride += VertexBufferLayoutElement::GetTypeSize(GL_UNSIGNED_INT) * count;
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.emplace_back(GL_UNSIGNED_BYTE, count, GL_TRUE );
		m_Stride += VertexBufferLayoutElement::GetTypeSize(GL_UNSIGNED_BYTE) * count;
	}

	inline unsigned int GetStride() const
	{
		return m_Stride;
	}

	inline std::vector<VertexBufferLayoutElement> GetElements() const
	{
		return m_Elements;
	}
};
