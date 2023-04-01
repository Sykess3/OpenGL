#include "Lab2.h"


#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Constant.h"
#include "test/bezierSurface/DataStructs.h"
#include "test/bezierSurface/LineRenderer.h"
#include "Renderer.h"
#include "VertexBufferLayout.h"
#include "Ñamera.h"
#include "imgui/imgui.h"

namespace test
{
	Lab2::Lab2()
		: m_UVStep(0.1f),
		m_Proj(glm::perspective(glm::radians(45.0f), (float)Constant::Width / (float)Constant::Height, 0.1f, 100.0f)),
		m_View(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -3))),
		m_RotationAxis(0.0f, 1.0f, 0.0f),
		m_RotationValue(90),
		m_Portions(0),
		m_Scale(1, 1, 1)
	{
		GlCall(glEnable(GL_BLEND));
		GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		GlCall(glEnable(GL_DEPTH_TEST));
		InitializePortion();
		InitializeRenderData();
		m_SurfaceShader = std::make_unique<Shader>("res/shaders/BasicColor.shader");
		m_SurfaceShader->Bind();
		m_SurfaceShader->SetUniform4f("objectColor", 1.0f, 0.0f, 0.0f, 1.0f);

		m_LightObjectShader = std::make_unique<Shader>("res/shaders/BasicNotLightable.shader");
		m_LightObjectShader->Bind();
		m_LightObjectShader->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
		
	}

	void Lab2::OnUpdate(float deltaTime)
	{
		if (m_UVStep != m_LastGeneratedStep)
		{
			InitializeRenderData();
		}
	}

	void Lab2::OnRender()
	{
		GlCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));

		float lightX = 5.0f;
		float lightY = 0.0f;
		float lightZ = 5.0f;
		glm::vec3 lightPos = glm::vec3(lightX, lightY, lightZ);
		Renderer renderer;
		{

			glm::mat4 model = glm::scale(glm::mat4(1), m_Scale);
			model = glm::rotate(model, glm::radians(m_RotationValue), m_RotationAxis);
			model = glm::translate(model, m_Translate);

			glm::mat4 view = Camera::GetInstance()->GetViewMatrix();
			glm::mat4 projection = glm::perspective(glm::radians(Camera::GetInstance()->Zoom),
				(float)Constant::Width / (float)Constant::Height, 0.1f, 100.0f);

			glm::mat4 mvp = projection * view * model;
			m_SurfaceShader->Bind();


			m_SurfaceShader->SetUniform3f("material.ambient", glm::vec3(1.0f, 0.5f, 0.31f));
			m_SurfaceShader->SetUniform3f("material.diffuse", glm::vec3(1.0f, 0.5f, 0.31f));
			m_SurfaceShader->SetUniform3f("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
			m_SurfaceShader->SetUniform1f("material.shininess", 32.0f);

			m_SurfaceShader->SetUniform3f("light.ambient", glm::vec3(0.2f, 0.2f, 0.2f));
			m_SurfaceShader->SetUniform3f("light.diffuse", glm::vec3(0.5f, 0.5f, 0.5f)); // darken diffuse light a bit
			m_SurfaceShader->SetUniform3f("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));

			m_SurfaceShader->SetUniform3f("lightPos", lightPos);
			m_SurfaceShader->SetUniform4f("objectColor", 1.0f, 0.0f, 0.0f, 1.0);
			m_SurfaceShader->SetUniform4f("lightColor", 1.0f, 1.0f, 1.0f, 1.0);
			m_SurfaceShader->SetUniform3f("viewPos", Camera::GetInstance()->Position);

			m_SurfaceShader->SetUniformMat4f("projection", projection);
			m_SurfaceShader->SetUniformMat4f("view", view);
			m_SurfaceShader->SetUniformMat4f("model", model);
			for (auto renderData : m_RenderData)
			{
				renderer.Draw(*m_SurfaceShader, *std::move(renderData->IBO), *std::move(renderData->VAO));
			}

			model = glm::mat4(1.0f);
			model = glm::translate(model, lightPos);
			model = glm::scale(model, glm::vec3(0.2f));
			

			m_LightObjectShader->Bind();
			m_LightObjectShader->SetUniform4f("objectColor", 1.0f, 1.0f, .0f, 1.0);
			m_LightObjectShader->SetUniform4f("lightColor", 1.0f, 1.0f, 1.0f, 1.0);
			m_LightObjectShader->SetUniformMat4f("projection", projection);
			m_LightObjectShader->SetUniformMat4f("view", view);
			m_LightObjectShader->SetUniformMat4f("model", model);
			
			for (auto renderData : m_RenderData)
			{
				renderer.Draw(*m_LightObjectShader, *std::move(renderData->IBO), *std::move(renderData->VAO));
			}
		}
	}

	void Lab2::OnImGuiRender()
	{
		ImGui::SliderFloat3("Rotation Axis", &m_RotationAxis.x, -1, 1);
		ImGui::SliderFloat("Rotation Value", &m_RotationValue, -180, 180);
		ImGui::SliderFloat3("Translation", &m_Translate.x, -1, 1);
		ImGui::SliderFloat3("Scale", &m_Scale.x, 0, 5);
		ImGui::SliderFloat("Step", &m_UVStep, 0.01, 0.1);
	}

	void Lab2::InitializeRenderData()
	{
		m_RenderData = m_BezierSurface.GetRenderData(m_Portions, m_UVStep, m_Normals);

		m_LastGeneratedStep = m_UVStep;
	}

	void Lab2::InitializePortion()
	{
		{
			std::array<Vertex, 5> cp1{};
			std::array<Vertex, 5> cp2{};
			std::array<Vertex, 5> cp3{};

			cp1[0] = { -0.3f, -0.4f, 0.7f };
			cp1[1] = { -0.3, 0.6f, -0.4f };
			cp1[2] = { 0, 0.6f, -0.4f };
			cp1[3] = { 0.3, 0.6f, -0.4f };
			cp1[4] = { 0.3f, -0.4f, 0.7f };

			cp2[0] = { -0.5f, 0, -0.0f };
			cp2[1] = { -0.5, 0.6f, -0.4f };
			cp2[2] = { 0, 0.6f, -0.4f };
			cp2[3] = { 0.5, 0.6f, -0.4f };
			cp2[4] = { 0.5f, 0, -0.0f };

			cp3[0] = { -0.3f, -0.0f, -0.7f };
			cp3[1] = { -0.3f, 0.4f, -0.7f };
			cp3[2] = { 0.0f, 0.4f, -0.7f };
			cp3[3] = { 0.3f, 0.4f, -0.7f };
			cp3[4] = { 0.3f, -0.0f, -0.7f };
			m_Portions.push_back({ cp1, cp2, cp3 });
		}
		{
			std::array<Vertex, 5> cp1{};
			std::array<Vertex, 5> cp2{};
			std::array<Vertex, 5> cp3{};

			cp1[0] = { 0.3f, -0.4f, 0.7f };
			cp1[1] = { 0.3f, -0.6f, 0.7f };
			cp1[2] = { 0, -0.6f, 0.7f };
			cp1[3] = { -0.3, -0.6f, 0.7f };
			cp1[4] = { -0.3f, -0.4f, 0.7f };

			cp2[0] = { 0.5f, 0, 0.0f };
			cp2[1] = { 0.5, -0.6f, 0.0f };
			cp2[2] = { 0, -0.6f, 0.0f };
			cp2[3] = { -0.5, -0.6f, 0.0f };
			cp2[4] = { -0.5f, 0, 0.0f };

			cp3[0] = { 0.3f, 0.0f, -0.7f };
			cp3[1] = { 0.3f, -0.4f, -0.7f };
			cp3[2] = { -0.0f, -0.4f, -0.7f };
			cp3[3] = { -0.3f, -0.4f, -0.7f };
			cp3[4] = { -0.3f, 0.0f, -0.7f };
			m_Portions.push_back({ cp1, cp2, cp3 });
		}
	}
}
