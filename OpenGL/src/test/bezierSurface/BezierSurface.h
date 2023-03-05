#pragma once
#include <vector>
#include <memory>
#include <tuple>
#include <utility>
 
#include "DataStructs.h"
#include "VertexArray.h"

namespace test
{
	class BezierSurface
	{
	public:
		std::tuple<std::unique_ptr<VertexBuffer>, std::unique_ptr<VertexArray>, int> GetRenderData(const std::vector<Portion>& portions, float uvStep);
	private:
		std::vector<Vertex> m_SurfaceVertexes;

	private:

		void GenerateSurfaceVertexes(const std::vector<Portion>& portions, float uvStep);
		std::tuple<std::unique_ptr<VertexBuffer>, std::unique_ptr<VertexArray>, int> GenerateArrayBuffer();
		Vertex Bezier5(const Vertex& p0, const Vertex& p1, const Vertex& p2, const Vertex& p3, const Vertex& p4, float t);
		Vertex Bezier3(const Vertex& p0, const Vertex& p1, const Vertex& p2, float t);

	};
}
