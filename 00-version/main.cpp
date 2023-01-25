#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main (int argc, char** argv)
{
    if (!glfwInit()) 
    {
        std::cerr << "ERROR: Could not start GLFW3" << std::endl; 
        return 1;   
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2); 
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
    
    GLFWwindow* window = glfwCreateWindow(640, 480, "OpenGL Initialization Example", NULL, NULL); 
    
    if (!window) 
    {
        std::cerr << "ERROR: could not open window with GLFW3" << std::endl; 
        glfwTerminate(); 
        return 1;     
    }

    glfwMakeContextCurrent(window); 
    glewExperimental = GL_TRUE; 
    glewInit();

    const GLubyte *renderer = glGetString(GL_RENDERER);
    const GLubyte *version = glGetString(GL_VERSION); 
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported " << version << std::endl;

    glfwTerminate();
    return 0;
}