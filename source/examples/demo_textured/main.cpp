
#include <iostream>
#include <memory>
#include <fstream>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glbinding/Binding.h>

#include <glbinding/gl/gl.h>
#include <globjects/globjects.h>
#include <texturebased/PolarMappedHimmel.h>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace gl;
using namespace glHimmel;

glm::mat4 g_projection;

void error(int errnum, const char * errmsg)
{
    std::cerr << errnum << ": " << errmsg << std::endl;
}

void key_callback(GLFWwindow * window, int key, int /*scancode*/, int action, int /*mods*/)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
}

// Read raw binary file into a char vector (probably the fastest way).
std::vector<char> rawFromFile(const std::string& filePath)
{
    auto stream = std::ifstream(filePath, std::ios::in | std::ios::binary | std::ios::ate);

    if (!stream)
    {
        std::cerr << "Reading from file '" << filePath << "' failed." << std::endl;
        exit(1);
        return std::vector<char>();
    }

    stream.seekg(0, std::ios::end);

    const auto size = stream.tellg();
    auto raw = std::vector<char>(size);

    stream.seekg(0, std::ios::beg);
    stream.read(raw.data(), size);

    return raw;
}

std::unique_ptr<AbstractHimmel> createPolarMappedDemo()
{
    auto himmel = std::unique_ptr<PolarMappedHimmel>(new PolarMappedHimmel(PolarMappedHimmel::MappingMode::Half));

    /*
    himmel->hBand()->setBottomColor(glm::vec4(0.48f, 0.46f, 0.42f, 1.00f));
    himmel->hBand()->setColor(glm::vec4(0.70f, 0.65f, 0.6f, 1.00f));
    himmel->hBand()->setScale(0.1f);
    */

    //himmel->assignTime(g_timef);
    himmel->setTransitionDuration(0.1f);

    himmel->setSecondsPerRAZ(300.f);
    himmel->setRazDirection(AbstractMappedHimmel::RazDirection::NorthWestSouthEast);

    auto image = rawFromFile("data/resources/polar_half_art_0.8192.2048.rgba.ub.raw");

    himmel->getOrCreateTexture2D(0)->image2D(0, GL_RGBA32F, 8192, 2048, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());

    /*himmel->getOrCreateTexture2D(1)->setImage(osgDB::readImageFile("resources/polar_half_art_1.jpg"));
    himmel->getOrCreateTexture2D(2)->setImage(osgDB::readImageFile("resources/polar_half_gen_2.jpg"));
    himmel->getOrCreateTexture2D(3)->setImage(osgDB::readImageFile("resources/polar_half_pho_1.jpg"));
    himmel->getOrCreateTexture2D(4)->setImage(osgDB::readImageFile("resources/polar_half_pho_7.jpg"));
    */
    himmel->pushTextureUnit(0, 0.0f);
    /*himmel->pushTextureUnit(1, 0.33f);
    himmel->pushTextureUnit(2, 0.66f);
    himmel->pushTextureUnit(3, 0.6f);
    himmel->pushTextureUnit(4, 0.8f);
    */
    return std::move(himmel);
 }

void onResize(GLFWwindow*, int width, int height)
{
    glViewport(0, 0, width, height);
    g_projection = glm::perspective(1.5, static_cast<double>(width) / height, 1.0, 2.0);
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

    glbinding::Binding::initialize(false); // only resolve functions that are actually used (lazy)
    globjects::init();
    onResize(window, 640, 480);

    glfwSetWindowSizeCallback(window, onResize);

    auto himmel = createPolarMappedDemo();
    himmel->initialize();
    //set projection

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        himmel->setProjection(g_projection);
        himmel->draw();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
