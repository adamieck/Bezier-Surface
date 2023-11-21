#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "glm/glm.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
void ShowFPSCounter()
{
    static float fps = 0.0f;
    static float frameTime = 0.0f;
    static float lastTime = 0.0f;

    float currentTime = glfwGetTime();
    frameTime = currentTime - lastTime;
    lastTime = currentTime;

    fps = 1.0f / frameTime;

    ImGui::Text("FPS: %.1f", fps);
}

int main(void) 
{
    /* INITIALIZATION */
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(800, 600, "Hello Window", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window!\n";
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    
    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD!\n";
    }
    std::cout << glGetString(GL_VERSION) << '\n';

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    /* ACTUAL CODE STARTS HERE */

    float vertices[] = {    // tex coords
    -1.0f, 1.0f, 0.0f,      0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f,     0.0f, 0.0f,
    1.0f, -1.0f, 0.0f,      1.0f, 0.0f,
    1.0f, 1.0f, 0.0f,       1.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    VertexArray va;
    VertexBuffer vb(vertices, sizeof(vertices));
    VBLayout layout;

    layout.Push(GL_FLOAT, 3);
    layout.Push(GL_FLOAT, 2);
    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, sizeof(indices) / sizeof(unsigned int)); // (, count)

    Shader shader;
    shader.AddShader("res/shaders/basic.vert", ShaderType::VERTEX)
        .AddShader("res/shaders/basic.frag", ShaderType::FRAGMENT);
    shader.Build();
    shader.Bind();

    Texture tex("res/textures/cat.png");
    tex.Bind();
    shader.SetUniform1i("_Tex", 0); // (name , tex_slot)

    Renderer renderer;

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 410");

    while (!glfwWindowShouldClose(window))
    {
        // Process input
        processInput(window);

        // Render
        renderer.Clear();
        renderer.Draw(va, ib, shader);
        
        //double currentTime = glfwGetTime();
        //shader.SetUniform1f("iTime", (float)currentTime);

    	// Start ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ShowFPSCounter();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

	glfwTerminate();
    return 0;
}


