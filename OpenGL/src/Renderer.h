#pragma once

#include <iostream>
#include <GL/glew.h>

#include "IndexBuffer.h"
#include "Shader.h"
#include "VertexArray.h"

#define ASSERT(x) if (!(x)) __debugbreak()

#define GlCall(x) GLClearError();\
x;\
ASSERT(GLLogCall(#x, __FILE__, __LINE__))


void GLClearError();

bool GLLogCall(const char* function, const char* file, const int line);

class Renderer
{
public:
	void Draw(const Shader& shader, const IndexBuffer& ib, const VertexArray& va);
	void Clear();
};
