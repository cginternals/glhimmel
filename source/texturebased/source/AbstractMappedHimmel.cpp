
#include <texturebased/AbstractMappedHimmel.h>

#include <cassert>
#include <limits>
#include <iostream>

#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glbinding/gl/types.h>
#include <globjects/Shader.h>
#include <globjects/Texture.h>
#include <globjects/NamedString.h>
#include <globjects/base/File.h>

#include <texturebased/ScreenAlignedQuad.h>
#include <texturebased/HorizonBand.h>
#include <texturebased/coordinates.h>

namespace
{
    const gl::GLint BACK_TEXTURE_INDEX(0);
    const gl::GLint SRC_TEXTURE_INDEX(1);
}

namespace glHimmel
{

AbstractMappedHimmel::AbstractMappedHimmel(const bool hBand, const bool fakeSun)
    : AbstractHimmel()
    , m_hquad()

    , m_program(nullptr)

    , m_changer()
    , m_activeBackUnit(nullptr)
    , m_activeSrcUnit(nullptr)
    , m_srcAlpha(0.0f)

    , m_razTimef()
    , m_razTransform()
    , m_razDirection(RazDirection::NorthWestSouthEast)
    , m_sunCoordinates(glm::vec3(1.0, 0.0, 1.0))
    , m_sunCoeffs(glm::vec4(0.63, 0.58, 0.49, 1.0))
    , m_sunScale(1.0f)
    , m_fakeSun(fakeSun)
{
    if (hBand)
    {
        m_hBand = std::unique_ptr<HorizonBand>(new HorizonBand());
    }
}

AbstractMappedHimmel::~AbstractMappedHimmel()
{
}

void AbstractMappedHimmel::initialize()
{
    setupProgram();

    m_razTimef.start();
}

void AbstractMappedHimmel::update()
{
    AbstractHimmel::update();

    // Update rotation around zenith.

    const float razd(m_razDirection == RazDirection::NorthWestSouthEast ? 1.f : -1.f);

    m_razTransform = glm::rotate<float>(glm::mat4(), razd * m_razTimef.getf(true) * glm::pi<float>() * 2.f, glm::vec3(0.f, 0.f, 1.f));

    const float t = timef();

    if (m_fakeSun)
    {
        EquatorialCoords<long double> equ;
        // TODO: fix this
        equ.declination = glm::degrees(t * glm::pi<float>() / 2.f);
        equ.right_ascension = glm::degrees((2.f * t - 1) * glm::pi<float>());

        m_sunCoordinates = equ.toEuclidean();
    }

    // Update two texture status for arbitrary blending (e.g. normal).

    // update texture change
    m_srcAlpha = m_changer.getSrcAlpha(t);

    // Avoid unnecessary unit switches.

    const globjects::ref_ptr<globjects::Texture> backUnit(m_changer.getBack(t));
    if (backUnit != m_activeBackUnit)
    {
        backUnit->bindActive(BACK_TEXTURE_INDEX);
        m_activeBackUnit = backUnit;
    }

    const globjects::ref_ptr<globjects::Texture> srcUnit(m_changer.getSrc(t));
    if (srcUnit != m_activeSrcUnit)
    {
        srcUnit->bindActive(SRC_TEXTURE_INDEX);
        m_activeSrcUnit = srcUnit;
    }
    updateUniforms();
}

void AbstractMappedHimmel::setupProgram()
{
    globjects::NamedString::create("/data/shader/fakeSun.glsl", new globjects::File("data/shader/fakeSun.glsl", false));
    globjects::NamedString::create("/data/shader/hband.glsl", new globjects::File("data/shader/hband.glsl", false));
    m_program = new globjects::Program;
    auto vertexShader = globjects::Shader::fromFile(GL_VERTEX_SHADER, "data/shader/mappedHimmel.vert");
    auto fragmentShader = globjects::Shader::fromFile(GL_FRAGMENT_SHADER, fragmentShaderPath());
    m_program->attach(vertexShader);
    m_program->attach(fragmentShader);
    m_program->use();
    std::cout << m_program->infoLog() << std::endl;
}


void AbstractMappedHimmel::updateUniforms() const
{
    m_program->setUniform("srcAlpha", m_srcAlpha);
    m_program->setUniform("back", BACK_TEXTURE_INDEX);
    m_program->setUniform("src", SRC_TEXTURE_INDEX);
    m_program->setUniform("modelView", m_view * m_razTransform);
    m_program->setUniform("inverseProjection", glm::inverse(m_projection));

    m_program->setUniform("razInverse", glm::inverse(m_razTransform));

    m_program->setUniform("fakeSun", m_fakeSun);
    if (m_fakeSun)
    {
        m_program->setUniform<glm::vec3>("sunCoords", m_sunCoordinates);
        m_program->setUniform<glm::vec4>("sunCoeffs", m_sunCoeffs);
        m_program->setUniform<float>("sunScale", m_sunScale);
    }

    m_program->setUniform("hBand", m_hBand.get() != nullptr);
    if (m_hBand) {
        m_hBand->updateUniforms(m_program);
    }
}

void AbstractMappedHimmel::setTransitionDuration(const float duration)
{
    m_changer.setTransitionDuration(duration);
}

float AbstractMappedHimmel::getTransitionDuration() const
{
    return m_changer.getTransitionDuration();
}

void AbstractMappedHimmel::pushTextureUnit(
    const globjects::ref_ptr<globjects::Texture> textureUnit
    , const float time)
{
    m_changer.push(textureUnit, time);
}

void AbstractMappedHimmel::setSecondsPerRAZ(const float secondsPerRAZ)
{
    m_razTimef.setSecondsPerCycle(std::abs(secondsPerRAZ));
}

float AbstractMappedHimmel::getSecondsPerRAZ() const
{
    return m_razTimef.getSecondsPerCycle();
}

void AbstractMappedHimmel::setRazDirection(const RazDirection razDirection)
{
    m_razDirection = razDirection;
}

AbstractMappedHimmel::RazDirection AbstractMappedHimmel::getRazDirection() const
{
    return m_razDirection;
}

void AbstractMappedHimmel::setSunScale(const float scale)
{
    m_sunScale = scale;
}

float AbstractMappedHimmel::getSunScale() const
{
    return m_sunScale;
}

void AbstractMappedHimmel::setSunCoeffs(const glm::vec4 &coeffs)
{
    m_sunCoeffs = coeffs;
}

glm::vec4 AbstractMappedHimmel::getSunCoeffs() const
{
    return m_sunCoeffs;
}

HorizonBand* AbstractMappedHimmel::hBand() const
{
    return m_hBand.get();
}

glm::vec4 AbstractMappedHimmel::defaultSunCoeffs()
{
    return glm::vec4(0.63, 0.58, 0.49, 1.0);
}

void AbstractMappedHimmel::draw()
{
    AbstractHimmel::draw();

    m_program->use();
    m_hquad.draw();
}


globjects::ref_ptr<globjects::Texture> AbstractMappedHimmel::getOrCreateTexture2D(const unsigned int textureUnit)
{
    // Retrieve an existing texture.

    const auto existingTex2D = m_tex2DsById.find(textureUnit);
    if (existingTex2D != m_tex2DsById.end())
        return existingTex2D->second;

    // Create and configure new texture object.

    auto newTex2D = globjects::Texture::createDefault();

    newTex2D->setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);

    m_tex2DsById[textureUnit] = newTex2D;

    // Assign some textures if there are none.

    if (m_tex2DsById.size() == 1)
        newTex2D->bindActive(textureUnit);
    if (m_tex2DsById.size() == 2)
        newTex2D->bindActive(textureUnit);

    return newTex2D;
}

} // namespace glHimmel

