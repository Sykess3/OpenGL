#include "BezierSurface.h"
#include <cmath>

#include "VertexBufferLayout.h"

namespace test
{

	std::tuple<std::unique_ptr<VertexBuffer>, std::unique_ptr<VertexArray>, int> BezierSurface::GetRenderData(const std::vector<Portion>& portions, float uvStep)
	{
		GenerateSurfaceVertexes(portions, uvStep);
		return GenerateArrayBuffer();
	}

	void BezierSurface::GenerateSurfaceVertexes(const std::vector<Portion>& portions, float uvStep)
	{
		m_SurfaceVertexes.clear();
		for (int i = 0; i < portions.size(); i++)
		{
			Portion portion = portions[i];
			bool hasVUsedOneLikeT = false;
			for (float v = 0; v <= 1; v += uvStep)
			{
				hasVUsedOneLikeT = v == 1;
				const auto cp1Vertex = Bezier5(portion.ControlPoints1[0], portion.ControlPoints1[1],
					portion.ControlPoints1[2], portion.ControlPoints1[3], portion.ControlPoints1[4], v);
				const auto cp2Vertex = Bezier5(portion.ControlPoints2[0], portion.ControlPoints2[1],
					portion.ControlPoints2[2], portion.ControlPoints2[3], portion.ControlPoints2[4], v);
				const auto cp3Vertex = Bezier5(portion.ControlPoints3[0], portion.ControlPoints3[1],
					portion.ControlPoints3[2], portion.ControlPoints3[3], portion.ControlPoints3[4], v);

				bool hasUUsedOneLikeT = false;
				for (float u = 0; u <= 1; u += uvStep)
				{
					hasUUsedOneLikeT = u == 1;
					auto surfacePoint = Bezier3(cp1Vertex, cp2Vertex, cp3Vertex, u);
					m_SurfaceVertexes.push_back(surfacePoint);
				}

				if (!hasUUsedOneLikeT)
				{
					auto surfacePoint = Bezier3(cp1Vertex, cp2Vertex, cp3Vertex, 1);
					m_SurfaceVertexes.push_back(surfacePoint);
				}
			}
			if (!hasVUsedOneLikeT)
			{
				const auto cp1Vertex = Bezier5(portion.ControlPoints1[0], portion.ControlPoints1[1],
					portion.ControlPoints1[2], portion.ControlPoints1[3], portion.ControlPoints1[4], 1);
				const auto cp2Vertex = Bezier5(portion.ControlPoints2[0], portion.ControlPoints2[1],
					portion.ControlPoints2[2], portion.ControlPoints2[3], portion.ControlPoints2[4], 1);
				const auto cp3Vertex = Bezier5(portion.ControlPoints3[0], portion.ControlPoints3[1],
					portion.ControlPoints3[2], portion.ControlPoints3[3], portion.ControlPoints3[4], 1);

				for (float u = 0; u <= 1; u += uvStep)
				{
					auto surfacePoint = Bezier3(cp1Vertex, cp2Vertex, cp3Vertex, u);
					m_SurfaceVertexes.push_back(surfacePoint);
				}

				auto surfacePoint = Bezier3(cp1Vertex, cp2Vertex, cp3Vertex, 1);
				m_SurfaceVertexes.push_back(surfacePoint);
			}
		}
	}

	std::tuple<std::unique_ptr<VertexBuffer>, std::unique_ptr<VertexArray>, int> BezierSurface::GenerateArrayBuffer()
	{
		int pointsCountInRowOrColumn = sqrt(m_SurfaceVertexes.size());
		std::vector<Vertex> sortedVector;
		sortedVector.reserve(m_SurfaceVertexes.size() * 3 - pointsCountInRowOrColumn);
		for (unsigned int i = 0; i < m_SurfaceVertexes.size() - pointsCountInRowOrColumn; i++)
		{
			unsigned i1 = (i + 1) % pointsCountInRowOrColumn;
			if (i1 != 0) {
				sortedVector.push_back(m_SurfaceVertexes[i]);
				sortedVector.push_back(m_SurfaceVertexes[i + pointsCountInRowOrColumn]);
				sortedVector.push_back(m_SurfaceVertexes[i + 1]);

				sortedVector.push_back(m_SurfaceVertexes[i + 1]);
				sortedVector.push_back(m_SurfaceVertexes[i + pointsCountInRowOrColumn]);
				sortedVector.push_back(m_SurfaceVertexes[i + pointsCountInRowOrColumn + 1]);
			}
		}

		std::unique_ptr<VertexBuffer> vbo = std::make_unique<VertexBuffer>(sortedVector.data(), sortedVector.size() * sizeof(Vertex));
		std::unique_ptr<VertexArray> vao = std::make_unique<VertexArray>();
		int pointsCount = sortedVector.size();
		VertexBufferLayout layout;
		layout.Push<float>(3);

		vao->AddBuffer(*vbo, layout);

		return {std::move(vbo), std::move(vao), pointsCount};
	}

	Vertex BezierSurface::Bezier5(const Vertex& p0, const Vertex& p1, const Vertex& p2, const Vertex& p3, const Vertex& p4, float t)
	{
		float x = std::pow(1 - t, 4) * p0.x + 4 * std::pow(1 - t, 3) * t * p1.x + 6 * std::pow(1 - t, 2) * std::pow(t, 2) * p2.x + 4 * (1 - t) * std::pow(t, 3) * p3.x + std::pow(t, 4) * p4.x;
		float y = std::pow(1 - t, 4) * p0.y + 4 * std::pow(1 - t, 3) * t * p1.y + 6 * std::pow(1 - t, 2) * std::pow(t, 2) * p2.y + 4 * (1 - t) * std::pow(t, 3) * p3.y + std::pow(t, 4) * p4.y;
		float z = std::pow(1 - t, 4) * p0.z + 4 * std::pow(1 - t, 3) * t * p1.z + 6 * std::pow(1 - t, 2) * std::pow(t, 2) * p2.z + 4 * (1 - t) * std::pow(t, 3) * p3.z + std::pow(t, 4) * p4.z;
		return Vertex{ x, y, z };
	}

	Vertex BezierSurface::Bezier3(const Vertex& p0, const Vertex& p1, const Vertex& p2, float t)
	{
		float x = std::pow(1 - t, 2) * p0.x + 2 * (1 - t) * t * p1.x + std::pow(t, 2) * p2.x;
		float y = std::pow(1 - t, 2) * p0.y + 2 * (1 - t) * t * p1.y + std::pow(t, 2) * p2.y;
		float z = std::pow(1 - t, 2) * p0.z + 2 * (1 - t) * t * p1.z + std::pow(t, 2) * p2.z;
		return Vertex{ x, y, z };
	}
}
