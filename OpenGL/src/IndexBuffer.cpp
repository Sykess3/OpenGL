#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const void* data, unsigned int count)
	:m_RenderID(0), m_Count(count)
{
    ASSERT(sizeof(unsigned int) == sizeof(GLuint));

    GlCall(glGenBuffers(1, &m_RenderID));
    GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID));
    GlCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
    GlCall(glDeleteBuffers(1, &m_RenderID));
}

void IndexBuffer::Bind() const
{
    GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RenderID));
}

void IndexBuffer::Unbind() const
{
    GlCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

unsigned IndexBuffer::GetCount() const
{
    return m_Count;
}


