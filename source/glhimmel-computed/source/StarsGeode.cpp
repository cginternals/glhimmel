
// Copyright (c) 2011-2012, Daniel Müller <dm@g4t3.de>
// Computer Graphics Systems Group at the Hasso-Plattner-Institute, Germany
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//   * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
//   * Neither the name of the Computer Graphics Systems Group at the
//     Hasso-Plattner-Institute (HPI), Germany nor the names of its
//     contributors may be used to endorse or promote products derived from
//     this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#include "glhimmel-computed/starsgeode.h"

//#include "coords.h"
//#include "earth.h"
//#include "stars.h"
//#include "strutils.h"

#include "glhimmel-computed/AbstractAstronomy.h"
#include <glhimmel-computed/Earth.h>
#include <glhimmel-computed/brightstars.h>
#include "glhimmel-computed/ComputedHimmel.h"
#include <glhimmel-base/coordinates.h>
#include <glhimmel-base/MathMacros.h>
#include "glhimmel-base/RandomMapGenerator.h"
#include <globjects/Buffer.h>
#include <globjects/Shader.h>
#include <globjects/VertexAttributeBinding.h>
#include <glbinding/gl/gl.h>
using namespace gl;


namespace glHimmel
{

StarsGeode::StarsGeode(const char* brightStarsFilePath)
    : m_colorAndRatio(defaultColor(), defaultColorRatio())
    , m_glareIntensity(1.f)
,   m_glareScale(defaultGlareScale())
,   m_apparentMagnitude(defaultApparentMagnitude())
,   m_scattering(defaultScattering())
,   m_scintillations(defaultScintillation())
,   m_scale(1.f)
{
    setupStars(brightStarsFilePath);
    setupShader();
    setupTextures();
};


StarsGeode::~StarsGeode()
{
};


void StarsGeode::update(const ComputedHimmel &himmel)
{
    // TODO: starmap and planets also require this ... - find better place
    const float fov = himmel.getCameraFovHint();
    const float height = static_cast<float>(himmel.getViewSizeHeightHint());

    m_program->setUniform("noise1", 0);
    m_program->setUniform("q", static_cast<float>(4.0 * tan(_rad(fov * 0.5)) / height));
    m_program->setUniform("R", himmel.astronomy()->getEquToHorTransform());
    m_program->setUniform("color", m_colorAndRatio);
    m_program->setUniform("glareIntensity", m_glareIntensity);
    m_program->setUniform("glareScale", m_glareScale);
    m_program->setUniform("apparentMagnitude", m_apparentMagnitude);
    m_program->setUniform("scintillations", m_scintillations);
    m_program->setUniform("scattering", m_scattering);
    m_program->setUniform("scale", m_scale);
    m_program->setUniform("sun", himmel.getSunPosition());
    m_program->setUniform("modelViewProjection"
        , himmel.getProjection() * himmel.getView());

    // 0: altitude in km
    // 1: apparent angular radius (not diameter!)
    // 2: radius up to "end of atm"
    // 3: seed (for randomness of stuff)

    // Clamp altitude into non uniform atmosphere. (min alt is 1m)
    glm::vec4 glm(_clamp(0.001f, Earth::atmosphereThicknessNonUniform(), himmel.getAltitude())
        , Earth::meanRadius()
        , Earth::meanRadius() + Earth::atmosphereThicknessNonUniform()
        , himmel.getSeed());
    m_program->setUniform("cmn", glm);
}


void StarsGeode::setupStars(
    const char *brightStarsFilePath)
{
    BrightStars bs(brightStarsFilePath);
    const BrightStars::s_BrightStar *stars = bs.stars();

    m_cAry.resize(bs.numStars());
    m_vAry.resize(bs.numStars());

    for (unsigned int i = 0; i < bs.numStars(); ++i)
    {
        EquatorialCoords<float> equ;
        equ.right_ascension = static_cast<float>(_rightascd(stars[i].RA, 0, 0));
        equ.declination = stars[i].DE;

        glm::vec3 vec = equ.toEuclidean();
        m_vAry[i] = glm::vec4(vec.x, vec.y, vec.z, i);

        m_cAry[i] = glm::vec4(stars[i].sRGB_R, stars[i].sRGB_G, stars[i].sRGB_B, stars[i].Vmag + 0.4);
        // the 0.4 accounts for magnitude decrease due to the earth's atmosphere
    }

    m_vao = new globjects::VertexArray();

    m_colorBuffer = new globjects::Buffer();
    m_colorBuffer->setData(m_cAry, GL_STATIC_DRAW);
    m_vertexBuffer = new globjects::Buffer();
    m_vertexBuffer->setData(m_vAry, GL_STATIC_DRAW);

    auto colorBinding = m_vao->binding(0);
    colorBinding->setAttribute(0);
    colorBinding->setBuffer(m_colorBuffer, 0, sizeof(glm::vec4));
    colorBinding->setFormat(4, GL_FLOAT, GL_FALSE, 0);
    // For static attributes for each vertex
    //g->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
    colorBinding->setValue(1.0f);

    auto vertexBinding = m_vao->binding(1);
    vertexBinding->setAttribute(1);
    vertexBinding->setBuffer(m_vertexBuffer, 0, sizeof(glm::vec4));
    vertexBinding->setFormat(4, GL_FLOAT, GL_FALSE, 0);

    m_vao->enable(0);
    m_vao->enable(1);

    // If things go wrong, fall back to big point rendering without geometry shader.
    //g->getOrCreateStateSet()->setAttribute(new osg::Point(2.0 * sqrt(2.0)));
}


void StarsGeode::setupShader()
{
    m_program = new globjects::Program;
    auto vertexShader = globjects::Shader::fromFile(GL_VERTEX_SHADER
        , "data/shader/stars.vert");
    auto fragmentShader = globjects::Shader::fromFile(GL_FRAGMENT_SHADER
        , "data/shader/stars.frag");
    auto geometryShader = globjects::Shader::fromFile(GL_GEOMETRY_SHADER
        , "data/shader/stars.geo");
    m_program->attach(vertexShader);
    m_program->attach(fragmentShader);
    m_program->attach(geometryShader);
    m_program->use();
}


void StarsGeode::setupTextures()
{
    const int noiseN = 256;

    std::vector<unsigned char> noiseMap(noiseN);
    RandomMapGenerator::generate1(noiseN, 1, &noiseMap[0]);

    // GL_LUMINANCE8 doesn't work as internal format
    m_noise = globjects::Texture::createDefault(GL_TEXTURE_1D);
    m_noise->image1D(0, GL_R8, noiseN, 0, GL_RED, GL_UNSIGNED_BYTE
        , reinterpret_cast<unsigned char*>(&noiseMap[0]));

    m_noise->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_noise->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    m_noise->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}


const float StarsGeode::setGlareIntensity(const float intensity)
{
    m_glareIntensity = intensity;
    return getGlareIntensity();
}

const float StarsGeode::getGlareIntensity() const
{
    return m_glareIntensity;
}


const float StarsGeode::setGlareScale(const float scale)
{
    m_glareScale = scale;
    return getGlareScale();
}

const float StarsGeode::getGlareScale() const
{
    return m_glareScale;
}

const float StarsGeode::defaultGlareScale()
{
    return 1.0f;
}


const float StarsGeode::setScintillation(const float scintillation)
{
    m_scintillations = scintillation;
    return getScintillation();
}

const float StarsGeode::getScintillation() const
{
    return m_scintillations;
}

const float StarsGeode::defaultScintillation()
{
    return 20.0f;
}


const float StarsGeode::setScattering(const float scattering)
{
    m_scattering = scattering;
    return getScattering();
}

const float StarsGeode::getScattering() const
{
    return m_scattering;
}

const float StarsGeode::defaultScattering()
{
    return 4.0f;
}


const float StarsGeode::setApparentMagnitude(const float vMag)
{
    m_apparentMagnitude = vMag;
    return getApparentMagnitude();
}

const float StarsGeode::getApparentMagnitude() const
{
    return m_apparentMagnitude;
}

const float StarsGeode::defaultApparentMagnitude()
{
    return 7.0f;
}


const glm::vec3 StarsGeode::setColor(const glm::vec3 color)
{
    m_colorAndRatio[0] = color[0];
    m_colorAndRatio[1] = color[1];
    m_colorAndRatio[2] = color[2];

    return getColor();
}

const glm::vec3 StarsGeode::getColor() const
{
    return glm::vec3(m_colorAndRatio[0], m_colorAndRatio[1], m_colorAndRatio[2]);
}

const glm::vec3 StarsGeode::defaultColor()
{
    return glm::vec3(0.66, 0.78, 1.00);
}


const float StarsGeode::setColorRatio(const float ratio)
{
    m_colorAndRatio[3] = ratio;
    return getColorRatio();
}

const float StarsGeode::getColorRatio() const
{
    return m_colorAndRatio[3];
}

const float StarsGeode::defaultColorRatio()
{
    return 0.66f;
}


const float StarsGeode::setScale(const float scale)
{
    m_scale = scale;
    return getScale();
}

const float StarsGeode::getScale() const
{
    return m_scale;
}

void StarsGeode::draw() const
{
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glEnable(GL_BLEND);

    m_noise->bindActive(0);
    m_program->use();
    m_vao->drawArrays(GL_POINTS, 0, static_cast<gl::GLsizei>(m_vAry.size()));

    glDepthFunc(GL_LESS);
    glDisable(GL_DEPTH_TEST);

    glDisable(GL_BLEND);
}

} // namespace glHimmel