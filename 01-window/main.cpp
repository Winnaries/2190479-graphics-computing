#include <GL/glew.h>
#include <GLFW/glfw3.h>

int main()
{
    if (!glfwInit())
    {
        return -1;
    }

    int width, height, zoomLevel = 0;
    int baseWidth = 240;
    int baseHeight = 320;
    bool canResize = true;
    bool equalAxis = false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(240, 320, "Nattawat Pornthisan", NULL, NULL);
    glfwMakeContextCurrent(window);
    glClearColor(0.000f, 0.415f, 0.654f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
        {
            equalAxis = true;
            glfwGetWindowSize(window, &width, NULL);
            glfwSetWindowSize(window, width, width);
            glfwSetWindowAspectRatio(window, 1, 1);
        }

        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
        {
            equalAxis = false;
            glfwGetWindowSize(window, &width, NULL);
            glfwSetWindowSize(window, width, width / 2 * 3);
            glfwSetWindowAspectRatio(window, 3, 4);
        }

        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS && canResize)
        {
            int width, height, resizedHeight;
            canResize = false;
            zoomLevel = (zoomLevel + 1) % 3;
            resizedHeight = equalAxis ? baseWidth : baseHeight;
            glfwSetWindowSize(window,
                              baseWidth * (zoomLevel + 1),
                              resizedHeight * (zoomLevel + 1));
        }

        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_RELEASE && !canResize)
        {
            canResize = true;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}