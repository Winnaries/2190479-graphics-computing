#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define BLUE            0.000f, 0.415f, 0.654f
#define YELLOW          0.996f, 0.800f, 0.008f

const float SWEDISH_FLAG_VERTICES[] = {
     0.0f,  4.0f,  0.0f, 
    16.0f,  4.0f,  0.0f, 
     0.0f,  6.0f,  0.0f, 

    16.0f,  4.0f,  0.0f, 
    16.0f,  6.0f,  0.0f, 
     0.0f,  6.0f,  0.0f, 

     5.0f,  0.0f,  0.0f, 
     7.0f,  0.0f,  0.0f, 
     5.0f, 10.0f,  0.0f, 

     7.0f,  0.0f,  0.0f, 
     7.0f, 10.0f,  0.0f, 
     5.0f, 10.0f,  0.0f, 
};

const float SWEDISH_FLAG_COLORS[] = {
    YELLOW, YELLOW, YELLOW,
    YELLOW, YELLOW, YELLOW,
    YELLOW, YELLOW, YELLOW,
    YELLOW, YELLOW, YELLOW,
};

const char *VERTEX_SHADER =
    "#version 330 core\n"
    "layout(location = 0) in vec3 position;"
    "layout(location = 1) in vec3 vertexColor;"
    "out vec3 fragmentColor;"
    "void main() {"
    "   gl_Position.x = position.x / 8.0f - 1.0f;"
    "   gl_Position.y = position.y / 5.0f - 1.0f;"
    "   gl_Position.z = 0.0f;"
    "   gl_Position.w = 1.0f;"
    "   fragmentColor = vertexColor;"
    "}";

const char *FRAGMENT_SHADER =
    "#version 330 core\n"
    "in vec3 fragmentColor;"
    "out vec3 color;"
    "void main() {"
    "   color = fragmentColor;"
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

    GLuint colorBuffer;
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(SWEDISH_FLAG_COLORS), SWEDISH_FLAG_COLORS, GL_STATIC_DRAW);

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
    glClearColor(BLUE, 1.0f); 

    while (!glfwWindowShouldClose(window))
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, true); 
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer); 
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, colorBuffer); 
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0); 
        glEnableVertexAttribArray(1);

        glUseProgram(program);
        glDrawArrays(GL_TRIANGLES, 0, 12); 

        glDisableVertexAttribArray(0); 
        glDisableVertexAttribArray(1); 
        glfwSwapBuffers(window);
        glfwPollEvents(); 
    }

    glfwTerminate();
    return 0;
}