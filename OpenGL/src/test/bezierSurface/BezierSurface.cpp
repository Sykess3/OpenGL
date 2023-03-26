#include <cmath>

#include "BezierSurface.h"
#include "VertexBufferLayout.h"
#include "RenderDataBuilder.h"

namespace test
{
	std::unique_ptr<RenderData> BezierSurface::GetRenderData(const std::vector<Portion>& portions, float uvStep)
	{
		GenerateSurfaceVertexes(portions, uvStep);
		return std::move(GenerateArrayBufferInternal());
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

	std::unique_ptr<RenderData> BezierSurface::GenerateArrayBufferInternal()
	{
		//int pointsCountInRowOrColumn = sqrt(m_SurfaceVertexes.size() / portionsCount);
		RenderDataBuilder builder(m_SurfaceVertexes.size());

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

					builder.Add(vertices[i + 1], j);
					builder.Add(vertices[i + pointsCountInRowOrColumn], j);
					builder.Add(vertices[i + pointsCountInRowOrColumn + 1], j);
				}
			}
		}

		VertexBufferLayout layout;
		layout.Push<float>(3);
		std::unique_ptr<RenderData> renderData = builder.Get(layout);


		return std::move(renderData);
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
}
