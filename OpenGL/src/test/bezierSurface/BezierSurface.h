#pragma once
#include <vector>
#include <tuple>
#include <utility>
#include <unordered_map>

#include "DataStructs.h"

namespace test
{
	class BezierSurface
	{
	public:
		std::unique_ptr<RenderData> GetRenderData(const std::vector<Portion>& portions, float uvStep);

	private:
		std::vector<std::vector<Vertex>> m_SurfaceVertexes;

	private:
		void GenerateSurfaceVertexes(const std::vector<Portion>& portions, float uvStep);
		std::unique_ptr<RenderData> GenerateArrayBufferInternal();
		Vertex Bezier5(const Vertex& p0, const Vertex& p1, const Vertex& p2, const Vertex& p3, const Vertex& p4,
		               float t);
		Vertex Bezier3(const Vertex& p0, const Vertex& p1, const Vertex& p2, float t);
	};
}
