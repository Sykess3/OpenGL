#include <cmath>

#include "BezierSurface.h"
#include "VertexBufferLayout.h"
#include "RenderDataBuilder.h"

namespace test
{
	std::vector<std::shared_ptr<RenderData>> BezierSurface::GetRenderData(
		const std::vector<Portion>& portions, float uvStep, std::vector<std::tuple<glm::vec3, glm::vec3>>& normalsOutput)
	{
		GenerateSurfaceVertexes(portions, uvStep);
		return GenerateArrayBufferInternal(normalsOutput);
	}

	void BezierSurface::GenerateSurfaceVertexes(const std::vector<Portion>& portions, float uvStep)
	{
		m_SurfaceVertexes.clear();
		for (int i = 0; i < portions.size(); i++)
		{
			Portion portion = portions[i];
			bool hasVUsedOneLikeT = false;
			m_SurfaceVertexes.emplace_back();
			auto& vertexes = m_SurfaceVertexes[i];
			for (float v = 0; v <= 1; v += uvStep)
			{
				hasVUsedOneLikeT = v == 1;
				const auto cp1Vertex = Bezier5(portion.ControlPoints1[0], portion.ControlPoints1[1],
				                               portion.ControlPoints1[2], portion.ControlPoints1[3],
				                               portion.ControlPoints1[4], v);
				const auto cp2Vertex = Bezier5(portion.ControlPoints2[0], portion.ControlPoints2[1],
				                               portion.ControlPoints2[2], portion.ControlPoints2[3],
				                               portion.ControlPoints2[4], v);
				const auto cp3Vertex = Bezier5(portion.ControlPoints3[0], portion.ControlPoints3[1],
				                               portion.ControlPoints3[2], portion.ControlPoints3[3],
				                               portion.ControlPoints3[4], v);

				bool hasUUsedOneLikeT = false;
				for (float u = 0; u <= 1; u += uvStep)
				{
					hasUUsedOneLikeT = u == 1;
					auto surfacePoint = Bezier3(cp1Vertex, cp2Vertex, cp3Vertex, u);
					vertexes.push_back(surfacePoint);
				}

				if (!hasUUsedOneLikeT)
				{
					auto surfacePoint = Bezier3(cp1Vertex, cp2Vertex, cp3Vertex, 1);
					vertexes.push_back(surfacePoint);
				}
			}
			if (!hasVUsedOneLikeT)
			{
				const auto cp1Vertex = Bezier5(portion.ControlPoints1[0], portion.ControlPoints1[1],
				                               portion.ControlPoints1[2], portion.ControlPoints1[3],
				                               portion.ControlPoints1[4], 1);
				const auto cp2Vertex = Bezier5(portion.ControlPoints2[0], portion.ControlPoints2[1],
				                               portion.ControlPoints2[2], portion.ControlPoints2[3],
				                               portion.ControlPoints2[4], 1);
				const auto cp3Vertex = Bezier5(portion.ControlPoints3[0], portion.ControlPoints3[1],
				                               portion.ControlPoints3[2], portion.ControlPoints3[3],
				                               portion.ControlPoints3[4], 1);

				for (float u = 0; u <= 1; u += uvStep)
				{
					auto surfacePoint = Bezier3(cp1Vertex, cp2Vertex, cp3Vertex, u);
					vertexes.push_back(surfacePoint);
				}

				auto surfacePoint = Bezier3(cp1Vertex, cp2Vertex, cp3Vertex, 1);
				vertexes.push_back(surfacePoint);
			}
		}
	}

	std::vector<std::shared_ptr<RenderData>> BezierSurface::GenerateArrayBufferInternal(std::vector<std::tuple<glm::vec3, glm::vec3>>& normalsOutput)
	{
		//int pointsCountInRowOrColumn = sqrt(m_SurfaceVertexes.size() / portionsCount);
		RenderDataBuilder builder(m_SurfaceVertexes.size());
		normalsOutput.clear();

		for (int j = 0; j < m_SurfaceVertexes.size(); j++)
		{
			auto& vertices = m_SurfaceVertexes[j];
			int pointsCountInRowOrColumn = sqrt(vertices.size());
			for (unsigned int i = 0; i < vertices.size() - pointsCountInRowOrColumn; i++)
			{
				unsigned isNotLastElementInRow = ((i + 1) % pointsCountInRowOrColumn) != 0;
				if (isNotLastElementInRow)
				{
					builder.Add(vertices[i], j);
					builder.Add(vertices[i + pointsCountInRowOrColumn], j);
					builder.Add(vertices[i + 1], j);
					std::tuple<glm::vec3, glm::vec3> normal1 = GetNormal(vertices[i + pointsCountInRowOrColumn], vertices[i], vertices[i + 1]);
					normalsOutput.push_back(normal1);

					builder.Add(vertices[i + 1], j);
					builder.Add(vertices[i + pointsCountInRowOrColumn], j);
					builder.Add(vertices[i + pointsCountInRowOrColumn + 1], j); // center
					std::tuple<glm::vec3, glm::vec3> normal2 = GetNormal(vertices[i + 1], vertices[i + pointsCountInRowOrColumn + 1], vertices[i + pointsCountInRowOrColumn]);
					normalsOutput.push_back(normal2);
				}
			}
		}

		VertexBufferLayout layout;
		layout.Push<float>(3);
		std::vector<std::shared_ptr<RenderData>> renderData = builder.Get(layout);


		return renderData;
	}


	Vertex BezierSurface::Bezier5(const Vertex& p0, const Vertex& p1, const Vertex& p2, const Vertex& p3,
	                              const Vertex& p4, float t)
	{
		float x = std::pow(1 - t, 4) * p0.x + 4 * std::pow(1 - t, 3) * t * p1.x + 6 * std::pow(1 - t, 2) *
			std::pow(t, 2) * p2.x + 4 * (1 - t) * std::pow(t, 3) * p3.x + std::pow(t, 4) * p4.x;
		float y = std::pow(1 - t, 4) * p0.y + 4 * std::pow(1 - t, 3) * t * p1.y + 6 * std::pow(1 - t, 2) *
			std::pow(t, 2) * p2.y + 4 * (1 - t) * std::pow(t, 3) * p3.y + std::pow(t, 4) * p4.y;
		float z = std::pow(1 - t, 4) * p0.z + 4 * std::pow(1 - t, 3) * t * p1.z + 6 * std::pow(1 - t, 2) *
			std::pow(t, 2) * p2.z + 4 * (1 - t) * std::pow(t, 3) * p3.z + std::pow(t, 4) * p4.z;
		return Vertex{x, y, z};
	}

	Vertex BezierSurface::Bezier3(const Vertex& p0, const Vertex& p1, const Vertex& p2, float t)
	{
		float x = std::pow(1 - t, 2) * p0.x + 2 * (1 - t) * t * p1.x + std::pow(t, 2) * p2.x;
		float y = std::pow(1 - t, 2) * p0.y + 2 * (1 - t) * t * p1.y + std::pow(t, 2) * p2.y;
		float z = std::pow(1 - t, 2) * p0.z + 2 * (1 - t) * t * p1.z + std::pow(t, 2) * p2.z;
		return Vertex{x, y, z};
	}

	std::tuple<glm::vec3, glm::vec3> BezierSurface::GetNormal(const Vertex& v1, const Vertex& v2, const Vertex& v3)
	{
		glm::vec3 dir1 = v1.GetPosition() - v2.GetPosition();
		glm::vec3 dir2 = v3.GetPosition() - v2.GetPosition();

		glm::vec3 cross = glm::cross(dir1, dir2);
		glm::vec3 normalized = glm::normalize(cross);
		int length = normalized.length();
		glm::vec3 start = glm::vec3((v1.x + v2.x + v3.x) / 3, (v1.y + v2.y + v3.y) / 3, (v1.z + v2.z + v3.z) / 3);
		glm::vec3 end = (normalized * 0.05f) + start;

		return std::make_tuple(start, end);
	}
}
