#pragma once
#include "VertexBuffer.h"

class VertexBufferLayout;

class VertexArray
{
private:
	unsigned int m_RenderID;
public:
	VertexArray();
	~VertexArray();

	VertexArray(const VertexArray& other);
	VertexArray(VertexArray&& other);
	VertexArray& operator=(const VertexArray& other);
	VertexArray& operator=(VertexArray&& other);

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;
};

