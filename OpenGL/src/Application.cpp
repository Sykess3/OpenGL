#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "Texture.h"
#include "VertexBufferLayout.h"
#include "Constant.h"
#include "Ñamera.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "test/bezierSurface/lab1/Test3DSurfaceBasedOnBezierCurves.h"
#include "test/Test3DCube.h"
#include "test/TestClearColor.h"
#include "test/TestTexture2D.h"
#include "test/bezierSurface/lab2/Lab2.h"

void ProcessInput(GLFWwindow* window);
void Scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void Mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void FramebufferSize_callback(GLFWwindow* window, int width, int height);

bool CursorIsDisabled;


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

    glfwSetCursorPosCallback(window, Mouse_callback);
    glfwSetScrollCallback(window, Scroll_callback);
    glfwSetFramebufferSizeCallback(window, FramebufferSize_callback);

    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK)
        std::cout << "Error" << std::endl;

    //-IMGUI--------------------------------------------
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 330";
    ImGui_ImplOpenGL3_Init(glsl_version);
    /////////////////////////////////////////////////////////

    GlCall(glEnable(GL_BLEND));
    GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    Renderer renderer;

    test::Test* currentTestPointer;
    auto* testMenu = new test::TestMenu(currentTestPointer);
    currentTestPointer = testMenu;

    testMenu->RegisterTest<test::TestClearColor>("Clear Color");
    testMenu->RegisterTest<test::TestTexture2D>("2D Texture");
    testMenu->RegisterTest<test::Test3DCube>("3d Cube");
    //testMenu->RegisterTest<test::Test3DSurfaceBasedOnBezierCurves>("LAB1");
    testMenu->RegisterTest<test::Lab2>("LAB2");

    while (!glfwWindowShouldClose(window))
    {
        GlCall(glEnable(GL_DEPTH_TEST));
        float currentFrame = static_cast<float>(glfwGetTime());
        Camera::deltaTime = currentFrame - Camera::lastFrame;
        Camera::lastFrame = currentFrame;

        ProcessInput(window);

        //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

void ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        Camera::GetInstance()->ProcessKeyboard(FORWARD, Camera::deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        Camera::GetInstance()->ProcessKeyboard(BACKWARD, Camera::deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Camera::GetInstance()->ProcessKeyboard(LEFT, Camera::deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Camera::GetInstance()->ProcessKeyboard(RIGHT, Camera::deltaTime);

    if (glfwGetKey(window, GLFW_KEY_CAPS_LOCK) == GLFW_RELEASE)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    else
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        Camera::GetInstance()->SetPosition(glm::vec3(0.0f, 0.0f, 6.0f));
        Camera::GetInstance()->ProcessMouseScroll(0);
    }
}

// -------------------------------------------------------
void Mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (Camera::firstMouse)
    {
	    Camera::lastX = xpos;
	    Camera::lastY = ypos;
	    Camera::firstMouse = false;
    }

    float xoffset = xpos - Camera::lastX;
    float yoffset = Camera::lastY - ypos; // reversed since y-coordinates go from bottom to top

    Camera::lastX = xpos;
    Camera::lastY = ypos;


    Camera::GetInstance()->ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void Scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Camera::GetInstance()->ProcessMouseScroll(static_cast<float>(yoffset));
}

void FramebufferSize_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

