#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "VertexBuffer.h"
#include "IndexBuffer.h"

static void parseShader(const std::string& filepath, std::string* outVertexShader, std::string* outFragmentShader)
{
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    ShaderType type = ShaderType::NONE;
    std::ifstream stream(filepath);
    std::string line;
    
    std::stringstream ss[2];

    while (std::getline(stream, line))
    {
        if (line.find("#shader") != std::string::npos)
        {
            if (line.find("vertex") != std::string::npos)
                type = ShaderType::VERTEX;
            else if (line.find("fragment") != std::string::npos)
                type = ShaderType::FRAGMENT;
        }
        else if (type != ShaderType::NONE)
        {
            ss[static_cast<int>(type)] << line << '\n';
        }
    }

    if (outVertexShader)
        *outVertexShader = ss[0].str();
    if (outFragmentShader)
        *outFragmentShader = ss[1].str();
}

static unsigned int compileShader(unsigned int type, const std::string& source)
{
    unsigned int shaderID = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shaderID, 1, &src, nullptr);
    glCompileShader(shaderID);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        std::cout << "Failed to compile" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "shader\n";
        std::cout << infoLog << std::endl;
        glDeleteShader(shaderID);
        return 0;
    }
    return shaderID;
}

static int createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int shaderProgram = glCreateProgram();
    unsigned int vertexShaderID = compileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fragmentShaderID = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(shaderProgram, vertexShaderID);
    glAttachShader(shaderProgram, fragmentShaderID);
    glLinkProgram(shaderProgram);
    glValidateProgram(shaderProgram);
    // check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "Error linking shaders\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
    return shaderProgram;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main(void) 
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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


    float vertices[] = {
    -1.0f, 1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,

    1.0f, 1.0f, 0.0f // additional vertex (square-maker)
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    VertexBuffer vb(vertices, sizeof(vertices));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    IndexBuffer ib(indices, sizeof(indices) / sizeof(unsigned int)); // (, count)

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    std::string vertexShaderSource, fragmentShaderSource;
    parseShader("res/shaders/fancy.shader", &vertexShaderSource, &fragmentShaderSource);
    unsigned int shaderProgram = createShader(vertexShaderSource, fragmentShaderSource);
    glUseProgram(shaderProgram);

    // add iResolution uniform
    int viewportWidth, viewportHeight;
    glfwGetFramebufferSize(window, &viewportWidth, &viewportHeight);
    glUniform3f(glGetUniformLocation(shaderProgram, "iResolution"),
        static_cast<float>(viewportWidth), static_cast<float>(viewportHeight),
        static_cast<float>(viewportWidth) / static_cast<float>(viewportHeight));


    while (!glfwWindowShouldClose(window))
    {
        // Process input
        processInput(window);

        // Render
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // bg
        glClear(GL_COLOR_BUFFER_BIT);

        
        float currentTime = glfwGetTime();
        glUniform1f(glGetUniformLocation(shaderProgram, "iTime"), currentTime);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
    return 0;
}


