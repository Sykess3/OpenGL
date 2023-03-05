#pragma once
#include "DataStructs.h"
#include "memory"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "BezierSurface.h"
#include "test/Test.h"

namespace test
{

	class Test3DSurfaceBasedOnBezierCurves : public Test
	{
	private:
		float m_UVStep;
		glm::mat4 m_Proj, m_View;
		glm::vec3 m_RotationAxis;
		float m_RotationValue;
		unsigned int m_VertexCount;
		float m_LastGeneratedStep;

		BezierSurface m_BezierSurface;

		std::vector<Portion> m_Portions;

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
		void InitializeRenderData();
	};

}
