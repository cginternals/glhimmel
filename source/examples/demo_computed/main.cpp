
#include <iostream>
#include <memory>
#include <fstream>
#include <algorithm>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glbinding/Binding.h>
#include <glbinding/gl/gl.h>
#include <globjects/globjects.h>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <glhimmel-base/Timef.h>
#include <glhimmel-computed/ComputedHimmel.h>
#include <glhimmel-computed/Astronomy.h>
#include <glhimmel-computed/AtmosphereDrawable.h>

using namespace gl;
using namespace glHimmel;

glm::mat4 g_projection;
std::unique_ptr<ComputedHimmel> g_himmel;
std::shared_ptr<TimeF> g_time;


float g_horizontalAngle;
float g_verticalAngle;

void setView()
{
    auto view = glm::lookAt(
        glm::vec3(0.0),
        glm::vec3(
            std::cos(g_horizontalAngle) * std::cos(g_verticalAngle),
            std::sin(g_horizontalAngle) * std::cos(g_verticalAngle),
            std::sin(g_verticalAngle)), 
        glm::vec3(
            -std::cos(g_horizontalAngle) * std::cos(glm::pi<float>() / 2 - g_verticalAngle),
            -std::sin(g_horizontalAngle) * std::cos(glm::pi<float>() / 2 - g_verticalAngle),
            std::sin(glm::pi<float>() / 2 - g_verticalAngle))
    );
    g_himmel->setView(view);
}


void initializeHimmel()
{
    auto atmosphere = new AtmosphereDrawable();
    auto astronomy = new Astronomy();
    g_himmel = std::unique_ptr<ComputedHimmel>(new ComputedHimmel(atmosphere, astronomy));
    g_himmel->initialize();
    g_himmel->assignTime(g_time);
    setView();
}

void error(int errnum, const char * errmsg)
{
    std::cerr << errnum << ": " << errmsg << std::endl;
}

void key_callback(GLFWwindow * window, int key, int /*scancode*/, int action, int /*mods*/)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);

    else if (key == GLFW_KEY_DOWN && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        g_verticalAngle -= 0.05;
        g_verticalAngle = std::max(g_verticalAngle, 0.f);
    }
    else if (key == GLFW_KEY_UP && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        g_verticalAngle += 0.05;
        g_verticalAngle = std::min(g_verticalAngle, glm::pi<float>() / 2.f);
    }
    else if (key == GLFW_KEY_RIGHT && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        g_horizontalAngle -= 0.05;
        g_horizontalAngle = std::fmod(g_horizontalAngle, glm::pi<float>() * 2.f);
    }
    else if (key == GLFW_KEY_LEFT && (action == GLFW_REPEAT || action == GLFW_PRESS))
    {
        g_horizontalAngle += 0.05;
        g_horizontalAngle = std::fmod(g_horizontalAngle, glm::pi<float>() * 2.f);
    }
    setView();
}

void onResize(GLFWwindow*, int width, int height)
{
    glViewport(0, 0, width, height);
    g_projection = glm::perspective(1.0, static_cast<double>(width) / height, 1.0, 2.0);
}

int main(int, char *[])
{
    if (!glfwInit())
        return 1;

    glfwSetErrorCallback(error);

    glfwDefaultWindowHints();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

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

    glbinding::Binding::initialize(false);
    globjects::init();

    onResize(window, 640, 480);
    glfwSetWindowSizeCallback(window, onResize);

    g_time = std::make_shared<TimeF>();
    g_time->setSecondsPerCycle(60);
    initializeHimmel();
    g_time->start();


    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        g_time->update();
        g_himmel->setProjection(g_projection);

        g_himmel->draw();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
