#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "Camera.h"
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
#include "glm/gtc/matrix_transform.hpp"

int WIDTH = 800;
int HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = WIDTH / 2.0f;
float lastY = HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	
float lastFrame = 0.0f;

bool isWireframe = false;
bool _enableCameraControls = true;
bool _leftMouseButtonPressed = false;
bool lightTree = false;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    if (_enableCameraControls && ImGui::GetIO().WantCaptureMouse)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        return;
    }
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        float xpos = static_cast<float>(xposIn);
        float ypos = static_cast<float>(yposIn);

        if (firstMouse)
        {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;

        camera.ProcessMouseMovement(xoffset, yoffset);
    }

}

// ImGui stuff
void FPSCounter(float deltaTime)
{
    float fps = 1.0f / deltaTime;

    ImGui::Text("FPS: %.1f", fps);
}

// End of ImGui stuff


int main(void) 
{
    /* INITIALIZATION */
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Bezier Surface", NULL, NULL);
    if (!window)
    {
        std::cout << "Failed to create GLFW window!\n";
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouse_callback);

    
    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD!\n";
    }
    std::cout << glGetString(GL_VERSION) << '\n';

    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    glEnable(GL_DEPTH_TEST);


    /* ACTUAL CODE STARTS HERE */

    float vertices[] = {                // tex coords
    0.0f, 1.0f, 1.0f,         0.0f, 1.0f,
    1.0 /3.0, 1.0f, 0.0f,     1.0 / 3.0, 1.0f,
    2.0 / 3.0, 1.0f, 0.0f,    2.0 / 3.0, 1.0f,
    1.0f, 1.0f, 0.0f,         1.0f, 1.0f,
    
    0.0f, 2.0/ 3.0, 0.0f,       0.0f, 2.0 / 3.0,
    1.0 / 3.0, 2.0 / 3.0, 0.0f, 1.0 / 3.0, 2.0 / 3.0,
    2.0 / 3.0, 2.0 / 3.0, 0.0f, 2.0 / 3.0, 2.0 / 3.0,
    1.0f, 2.0 / 3.0, 0.0f,      1.0f, 2.0 / 3.0,
    
    0.0f, 1.0 / 3.0, 0.0f,      0.0f, 1.0 / 3.0,
    1.0 / 3.0, 1.0 / 3.0, 0.0f, 1.0 / 3.0, 1.0 / 3.0,
    2.0 / 3.0, 1.0 / 3.0, 0.0f, 2.0 / 3.0, 1.0 / 3.0,
    1.0f, 1.0 / 3.0, 0.0f,      1.0f, 1.0 / 3.0,
    
    0.0f, 0.0f, 0.0f,      0.0f, 0.0f,
    1.0 / 3.0, 0.0f, 0.0f, 1.0 / 3.0, 0.0f,
    2.0 / 3.0, 0.0f, 0.0f, 2.0 / 3.0, 0.0f,
    1.0f, 0.0f, 0.0f,      1.0f, 0.0f,

    };

    /*unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };*/


    VertexArray va;
    VertexBuffer vb(vertices, sizeof(vertices));
    VBLayout layout;

    layout.Push(GL_FLOAT, 3);
    layout.Push(GL_FLOAT, 2);
    va.AddBuffer(vb, layout);

    //IndexBuffer ib(indices, sizeof(indices) / sizeof(unsigned int)); // (, count)

    Shader shader;
    shader.AddShader("res/shaders/texture.vert", ShaderType::VERTEX)
        .AddShader("res/shaders/texture.frag", ShaderType::FRAGMENT)
		.AddShader("res/shaders/texture.tcs", ShaderType::TESS_CTRL)
        .AddShader("res/shaders/texture.tes", ShaderType::TESS_EVAL);
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

    glm::mat4 model;
    glm::mat4 view;
    glm::mat4 proj;
    glm::mat4 MVP;

	glPatchParameteri(GL_PATCH_VERTICES, 16);
    shader.SetUniform1f("TessLevel", 16.0);

    float Kd = 0.5f;
    float Ks = 0.5f;
    float m = 1.0f;
    float zLight = 0.3f;
    bool paused = false;
    float maxArg = 25.0f;
    float minArg = 0.1f;
    float direction = 1.0f;
    glm::vec3 lightColor(1.0, 1.0, 1.0);
    shader.SetUniform1f("Kd", Kd);
    shader.SetUniform1f("Ks", Ks);
    shader.SetUniform1f("m", m);
    shader.SetUniform3f("LightPosition", 0.5f, 0.5f, zLight);
    shader.SetUniform3fv("LightColor", lightColor);

    float arg = 0;
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Process input
        processInput(window);

        // Render
        renderer.Clear();
        renderer.SetPolygonMode(isWireframe ? GL_LINE : GL_FILL);


        // MVP //
        model = glm::rotate(glm::mat4(1.0f),  glm::radians(-80.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::translate(model, glm::vec3(0.0, 0.0, 0.0));
        view = camera.GetViewMatrix();
        proj = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
        MVP = proj * view * model;

        shader.SetUniformMatrix4f("MVP", MVP);

        if (!paused)
        {
            float speed = 4.0f;
            arg += static_cast<float>(deltaTime) * speed * direction;

            if (arg > maxArg || arg < minArg) {
                direction = -direction; 
            }
            float startPos = 0.5f;
            float offset = 0.5;
            float x = arg / 40 * cos(arg);
            float y = arg / 40 * sin(arg);
            float z = zLight;
            shader.SetUniform3f("LightPosition", x + startPos, y + startPos, z);
        }

        // Draw Call
        renderer.Draw(va, shader, sizeof(vertices) / (sizeof(float) * 5));

    	// ImGui here //
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        FPSCounter(deltaTime);
        ImGui::Separator();
        ImGui::Checkbox("Wireframe", &isWireframe);

        // Light
        ImGui::SetNextItemOpen(true, ImGuiCond_Once);
        if (ImGui::TreeNode("Light"))
        {
			ImGui::Text("Parameters:");
            ImGui::SliderFloat("Z-Position", &zLight, 0.0, 2.0);
            ImGui::SliderFloat("Kd", &Kd, 0.0, 1.0);
            shader.SetUniform1f("Kd", Kd);

            ImGui::SliderFloat("Ks", &Ks, 0.0, 1.0);
            shader.SetUniform1f("Ks", Ks);
            ImGui::SliderFloat("m", &m, 1.0, 100.0);
            shader.SetUniform1f("m", m);

            ImGui::ColorEdit3("Color", (float*)&lightColor);
            shader.SetUniform3fv("LightColor", lightColor);

            if (ImGui::Button("Toggle Animation")) {
                paused = !paused;
            }
            if(ImGui::Button("reset"))
            {
                arg = 0;
            }
            ImGui::TreePop();
        }
        //
        if(ImGui::TreeNode("Bezier"))
        {
            for (int i = 0; i < 16; i++)
            {
                ImGui::SliderFloat(
                    "Slider", &vertices[i * 5 + 2],
                    0.0f, 1.0f                                // Slider range
                );
                vb.Update(vertices, sizeof(vertices));
            }


            ImGui::TreePop();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        //ImGui end //

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


