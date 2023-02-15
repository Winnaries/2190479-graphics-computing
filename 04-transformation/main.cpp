#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

const float SWEDISH_FLAG_VERTICES[] = {
    // XY           // UV
    -8.0f, -5.0f,    0.0f,  0.0f, 
     8.0f, -5.0f,    1.0f,  0.0f, 
     8.0f,  5.0f,    1.0f,  1.0f, 
    -8.0f, -5.0f,    0.0f,  0.0f, 
     8.0f,  5.0f,    1.0f,  1.0f, 
    -8.0f,  5.0f,    0.0f,  1.0f, 
};

const char *VERTEX_SHADER =
    "#version 330\n"
    "layout(location = 0) in vec2 vertexXy;"
    "layout(location = 1) in vec2 vertexUv;"
    "out vec2 uv;"
    "uniform mat4 transform;"
    "void main() {"
    "   gl_Position = transform * vec4(vertexXy, 0.0f, 1.0f);"
    "   gl_Position.xy = gl_Position.xy / vec2(16.0f, 10.0f);"
    "   uv = vertexUv;"
    "}";

const char *FRAGMENT_SHADER =
    "#version 330\n"
    "in vec2 uv;"
    "out vec3 color;"
    "uniform sampler2D textureSampler;"
    "void main() {"
    "   color = texture(textureSampler, uv).rgb;"
    "}";

int main()
{
    if (!glfwInit())
    {
        return -1;
    }

    std::cerr << std::endl; 

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); 

    GLFWwindow *window = glfwCreateWindow(960, 600, "Nattawat Pornthisan", NULL, NULL);

    glfwMakeContextCurrent(window);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glewExperimental = true;
    glewInit();

    GLuint vao; 
    glGenVertexArrays(1, &vao); 
    glBindVertexArray(vao); 

    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(SWEDISH_FLAG_VERTICES), SWEDISH_FLAG_VERTICES, GL_STATIC_DRAW);

    GLuint texture; 
    glGenTextures(1, &texture); 
    glBindTexture(GL_TEXTURE_2D, texture); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

    int width, height, nrChannels; 
    unsigned char *data = stbi_load("flag.png", &width, &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); 
    }
    else
        std::cout << "Failed to load texture" << std::endl;

    stbi_image_free(data); 

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &VERTEX_SHADER, NULL);
    glCompileShader(vs);

    GLint success = 0; 
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    std::cerr << "Vertex Shader Status: " << success << std::endl; 

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &FRAGMENT_SHADER, NULL);
    glCompileShader(fs);

    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    std::cerr << "Fragment Shader Status: " << success << std::endl;

    GLuint program = glCreateProgram();
    glAttachShader(program, fs);
    glAttachShader(program, vs);
    glLinkProgram(program);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); 

    float speed = 0.0001f; 
    float xPosition = -12.0f; 
    float prevTime = (float)glfwGetTime(); 
    bool speedLocked = false; 

    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 axisX = glm::vec3(1.0f, 0.0f, 0.0f);
    glm::vec3 axisY = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 axisZ = glm::vec3(0.0f, 0.0f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true); 
        }

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && !speedLocked)
        {
            speed *= 2.0; 
            speedLocked = true; 
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && !speedLocked)
        {
            speed *= 0.5; 
            speedLocked = true; 
        }

        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE && speedLocked)
        {
            speedLocked = false; 
        }

        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE && speedLocked)
        {
            speedLocked = false; 
        }

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        {
            rotation = -axisY; 
        }

        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        {
            rotation = axisY;
        }

        if (xPosition > 12.0f) {
            xPosition = -12.0f;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer); 
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)(2 * sizeof(float))); 
        glEnableVertexAttribArray(1); 

        glBindTexture(GL_TEXTURE_2D, texture);

        float time = (float)glfwGetTime();
        xPosition += speed * (time - prevTime);
        prevTime = time; 

        glm::mat4 tfMatrix = glm::mat4(1.0f);
        tfMatrix = glm::translate(tfMatrix, glm::vec3(xPosition, 0.0f, 0.0f));
        tfMatrix = glm::rotate(tfMatrix, time, rotation); // Around Z-axis

        unsigned int tfLocation = glGetUniformLocation(program, "transform");
        glUniformMatrix4fv(tfLocation, 1, GL_FALSE, glm::value_ptr(tfMatrix));

        glUseProgram(program);
        glDrawArrays(GL_TRIANGLES, 0, 6); 

        glDisableVertexAttribArray(0); 
        glDisableVertexAttribArray(1); 
        glfwSwapBuffers(window);
        glfwPollEvents(); 
    }

    glfwTerminate();
    return 0;
}