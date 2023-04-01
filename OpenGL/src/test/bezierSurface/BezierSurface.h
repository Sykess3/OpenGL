#pragma once
#include <vector>
#include <tuple>
#include <utility>

#include "DataStructs.h"

namespace test
{
	class BezierSurface
	{
	public:
		std::vector<std::shared_ptr<RenderData>> GetRenderData(const std::vector<Portion>& portions, float uvStep, std::vector<std::tuple<glm::vec3, glm::vec3>>& normalsOutput);

	private:
		std::vector<std::vector<Vertex>> m_SurfaceVertexes;

	private:
		void GenerateSurfaceVertexes(const std::vector<Portion>& portions, float uvStep);
		std::vector<std::shared_ptr<RenderData>> GenerateArrayBufferInternal(std::vector<std::tuple<glm::vec3, glm::vec3>>& normalsOutput);
		Vertex Bezier5(const Vertex& p0, const Vertex& p1, const Vertex& p2, const Vertex& p3, const Vertex& p4,
		               float t);
		Vertex Bezier3(const Vertex& p0, const Vertex& p1, const Vertex& p2, float t);

		std::tuple<glm::vec3, glm::vec3> GetNormal(const Vertex& v1, const Vertex& v2, const Vertex& v3);
	};
}
