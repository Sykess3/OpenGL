#pragma once
#include "Test.h"
#include "DataStructs.h"
#include "memory"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

namespace test
{

	class Test3DSurfaceBasedOnBezierCurves : public Test
	{
	private:
		float m_UVStep;
		glm::mat4 m_Proj, m_View;
		glm::vec3 m_RotationAxis;
		float m_RotationValue;
		unsigned int m_PointsCount;

		float m_LastGeneratedStep;

		std::vector<Portion> m_Portions;
		std::vector<Vertex> m_SurfacePoints;

		std::unique_ptr<VertexBuffer> m_VBO;
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<Shader> m_Shader;

	public:
		Test3DSurfaceBasedOnBezierCurves();

		void OnUpdate(float deltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;

	private:
		void InitializePortion();
		Vertex Bezier5(Vertex p0, Vertex p1, Vertex p2, Vertex p3, Vertex p4, float t);
		Vertex Bezier3(Vertex p0, Vertex p1, Vertex p2, float t);
		void GenerateSurfacePoints();
		void GenerateArrayBuffer();
	};

}
