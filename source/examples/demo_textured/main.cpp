
#include <iostream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glbinding/Binding.h>

#include <glbinding/gl/gl.h>
#include <globjects/globjects.h>

using namespace gl;

void error(int errnum, const char * errmsg)
{
    std::cerr << errnum << ": " << errmsg << std::endl;
}

void key_callback(GLFWwindow * window, int key, int /*scancode*/, int action, int /*mods*/)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
}


int main(int, char *[])
{
    if (!glfwInit())
        return 1;

    glfwSetErrorCallback(error);

    glfwDefaultWindowHints();

#ifdef SYSTEM_DARWIN
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

    GLFWwindow * window = glfwCreateWindow(640, 480, "Texture Demo", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, key_callback);

    glfwMakeContextCurrent(window);

    glbinding::Binding::initialize(false); // only resolve functions that are actually used (lazy)
    globjects::init();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
