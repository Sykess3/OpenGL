#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"



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

    float positions[] =
    {
            -0.5f, -0.5f, 0.0f, 0.0f, // 0
            0.5f, -0.5f, 1.0f, 0.0f,  // 1
            0.5f, 0.5f, 1.0f, 1.0f,    // 2
            -0.5f, 0.5f, 0.0f, 1.0f   // 3
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

    va.Unbind();
    shader.Unbind();

    Renderer renderer;

    GlCall(glBindBuffer(GL_ARRAY_BUFFER, 0));

    float r = 0.0f;
    float increment = 0.05f;

    /* loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* render here */
        shader.Bind();
        //shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

        renderer.Clear();
        vb.Bind();
        ib.Bind();

        renderer.Draw(shader, ib, va);

        if (r > 1.0f) {
            increment = -0.05f;
        }
        else if (r < 0.0f) {
            increment = 0.05f;
        }
        r += increment;

        /* swap front and back buffers */
        glfwSwapBuffers(window);

        /* poll for and process events */
		glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

