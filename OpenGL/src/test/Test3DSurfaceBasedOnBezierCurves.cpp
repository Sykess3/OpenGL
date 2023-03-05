#include "Test3DSurfaceBasedOnBezierCurves.h"
#include <cmath>

#include "Constant.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "imgui/imgui.h"

namespace test
{

	Test3DSurfaceBasedOnBezierCurves::Test3DSurfaceBasedOnBezierCurves()
		: m_UVStep(0.1f),
		m_Proj(glm::perspective(glm::radians(45.0f), (float)Constant::Width / (float)Constant::Height, 0.1f, 100.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -3))),
		m_RotationAxis(0.0f, 0.1f, 0.0f),
		m_RotationValue(0),
		m_Portions(0)
	{
		GlCall(glEnable(GL_BLEND));
		GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
		GlCall(glPolygonMode(GL_FRONT_AND_BACK, GL_LINE));

		GlCall(glEnable(GL_DEPTH_TEST));
		InitializePortion();
		GenerateSurfacePoints();
		GenerateArrayBuffer();
		m_Shader = std::make_unique<Shader>("res/shaders/BasicColor.shader");
		m_Shader->Bind();
		m_Shader->SetUniform4f("u_Color", 1.0f, 0.0f, 0.0f, 1.0f);
	}

	void Test3DSurfaceBasedOnBezierCurves::OnUpdate(float deltaTime)
	{
		if(m_UVStep != m_LastGeneratedStep)
		{
			//int intStep = m_UVStep * 1000;
			//int substructValue = intStep % 10;
			//m_UVStep -= (substructValue / 1000.f);
			VertexBuffer* vbo = m_VBO.release();
			delete vbo;

			VertexArray* vao = m_VAO.release();
			delete vao;

			GenerateSurfacePoints();
			GenerateArrayBuffer();
		}
	}

	void Test3DSurfaceBasedOnBezierCurves::OnRender()
	{

		GlCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));

		Renderer renderer;
		{
			glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(m_RotationValue), m_RotationAxis);
			glm::mat4 mvp = m_Proj * m_View * model;

			m_Shader->Bind();
			m_Shader->SetUniformMat4f("u_MVP", mvp);

			renderer.Draw(*m_Shader, *m_VAO, m_PointsCount);
		}
	}
	void Test3DSurfaceBasedOnBezierCurves::OnImGuiRender()
	{
		ImGui::SliderFloat3("Rotation Axis", &m_RotationAxis.x, -1, 1);
		ImGui::SliderFloat("Rotation Value", &m_RotationValue, -180, 180);
		ImGui::SliderFloat("Step", &m_UVStep, 0.01, 0.1);
		ImGui::Text("Surface Points Count %d", m_SurfacePoints.size());
	}

	void Test3DSurfaceBasedOnBezierCurves::GenerateSurfacePoints()
	{
		m_SurfacePoints.clear();
		//m_SurfacePoints.insert(m_SurfacePoints.end(),m_Portions[0].ControlPoints1.begin(), m_Portions[0].ControlPoints1.end());
		//m_SurfacePoints.insert(m_SurfacePoints.end(), m_Portions[0].ControlPoints2.begin(), m_Portions[0].ControlPoints2.end());
		//m_SurfacePoints.insert(m_SurfacePoints.end(), m_Portions[0].ControlPoints3.begin(), m_Portions[0].ControlPoints3.end());
		for (int i = 0; i < m_Portions.size(); i++)
		{
			Portion portion = m_Portions[i];
			for (float v = 0; v <= 1; v += m_UVStep)
			{
				const auto cp1Vertex = Bezier5(portion.ControlPoints1[0], portion.ControlPoints1[1],
				                               portion.ControlPoints1[2], portion.ControlPoints1[3], portion.ControlPoints1[4], v);
				const auto cp2Vertex = Bezier5(portion.ControlPoints2[0], portion.ControlPoints2[1],
				                               portion.ControlPoints2[2], portion.ControlPoints2[3], portion.ControlPoints2[4], v);
				const auto cp3Vertex = Bezier5(portion.ControlPoints3[0], portion.ControlPoints3[1],
				                               portion.ControlPoints3[2], portion.ControlPoints3[3], portion.ControlPoints3[4], v);

				for (float u = 0; u <= 1; u += m_UVStep)
				{
					auto surfacePoint = Bezier3(	cp1Vertex, cp2Vertex, cp3Vertex, u);
					m_SurfacePoints.push_back(surfacePoint);
				}

				auto surfacePoint = Bezier3(cp1Vertex, cp2Vertex, cp3Vertex, 1);
				m_SurfacePoints.push_back(surfacePoint);
			}
			const auto cp1Vertex = Bezier5(portion.ControlPoints1[0], portion.ControlPoints1[1],
				portion.ControlPoints1[2], portion.ControlPoints1[3], portion.ControlPoints1[4], 1);
			const auto cp2Vertex = Bezier5(portion.ControlPoints2[0], portion.ControlPoints2[1],
				portion.ControlPoints2[2], portion.ControlPoints2[3], portion.ControlPoints2[4], 1);
			const auto cp3Vertex = Bezier5(portion.ControlPoints3[0], portion.ControlPoints3[1],
				portion.ControlPoints3[2], portion.ControlPoints3[3], portion.ControlPoints3[4], 1);

			for (float u = 0; u <= 1; u += m_UVStep)
			{
				auto surfacePoint = Bezier3(cp1Vertex, cp2Vertex, cp3Vertex, u);
				m_SurfacePoints.push_back(surfacePoint);
			}

			auto surfacePoint = Bezier3(cp1Vertex, cp2Vertex, cp3Vertex, 1);
			m_SurfacePoints.push_back(surfacePoint);
		}
		m_LastGeneratedStep = m_UVStep;
	}

	Vertex Test3DSurfaceBasedOnBezierCurves::Bezier5(Vertex p0, Vertex p1, Vertex p2, Vertex p3, Vertex p4, float t)
	{
		float x = std::pow(1 - t, 4) * p0.x + 4 * std::pow(1 - t, 3) * t * p1.x + 6 * std::pow(1 - t, 2) * std::pow(t, 2) * p2.x + 4 * (1 - t) * std::pow(t, 3) * p3.x + std::pow(t, 4) * p4.x;
		float y = std::pow(1 - t, 4) * p0.y + 4 * std::pow(1 - t, 3) * t * p1.y + 6 * std::pow(1 - t, 2) * std::pow(t, 2) * p2.y + 4 * (1 - t) * std::pow(t, 3) * p3.y + std::pow(t, 4) * p4.y;
		float z = std::pow(1 - t, 4) * p0.z + 4 * std::pow(1 - t, 3) * t * p1.z + 6 * std::pow(1 - t, 2) * std::pow(t, 2) * p2.z + 4 * (1 - t) * std::pow(t, 3) * p3.z + std::pow(t, 4) * p4.z;
		return Vertex{ x, y, z };
	}

	Vertex Test3DSurfaceBasedOnBezierCurves::Bezier3(Vertex p0, Vertex p1, Vertex p2, float t)
	{
		float x = std::pow(1 - t, 2) * p0.x + 2 * (1 - t) * t * p1.x + std::pow(t, 2) * p2.x;
		float y = std::pow(1 - t, 2) * p0.y + 2 * (1 - t) * t * p1.y + std::pow(t, 2) * p2.y;
		float z = std::pow(1 - t, 2) * p0.z + 2 * (1 - t) * t * p1.z + std::pow(t, 2) * p2.z;
		return Vertex{ x, y, z };
	}

	void Test3DSurfaceBasedOnBezierCurves::GenerateArrayBuffer()
	{
		int pointsCountInRowOrColumn = static_cast<int>(1 / m_UVStep) + 1;
		std::vector<Vertex> sortedVector;
		sortedVector.reserve(m_SurfacePoints.size() * 3 - pointsCountInRowOrColumn);
		for (unsigned int i = 0; i < m_SurfacePoints.size() - pointsCountInRowOrColumn; i++)
		{
			unsigned i1 = (i + 1) % pointsCountInRowOrColumn;
			if (i1 != 0) {
				sortedVector.push_back(m_SurfacePoints[i]);
				sortedVector.push_back(m_SurfacePoints[i + pointsCountInRowOrColumn]);
				sortedVector.push_back(m_SurfacePoints[i + 1]);

				sortedVector.push_back(m_SurfacePoints[i + 1]);
				sortedVector.push_back(m_SurfacePoints[i + pointsCountInRowOrColumn]);
				sortedVector.push_back(m_SurfacePoints[i + pointsCountInRowOrColumn + 1]);
			}
			else
			{
				int a = 5;
			}
		}

		m_VBO = std::make_unique<VertexBuffer>(sortedVector.data(), sortedVector.size() * sizeof(Vertex));
		m_VAO = std::make_unique<VertexArray>();
		m_PointsCount = sortedVector.size();
		VertexBufferLayout layout;
		layout.Push<float>(3);

		m_VAO->AddBuffer(*m_VBO, layout);
	}

	void Test3DSurfaceBasedOnBezierCurves::InitializePortion()
	{
		{
			std::array<Vertex, 5> cp1;
			std::array<Vertex, 5> cp2;
			std::array<Vertex, 5> cp3;

			cp1[0] = { 0.8f, 0, 0.7f };
			cp1[1] = { 0.4f, 0.6f, 0.7f };
			cp1[2] = { 0, 0.6f, 0.7f };
			cp1[3] = { -0.4f, 0.6f, 0.7f };
			cp1[4] = { -0.8f, 0, 0.7f };

			cp2[0] = { 0.8f, 0, 0.0f };
			cp2[1] = { 0.4f, 0.6f, 0.0f };
			cp2[2] = { 0, 0.6f, 0.0f };
			cp2[3] = { -0.4f, 0.6f, 0.0f };
			cp2[4] = { -0.8f, 0, 0.0f };

			cp3[0] = { 0.8f, 0, -0.7f };
			cp3[1] = { 0.4f, 0.6f, -0.7f };
			cp3[2] = { 0, 0.6f, -0.7f };
			cp3[3] = { -0.4f, 0.6f, -0.7f };
			cp3[4] = { -0.8f, 0, -0.7f };
			m_Portions.push_back({ cp1, cp2, cp3 });
		}
	}



}
