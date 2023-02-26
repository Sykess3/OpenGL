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

    float positions[] =
    {
            100.0f, 100.0f, 0.0f, 0.0f,  // 0
            200.0f, 100.0f, 1.0f, 0.0f,  // 1
            200.0f, 200.0f, 1.0f, 1.0f,  // 2
            100.0f, 200.0f, 0.0f, 1.0f   // 3
    };

    unsigned int indices[] =
    {
        0, 1, 2,
        2, 3, 0
    };

    GlCall(glEnable(GL_BLEND));
    GlCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));


	const VertexBuffer vb(positions, 4 * 4 * sizeof(float));
    const IndexBuffer ib(indices, 6);
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    VertexArray va;
    va.AddBuffer(vb, layout);

    Shader shader("res/shaders/Basic.shader");
    shader.Bind();
    //shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

    Texture texture("res/textures/Test.png");
    texture.Bind(0);
    shader.SetUniform1i("u_Texture", 0);

    glm::mat4 projection = glm::ortho(0.0f, 640.0f, 0.0f, 480.0f, -1.0f, 1.0f);
    glm::mat4 view = glm::translate(glm::mat4(1), glm::vec3(-100, 0, 0));

    va.Unbind();
    shader.Unbind();

    Renderer renderer;

    glm::vec3 translation(100, 0, 0);
    /* loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        renderer.Clear();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        shader.Bind();
        vb.Bind();
        ib.Bind();

        glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);

        glm::mat4 mvp = projection * view * model;

        shader.SetUniformMat4f("u_MVP", mvp);

        renderer.Draw(shader, ib, va);

        {
            ImGui::Begin("Hello, world!");
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::SliderFloat("Translation", &translation.x, 0.0f, 640.0f);
            ImGui::End();
        }


        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        /* swap front and back buffers */
        glfwSwapBuffers(window);

        /* poll for and process events */
		glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

