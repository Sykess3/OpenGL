#pragma once
#include "DataStructs.h"
#include "memory"
#include "Shader.h"
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
		float m_LastGeneratedStep;

		BezierSurface m_BezierSurface;

		std::vector<Portion> m_Portions;

		std::vector<std::shared_ptr<RenderData>> m_RenderData;
		std::vector<std::tuple<glm::vec3, glm::vec3>> m_Normals;
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
