
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

#include <texturebased/PolarMappedHimmel.h>
#include <texturebased/SphereMappedHimmel.h>
#include <texturebased/ParaboloidMappedHimmel.h>
#include <texturebased/HorizonBand.h>

using namespace gl;
using namespace glHimmel;

glm::mat4 g_projection;
std::unique_ptr<AbstractHimmel> g_himmel;
std::shared_ptr<TimeF> g_time;


float g_horizontalAngle;
float g_verticalAngle;

void setView()
{
    auto view = glm::lookAt(
        glm::vec3(0.0), 
        glm::vec3(
            std::cos(g_horizontalAngle),
            std::sin(g_horizontalAngle),
            std::sin(g_verticalAngle)), 
        glm::vec3(
            -std::cos(g_horizontalAngle),
            -std::sin(g_horizontalAngle),
            std::sin(glm::pi<float>() / 2 - g_verticalAngle))
    );
    g_himmel->setView(view);
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
    auto himmel = std::unique_ptr<PolarMappedHimmel>(new PolarMappedHimmel(PolarMappedHimmel::MappingMode::Half, true));


    himmel->hBand()->setBottomColor(glm::vec4(0.48f, 0.46f, 0.42f, 1.00f));
    himmel->hBand()->setColor(glm::vec4(0.70f, 0.65f, 0.6f, 1.00f));
    himmel->hBand()->setScale(0.1f);

    himmel->setTransitionDuration(0.1f);

    //himmel->setSecondsPerRAZ(30.f);
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

std::unique_ptr<AbstractHimmel> createSphereMappedDemo()
{
    auto himmel = std::unique_ptr<SphereMappedHimmel>(new SphereMappedHimmel());

    himmel->setTransitionDuration(0.1f);

    //himmel->setSecondsPerRAZ(30.f);
    himmel->setRazDirection(AbstractMappedHimmel::RazDirection::NorthWestSouthEast);

    auto image1 = rawFromFile("data/resources/sphere_gen_0.4096.4096.rgba.ub.raw");
    auto image2 = rawFromFile("data/resources/sphere_gen_2.4096.4096.rgba.ub.raw");

    himmel->getOrCreateTexture2D(0)->image2D(0, GL_RGBA32F, 4096, 4096, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1.data());
    himmel->getOrCreateTexture2D(1)->image2D(0, GL_RGBA32F, 4096, 4096, 0, GL_RGBA, GL_UNSIGNED_BYTE, image2.data());

    himmel->pushTextureUnit(himmel->getOrCreateTexture2D(0), 0.00f);
    himmel->pushTextureUnit(himmel->getOrCreateTexture2D(1), 0.50f);

    return std::move(himmel);
}

std::unique_ptr<AbstractHimmel> createParaboloidMappedDemo()
{
    auto himmel = std::unique_ptr<ParaboloidMappedHimmel>(new ParaboloidMappedHimmel(false, true));

    himmel->setTransitionDuration(0.05f);

    //himmel->setSecondsPerRAZ(30.f);
    himmel->setRazDirection(AbstractMappedHimmel::RazDirection::NorthWestSouthEast);

    auto image1 = rawFromFile("data/resources/paraboloid_gen_0.1024.1024.rgba.ub.raw");
    auto image2 = rawFromFile("data/resources/paraboloid_gen_1.1024.1024.rgba.ub.raw");
    auto image3 = rawFromFile("data/resources/paraboloid_gen_2.1024.1024.rgba.ub.raw");
    auto image4 = rawFromFile("data/resources/paraboloid_gen_3.1024.1024.rgba.ub.raw");
    auto image5 = rawFromFile("data/resources/paraboloid_pho_0.1024.1024.rgba.ub.raw");
    auto image6 = rawFromFile("data/resources/paraboloid_pho_1.1024.1024.rgba.ub.raw");
    auto image7 = rawFromFile("data/resources/paraboloid_pho_2.1024.1024.rgba.ub.raw");
    auto image8 = rawFromFile("data/resources/paraboloid_pho_3.1024.1024.rgba.ub.raw");

    himmel->getOrCreateTexture2D(0)->image2D(0, GL_RGBA32F, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1.data());
    himmel->getOrCreateTexture2D(1)->image2D(0, GL_RGBA32F, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, image2.data());
    himmel->getOrCreateTexture2D(2)->image2D(0, GL_RGBA32F, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, image3.data());
    himmel->getOrCreateTexture2D(3)->image2D(0, GL_RGBA32F, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, image4.data());
    himmel->getOrCreateTexture2D(4)->image2D(0, GL_RGBA32F, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, image5.data());
    himmel->getOrCreateTexture2D(5)->image2D(0, GL_RGBA32F, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, image6.data());
    himmel->getOrCreateTexture2D(6)->image2D(0, GL_RGBA32F, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, image7.data());
    himmel->getOrCreateTexture2D(7)->image2D(0, GL_RGBA32F, 1024, 1024, 0, GL_RGBA, GL_UNSIGNED_BYTE, image8.data());

    const float a = 1.0 / 8.0;

    himmel->pushTextureUnit(himmel->getOrCreateTexture2D(0), 0.0 * a);
    himmel->pushTextureUnit(himmel->getOrCreateTexture2D(1), 1.0 * a);
    himmel->pushTextureUnit(himmel->getOrCreateTexture2D(2), 2.0 * a);
    himmel->pushTextureUnit(himmel->getOrCreateTexture2D(3), 3.0 * a);
    himmel->pushTextureUnit(himmel->getOrCreateTexture2D(4), 4.0 * a);
    himmel->pushTextureUnit(himmel->getOrCreateTexture2D(5), 5.0 * a);
    himmel->pushTextureUnit(himmel->getOrCreateTexture2D(6), 6.0 * a);
    himmel->pushTextureUnit(himmel->getOrCreateTexture2D(7), 7.0 * a);

    return std::move(himmel);
}

void initializeHimmel()
{
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
    else if (key == '1' && action == GLFW_PRESS)
    {
        g_himmel = createPolarMappedDemo();
        initializeHimmel();
    }
    else if (key == '2' && action == GLFW_PRESS)
    {
        g_himmel = createSphereMappedDemo();
        initializeHimmel();
    }
    else if (key == '3' && action == GLFW_PRESS)
    {
        g_himmel = createParaboloidMappedDemo();
        initializeHimmel();
    }
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
    g_himmel = createPolarMappedDemo();
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
