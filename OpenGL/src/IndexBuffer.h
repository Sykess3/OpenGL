#pragma once
class IndexBuffer
{
private:
	unsigned int m_RenderID;
	unsigned int m_Count;
public:
	IndexBuffer(const void* data, unsigned int count);
	~IndexBuffer();

	void Bind() const;
	void Unbind() const;

	unsigned int GetCount() const;
};

