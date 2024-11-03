//
//  main.cpp
//  colored_building_no_indices
//
//  Created by Nazirul Hasan on 26/8/23.
//  Modified by Badiuzzaman
//

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
float rotateAngle = 0.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float scale_X = 1.3;
float scale_Y = 1.3;

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 colorInfo;\n"
"uniform mat4 transform;\n"
"void main()\n"
"{\n"
"   gl_Position = transform * vec4(aPos, 1.0);\n"
"   colorInfo = aColor;\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"in vec3 colorInfo;\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(colorInfo, 1.0f);\n"
"}\n\0";

int main()
{
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Simple Building", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // Load GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Build and compile shader program
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Define vertices with color data for each vertex
    float vertices[] = {
        // main building
         0.3f, -0.5f, 0.0f,  0.6f, 0.5f, 0.74f, // bottom right, red
        -0.3f, -0.5f, 0.0f,  0.6f, 0.5f, 0.74f,  // bottom left, red
         0.3f,  0.5f, 0.0f,  0.6f, 0.5f, 0.74f,  // top right, red
        -0.3f,  0.5f, 0.0f,  0.6f, 0.5f, 0.74f,
         // Second triangle (Blue)
         // 0.3f,  0.5f, 0.0f,  0.137f, 0.612f, 0.506f,  // top right, blue
         //-0.3f,  0.5f, 0.0f,  0.137f, 0.612f, 0.506f,  // top left, blue
         //-0.3f, -0.5f, 0.0f,  0.137f, 0.612f, 0.506f   // bottom left, blue
        //left bottom window

        -0.1f,  -0.3f, 0.0f,  0.0f, 1.0f, 1.0f,
        -0.1f,  -0.2f, 0.0f,  0.0f, 1.0f, 1.0f,
        -0.2f,  -0.3f, 0.0f,  0.0f, 1.0f, 1.0f,
        -0.2f,  -0.2f, 0.0f,  0.0f, 1.0f, 1.0f,

        //right bottom window
        0.2f,  -0.3f, 0.0f,  0.0f, 1.0f, 1.0f,
        0.1f,  -0.3f, 0.0f,  0.0f, 1.0f, 1.0f,
        0.2f,  -0.2f, 0.0f,  0.0f, 1.0f, 1.0f,
        0.1f,  -0.2f, 0.0f,  0.0f, 1.0f, 1.0f,

        //right top window
        0.2f,  0.2f, 0.0f,  0.0f, 1.0f, 1.0f,
        0.1f,  0.2f, 0.0f,  0.0f, 1.0f, 1.0f,
        0.2f,  0.3f, 0.0f,  0.0f, 1.0f, 1.0f,
        0.1f,  0.3f, 0.0f,  0.0f, 1.0f, 1.0f,

        //left top window
        -0.1f,  0.2f, 0.0f,  0.0f, 1.0f, 1.0f,
        -0.2f,  0.2f, 0.0f,  0.0f, 1.0f, 1.0f,
        -0.1f,  0.3f, 0.0f,  0.0f, 1.0f, 1.0f,
        -0.2f,  0.3f, 0.0f,  0.0f, 1.0f, 1.0f,

        //bottom bar
        -0.45f,  -0.54f, 0.0f,  0.25f, 0.25f, 0.25f,
        -0.45f,  -0.5f, 0.0f,  0.25f, 0.25f, 0.25f,
        0.34f,  -0.54f, 0.0f,  0.25f, 0.25f, 0.25f,
        0.34f,  -0.5f, 0.0f,   0.25f, 0.25f, 0.25f,

        //top stair
        0.1f,  0.5f, 0.0f,  0.21f, 0.21f, 0.21f,
        0.1f,  0.6f, 0.0f,  0.21f, 0.21f, 0.21f,
        0.2f,  0.5f, 0.0f,  1.0f, 0.612f, 0.506f,
        0.2f,  0.6f, 0.0f,  1.0f, 0.612f, 0.506f,
        
        //top roof
        0.075f,  0.6f, 0.0f,  0.23f, 0.43f, 0.55f,
        0.225f,  0.6f, 0.0f,  0.23f, 0.43f, 0.55f,
        0.15f,  0.65f, 0.0f,  0.23f, 0.43f, 0.55f,

        //door
        -0.06f,  -0.5f, 0.0f,   0.29f, 0.21f, 0.15f,
        -0.06f,  -0.36f, 0.0f,  0.29f, 0.21f, 0.15f,
        0.06f,  -0.5f, 0.0f,  0.29f, 0.21f, 0.15f,
        0.06f,  -0.36f, 0.0f,  0.29f, 0.21f, 0.15f,

        

        //midline
        -0.3f,  -0.025f, 0.0f,  0.21f, 0.21f, 0.21f,
        -0.3f,  0.025f, 0.0f,  0.21f, 0.21f, 0.21f,
        0.3f,  -0.025f, 0.0f,  0.0f, 0.5f, 0.5f,
        0.3f,  0.025f, 0.0f,  0.0f, 0.5f, 0.5f,

        //midline line
        0.3f,  0.0f, 0.0f,  1.0f, 1.0f, 1.0f,
        -0.3f,  0.0f, 0.0f,  1.0f, 1.0f, 1.0f,

        //door triangle
        -0.06f,  -0.36f, 0.0f,  1.0f, 1.0f, 1.0f,
        0.06f,  -0.36f, 0.0f,  1.0f, 1.0f, 1.0f,
        0.0f,  -0.4f, 0.0f,  0.63f, 0.27f, 0.0f,

        //door line
        0.0f,  -0.4f, 0.0f,  1.0f, 1.0f, 1.0f,
        0.0f,  -0.5f, 0.0f,  1.0f, 1.0f, 1.0f,

        // building back 
        -0.3f,  0.5f, 0.0f,  0.6f, 0.5f, 0.74f,
        -0.3f, 0.4f, 0.0f,  0.6f, 0.5f, 0.74f,
        -0.4f,  0.4f, 0.0f,  0.6f, 0.5f, 0.74f,
        -0.3f,  -0.5f, 0.0f,  0.6f, 0.5f, 0.74f,
        -0.4f,  -0.5f, 0.0f,  0.6f, 0.5f, 0.74f,

        
        

        //left bottom window line v
        -0.15f,  -0.3f, 0.0f,  0.0f, 0.0f, 0.0f,
        -0.15f,  -0.2f, 0.0f,  0.0f, 0.0f, 0.0f,

        //right bottom window line v
       0.15f,  -0.3f, 0.0f,  0.0f, 0.0f, 0.0f,
       0.15f,  -0.2f, 0.0f,  0.0f, 0.0f, 0.0f,

       //right top window line v
       0.15f,  0.2f, 0.0f,  0.0f, 0.0f, 0.0f,
       0.15f,  0.3f, 0.0f, 0.0f, 0.0f, 0.0f,

       //left top window line v
       -0.15f,  0.2f, 0.0f, 0.0f, 0.0f, 0.0f,
       -0.15f,  0.3f, 0.0f, 0.0f, 0.0f, 0.0f,

       //midline left
       -0.3f,  -0.025f, 0.0f,  0.21f, 0.21f, 0.21f,
       -0.3f,  0.025f, 0.0f,  0.21f, 0.21f, 0.21f,
       -0.4f,  -0.05f, 0.0f,  0.0f, 0.5f, 0.5f,
       -0.4f,  0.0f, 0.0f,  0.0f, 0.5f, 0.5f,
        
       //midline line left
        -0.4f, -0.025f, 0.0f, 1.0f, 1.0f, 1.0f,
       -0.3f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,

       //building line
        -0.3f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.3f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f,

        //top stair door
        0.14f, 0.5f, 0.0f, 0.23f, 0.43f, 0.55f,
        0.14f, 0.56f, 0.0f, 0.23f, 0.43f, 0.55f,
        0.16f, 0.5f, 0.0f, 0.23f, 0.43f, 0.55f,
        0.16f, 0.56f, 0.0f, 0.23f, 0.43f, 0.55f,

        //left bottom window
        
        -0.1f,  -0.2f, 0.0f,  0.0f, 0.28f, 0.53f,
        -0.15f, -0.15f, 0.0f, 0.0f, 0.28f, 0.53f,
        -0.2f,  -0.2f, 0.0f,  0.0f, 0.28f, 0.53f,
        
        //right bottom window
        0.15f, -0.15f, 0.0f, 0.0f, 0.28f, 0.53f,
        0.2f,  -0.2f, 0.0f, 0.0f, 0.28f, 0.53f,
        0.1f,  -0.2f, 0.0f, 0.0f, 0.28f, 0.53f,

        //right top window
        0.15f, 0.35f, 0.0f, 0.0f, 0.28f, 0.53f,
        0.2f,  0.3f, 0.0f, 0.0f, 0.28f, 0.53f,
        0.1f,  0.3f, 0.0f, 0.0f, 0.28f, 0.53f,

        //left top window
        -0.15f, 0.35f, 0.0f, 0.0f, 0.28f, 0.53f,
        -0.1f,  0.3f, 0.0f, 0.0f, 0.28f, 0.53f,
        -0.2f,  0.3f, 0.0f, 0.0f, 0.28f, 0.53f,
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Render loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(translate_X, translate_Y, 0.0f));
        glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(rotateAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale_X, scale_Y, 1.0f));
        glm::mat4 modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;

        glUseProgram(shaderProgram);
        unsigned int transformLoc = glGetUniformLocation(shaderProgram, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(modelMatrix));

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

        glDrawArrays(GL_TRIANGLE_STRIP,4,4);
        glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 20, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 24, 4);
        glDrawArrays(GL_TRIANGLES,28,3);
        glDrawArrays(GL_TRIANGLE_STRIP, 31, 4);
        glDrawArrays(GL_TRIANGLE_STRIP, 35, 4);
        glDrawArrays(GL_LINES, 39, 2);
        glDrawArrays(GL_TRIANGLES, 41, 3);
        glDrawArrays(GL_LINES, 44, 2);
        glDrawArrays(GL_TRIANGLE_STRIP, 46, 5);
        
        glDrawArrays(GL_LINES, 51, 8);
        glDrawArrays(GL_TRIANGLE_STRIP, 59, 4);
        glDrawArrays(GL_LINES, 63, 2);
        glDrawArrays(GL_LINES, 65, 2);
        glDrawArrays(GL_TRIANGLE_STRIP, 67, 4);
        glDrawArrays(GL_TRIANGLES, 71, 12);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    const float translationSpeed = 0.01f;
    const float rotatespeed = 1.0f;
    const float scalingspeed = 0.01f;

    // Move right
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        translate_X += translationSpeed;

    // Move left
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        translate_X -= translationSpeed;

    // Move up
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        translate_Y += translationSpeed;

    // Move down
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        translate_Y -= translationSpeed;

    //rotate anticlockwise
    if (glfwGetKey(window, 'Q') == GLFW_PRESS)
        rotateAngle += rotatespeed;

    //rotate clockwise
    if (glfwGetKey(window, 'E') == GLFW_PRESS)
        rotateAngle -= rotatespeed;

    if (glfwGetKey(window, 'W') == GLFW_PRESS)
    {
        scale_X += scalingspeed;
        scale_Y += scalingspeed;
    }
    //rotate clockwise
    if (glfwGetKey(window, 'S') == GLFW_PRESS)
    {
        scale_X -= scalingspeed;
        scale_Y -= scalingspeed;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
