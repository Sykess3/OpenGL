#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "test/TestClearColor.h"


int main(void)
{
    GLFWwindow* window;
    /* Initialize the library */
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
	glfwMakeContextCurrent(window);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error" << std::endl;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 330";
    ImGui_ImplOpenGL3_Init(glsl_version);

    GlCall(glEnable(GL_BLEND));
    GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    Renderer renderer;

    test::Test* currentTestPointer;
    auto* testMenu = new test::TestMenu(currentTestPointer);
    currentTestPointer = testMenu;

    testMenu->RegisterTest<test::TestClearColor>("Clear Color");

    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0, 0, 0, 1);
        renderer.Clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (currentTestPointer)
        {
            currentTestPointer->OnUpdate(0);
            currentTestPointer->OnRender();

            ImGui::Begin("Test");
            if (currentTestPointer != testMenu && ImGui::Button("<-"))
            {
                delete currentTestPointer;
                currentTestPointer = testMenu;
            }

            currentTestPointer->OnImGuiRender();
            ImGui::End();
        }



        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);

		glfwPollEvents();
    }

    if (currentTestPointer != testMenu)
    {
        delete testMenu;
    }

    delete currentTestPointer;

    glfwTerminate();
    return 0;
}

