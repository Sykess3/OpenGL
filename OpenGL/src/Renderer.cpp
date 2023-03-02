#include "Renderer.h"

void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, const int line)
{
	int hasNotError = true;
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << ")" << function << " " << file << ":" << line << std::endl;
		hasNotError = false;
	}

	return hasNotError;
}

void Renderer::Draw(const Shader& shader, const IndexBuffer& ib, const VertexArray& va)
{
	va.Bind();
	ib.Bind();
	shader.Bind();

	GlCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Draw(const Shader& shader, const VertexArray& va, unsigned int bufferElementsCount)
{
	va.Bind();
	shader.Bind();

	GlCall(glDrawArrays(GL_TRIANGLES, 0, bufferElementsCount));
}

void Renderer::Clear()
{
	GlCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}
