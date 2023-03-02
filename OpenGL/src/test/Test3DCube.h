#pragma once
#include "Test.h"
#include "VertexArray.h"

#include <memory>

#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include<array>

namespace test
{
	class Test3DCube : public Test
	{
	public:
		Test3DCube();
		~Test3DCube() override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:
		std::array<glm::vec3, 10> m_CubePosition;

		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;

		glm::mat4 m_Proj, m_View;
		glm::vec3 m_TranslationA;
		glm::vec3 m_RotationAxis;
	};
}

