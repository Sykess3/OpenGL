#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "Texture.h"
#include "VertexBufferLayout.h"
#include "Constant.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "test/bezierSurface/Test3DSurfaceBasedOnBezierCurves.h"
#include "test/Test3DCube.h"
#include "test/TestClearColor.h"
#include "test/TestTexture2D.h"


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
    window = glfwCreateWindow(Constant::Width, Constant::Height, "Hello World", NULL, NULL);
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
    testMenu->RegisterTest<test::TestTexture2D>("2D Texture");
    testMenu->RegisterTest<test::Test3DCube>("3d Cube");
    testMenu->RegisterTest<test::Test3DSurfaceBasedOnBezierCurves>("LAB1");

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

