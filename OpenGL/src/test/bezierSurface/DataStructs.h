#pragma once
#include <array>

namespace test
{
	struct Vertex
	{
		float x, y, z;
	};
	struct Portion
	{
		std::array<Vertex, 5> ControlPoints1;
		std::array<Vertex, 5> ControlPoints2;
		std::array<Vertex, 5> ControlPoints3;
	};
}