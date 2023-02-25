#include "VertexArray.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"

VertexArray::VertexArray()
{
	GlCall(glGenVertexArrays(1, &m_RenderID));
}

VertexArray::~VertexArray()
{
	GlCall(glDeleteVertexArrays(1, &m_RenderID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
	Bind();
	vb.Bind();
	const std::vector<VertexBufferLayoutElement>& layoutElements = layout.GetElements();
	unsigned int offset = 0;
	for (int i = 0; i < layoutElements.size(); ++i)
	{
		const VertexBufferLayoutElement& element = layoutElements[i];

		GlCall(glEnableVertexAttribArray(i));
		GlCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
		offset += element.count * VertexBufferLayoutElement::GetTypeSize(element.type);
	}
}

void VertexArray::Bind() const
{
	GlCall(glBindVertexArray(m_RenderID));
}

void VertexArray::Unbind() const
{
	GlCall(glBindVertexArray(0));
}


