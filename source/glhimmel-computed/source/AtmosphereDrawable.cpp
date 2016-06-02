#include <glhimmel-computed/AtmosphereDrawable.h>

#include "glhimmel-computed/ComputedHimmel.h"
#include <glhimmel-base/ScreenAlignedTriangle.h>
#include <glhimmel-computed/AbstractAstronomy.h>
#include <glhimmel-computed/AtmospherePrecompute.h>

#include <assert.h>
#include <globjects/base/File.h>
#include <globjects/NamedString.h>
#include <globjects/Shader.h>
#include <globjects/Texture.h>


namespace glHimmel
{

AtmosphereDrawable::AtmosphereDrawable()
:   m_program(new globjects::Program)

,   m_transmittance(NULL)
,   m_irradiance(NULL)
,   m_inscatter(NULL)

,   m_sunScaleFactor(defaultSunScaleFactor())
,   m_exposure(defaultExposure())
,   m_lheurebleueColor(defaultLHeureBleueColor())
,   m_lheurebleueIntensity(defaultLHeureBleueIntensity())

{
    setupProgram();
    setupTextures();
}


AtmosphereDrawable::~AtmosphereDrawable()
{
}


void AtmosphereDrawable::update(const Himmel &himmel)
{
    auto sunScale = himmel.astro()->getAngularSunRadius() * m_sunScaleFactor;

    m_program->setUniform("sunScale", sunScale);
    m_program->setUniform("lheurebleueColor", m_lheurebleueColor);
    m_program->setUniform("lheurebleueIntensity", m_lheurebleueIntensity);
    m_program->setUniform("exposure", m_exposure);

    m_precompute.compute();
}

void AtmosphereDrawable::setupTextures()
{
    assert(m_precompute);

    m_program->setUniform("transmittanceSampler", 0);
    m_program->setUniform("irradianceSampler", 1);
    m_program->setUniform("inscatterSampler", 2);

    m_transmittance = m_precompute.getTransmittanceTexture();
    m_transmittance->bindActive(0);

    m_irradiance = m_precompute.getIrradianceTexture();
    m_irradiance->bindActive(1);

    m_inscatter = m_precompute.getInscatterTexture();
    m_irradiance->bindActive(2);
}

void AtmosphereDrawable::setupProgram()
{
    globjects::NamedString::create("/data/shader/bruneton.glsl", new globjects::File("data/shader/bruneton.glsl", false));
    globjects::NamedString::create("/data/shader/dither.glsl", new globjects::File("data/shader/dither.glsl", false));
    m_program = new globjects::Program;
    auto vertexShader = globjects::Shader::fromFile(GL_VERTEX_SHADER, "data/shader/computedHimmel.vert");
    auto fragmentShader = globjects::Shader::fromFile(GL_FRAGMENT_SHADER, "data/shader/computedHimmel.frag");
    m_program->attach(vertexShader);
    m_program->attach(fragmentShader);
    m_program->use();
}


void AtmosphereDrawable::setSunScaleFactor(const float scale)
{
    m_sunScaleFactor = scale;
}

float AtmosphereDrawable::getSunScaleFactor() const
{
    return m_sunScaleFactor;
}

float AtmosphereDrawable::defaultSunScaleFactor()
{
    return 2.f;
}


void AtmosphereDrawable::setExposure(const float exposure)
{
    m_exposure = exposure;
}

float AtmosphereDrawable::getExposure() const
{
    return m_exposure;
}

float AtmosphereDrawable::defaultExposure()
{
    return 0.22f;
}


void AtmosphereDrawable::setLHeureBleueColor(const glm::vec3 &color)
{
    m_lheurebleueColor = color;

}

glm::vec3 AtmosphereDrawable::getLHeureBleueColor() const
{
    return m_lheurebleueColor;
}

glm::vec3 AtmosphereDrawable::defaultLHeureBleueColor()
{
    return glm::vec3(0.08, 0.3, 1.0);
}


void AtmosphereDrawable::setLHeureBleueIntensity(const float intensity)
{
    m_lheurebleueIntensity = intensity;
}

float AtmosphereDrawable::getLHeureBleueIntensity() const
{
    return m_lheurebleueIntensity;
}

float AtmosphereDrawable::defaultLHeureBleueIntensity()
{
    return 0.5f;
}

void AtmosphereDrawable::setAverageGroundReflectance(const float reflectance)
{
    m_precompute.getModelConfig().avgGroundReflectance = reflectance;
    m_precompute.dirty();
}

void AtmosphereDrawable::setThicknessRayleigh(const float thickness)
{
    m_precompute.getModelConfig().HR = thickness;
    m_precompute.dirty();
}

void AtmosphereDrawable::setScatteringRayleigh(const glm::vec3 &coefficients)
{
    m_precompute.getModelConfig().betaR = coefficients;
    m_precompute.dirty();
}

void AtmosphereDrawable::setThicknessMie(const float thickness)
{
    m_precompute.getModelConfig().HM = thickness;
    m_precompute.dirty();
}

void AtmosphereDrawable::setScatteringMie(const float coefficient)
{
    m_precompute.getModelConfig().betaMSca = glm::vec3(1, 1, 1) * coefficient;
    m_precompute.getModelConfig().betaMEx = m_precompute.getModelConfig().betaMSca / 0.9f;
    m_precompute.dirty();
}

void AtmosphereDrawable::setPhaseG(const float g)
{
    m_precompute.getModelConfig().mieG = g;
    m_precompute.dirty();
}

void AtmosphereDrawable::draw()
{
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glEnable(GL_BLEND);

    m_program->use();
    m_screenAlignedTriangle.draw();

    glDepthFunc(GL_LESS);
    glDisable(GL_DEPTH_TEST);
    
    glDisable(GL_BLEND);
}

} // namespace glHimmel
