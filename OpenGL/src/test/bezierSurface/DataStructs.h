#pragma once
#include <array>
#include <memory>
#include <cmath>
#include <vector>

#include "Constant.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

namespace test
{
	struct Vertex
	{
		float x, y, z;

		Vertex() noexcept
			: x(0), y(0), z(0)
		{
		}

		Vertex(float x, float y, float z) noexcept
			: x(x), y(y), z(z)
		{
		}

		Vertex(const Vertex& other) noexcept
			: x(other.x), y(other.y), z(other.z)
		{
		}

		Vertex(Vertex&& other) noexcept
			: x(other.x), y(other.y), z(other.z)
		{
			other.x = 0;
			other.y = 0;
			other.z = 0;
		}

		Vertex& operator=(const Vertex& other) noexcept
		{
			if (*this != other)
			{
				x = other.x;
				y = other.y;
				z = other.z;
			}
			return *this;
		}

		Vertex& operator=(Vertex&& other)
		{
			x = other.x;
			y = other.y;
			z = other.z;

			return *this;
		}


		bool operator==(const Vertex& other) const
		{
			return abs(other.x - x) < Constant::Epsilon && abs(other.y - y) < Constant::Epsilon && (other.z - z) <
				Constant::Epsilon;
		}

		bool operator!=(const Vertex& other) const
		{
			return !(other == *this);
		}
	};

	struct Portion
	{
		std::array<Vertex, 5> ControlPoints1;
		std::array<Vertex, 5> ControlPoints2;
		std::array<Vertex, 5> ControlPoints3;

		static const unsigned int Count = 15;
	};


	struct RenderData
	{
		RenderData(std::unique_ptr<IndexBuffer> ibo, std::unique_ptr<VertexArray> vao, std::unique_ptr<VertexBuffer> vbo)
			:IBO(std::move(ibo)), VAO(std::move(vao)), VBO(std::move(vbo))
		{
		}
		std::unique_ptr<IndexBuffer> IBO;
		std::unique_ptr<VertexArray> VAO;
		std::unique_ptr<VertexBuffer> VBO;
	};
}
