#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

const char *VERTEX_SHADER =
    "#version 330 core\n"
    "layout (location = 0) in vec3 vertexPosition;"
    "layout (location = 1) in vec3 vertexNormal;"
    "uniform mat4 model;"
    "uniform mat4 view;"
    "uniform mat4 projection;"
    "out vec3 normal;"
    "out vec3 position;"
    "void main() {"
    "   gl_Position = projection * view * model * vec4(vertexPosition, 1.0);"
    "   normal = mat3(transpose(inverse(model))) * vertexNormal;"
    "   position = vec3(model * vec4(vertexPosition, 1.0)); " 
    "}";

const char *FRAGMENT_SHADER =
    "#version 330\n"
    "in vec3 normal;"
    "in vec3 position;"
    "uniform vec3 lightPosition;"
    "out vec3 color;"
    "void main() {"
    "   float ambientStrength = 0.1f;"
    "   vec3 objectColor = vec3(1.0f, 0.1f, 0.1f);"
    "   vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);"
    ""
    "   vec3 lightDirection = normalize(lightPosition - position);"
    "   float diffuseStrength = max(dot(normal, lightDirection), 0.0);"
    ""
    "   vec3 diffuse = diffuseStrength * lightColor;"
    "   vec3 ambient = ambientStrength * lightColor;"
    ""
    "   color = (ambient + diffuse) * objectColor;"
    "}";

void createSphereVertices(float radius, int slices, int stacks, std::vector<float>& vertices) {
    float phi0, phi1, theta; 
    float x, y, z; 
    float nx, ny, nz; 

    for (int i = 0; i < stacks; i++) {
        phi0 = i * M_PI / (float)stacks; 
        for (int j = 0; j <= slices; j++) {
            theta = j * 2.0 * M_PI / (float)slices; 
            x = radius * cos(theta) * sin(phi0); 
            y = radius * sin(theta) * sin(phi0); 
            z = radius * cos(phi0);
            nx = x / radius;
            ny = y / radius;
            nz = z / radius;
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            vertices.push_back(nx);
            vertices.push_back(ny);
            vertices.push_back(nz);

            phi1 = (i + 1) * M_PI / (float)stacks;
            x = radius * cos(theta) * sin(phi1);
            y = radius * sin(theta) * sin(phi1);
            z = radius * cos(phi1);
            nx = x / radius;
            ny = y / radius;
            nz = z / radius;
            vertices.push_back(x);
            vertices.push_back(y);
            vertices.push_back(z);
            vertices.push_back(nx);
            vertices.push_back(ny);
            vertices.push_back(nz);
        }
    }
}

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

    GLint success = 0;
    glewExperimental = true;
    glewInit();

    // Compiling shaders.
    // Don't mess with this part!
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &VERTEX_SHADER, NULL);
    glCompileShader(vs);

    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    std::cerr << "Vertex Shader Status: " << success << std::endl;

    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &FRAGMENT_SHADER, NULL);
    glCompileShader(fs);

    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    std::cerr << "Fragment Shader Status: " << success << std::endl;

    // Attaching shaders, 
    // again don't mess with this!
    GLuint program = glCreateProgram();
    glAttachShader(program, fs);
    glAttachShader(program, vs);
    glLinkProgram(program);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    // Perform a sphere tessellation 
    // to verticize the sphere. 
    std::vector<float> vertices;
    createSphereVertices(1, 100, 100, vertices);

    // Have to find a way to compute
    // sphere vertices and scale them
    // along a single axis.
    GLuint vao;
    glEnable(GL_DEPTH_TEST);
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint sphere; 
    glGenBuffers(1, &sphere); 
    glBindBuffer(GL_ARRAY_BUFFER, sphere);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);  

    // Actual computation loops which 
    // transform objects into display space.
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindBuffer(GL_ARRAY_BUFFER, sphere);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glUseProgram(program);

        glm::vec3 lightPosition = glm::vec3(0.0f, 5.0f, 0.0f);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        model = glm::scale(model, glm::vec3(2.0f, 1.0f, 1.0f)); 

        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -10.0f));

        glm::mat4 projection;
        projection = glm::perspective(glm::radians(45.0f), 16.0f / 10.0f, 0.1f, 100.0f);

        unsigned int modelLocation = glGetUniformLocation(program, "model");
        unsigned int viewLocation = glGetUniformLocation(program, "view");
        unsigned int projectionLocation = glGetUniformLocation(program, "projection");
        unsigned int lightLocation = glGetUniformLocation(program, "lightPosition");

        glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));
        glUniform3fv(lightLocation, 1, glm::value_ptr(lightPosition));

        glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size() / 6);

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}