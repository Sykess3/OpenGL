
#include "TestClearColor.h"
#include "Renderer.h"
#include "imgui/imgui.h"

namespace test
{
	TestClearColor::TestClearColor()
		: m_ClearColor{0.1, 0.1, 0.1, 1}
	{
		
	}

	TestClearColor::~TestClearColor()
	{
		
	}

	void TestClearColor::OnUpdate(float deltaTime)
	{
		
	}

	void TestClearColor::OnRender()
	{
		GlCall(glClearColor(m_ClearColor[0], m_ClearColor[1], m_ClearColor[2], m_ClearColor[3]));
		GlCall(glClear(GL_COLOR_BUFFER_BIT));
	}
	void TestClearColor::OnImGuiRender()
	{
		ImGui::ColorEdit4("Clear Color", m_ClearColor);
	}


}
