
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


#include "glhimmel-computed/DubeCloudLayerGeode.h"

#include <glhimmel-computed/Earth.h>
#include "glhimmel-computed/HighCloudLayerGeode.h"

#include "glhimmel-computed/ComputedHimmel.h"
#include "glhimmel-base/Noise.h"
#include <glhimmel-base/MathMacros.h>
#include <glhimmel-base/Timef.h>
#include <globjects/Shader.h>
#include <glbinding/gl/gl.h>
using namespace gl;


namespace glHimmel
{

DubeCloudLayerGeode::DubeCloudLayerGeode(const int texSize)
:   m_noiseSize(texSize)
,   m_coverage(0.2f)
,   m_sharpness(0.3f)
,   m_change(defaultChange())
,   m_wind(glm::vec2(0.f, 0.f))
,   m_altitude(defaultAltitude())
,   m_scale(defaultScale())
,   m_thickness(3.f)
,   m_offset(-.5f)
,   m_bcolor(glm::vec3(1.f, 1.f, 1.f))
,   m_tcolor(glm::vec3(1.f, 1.f, 1.f))
{
    setupShader();
    setupTextures();
};


DubeCloudLayerGeode::~DubeCloudLayerGeode()
{
};


void DubeCloudLayerGeode::update(const ComputedHimmel &himmel)
{
    HighCloudLayerGeode::createPreRenderedNoise(m_noiseSize, m_preNoise
    , static_cast<float>(himmel.getTime()->getf())
    , m_coverage
    , m_sharpness
    , m_change
    , m_wind
    , m_noise);

    m_program->setUniform("altitude", m_altitude);
    m_program->setUniform("offset", m_offset);
    m_program->setUniform("thickness", m_thickness);
    m_program->setUniform("scale", m_scale);
    m_program->setUniform("tcolor", m_tcolor);
    m_program->setUniform("bcolor", m_bcolor);

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

    m_program->setUniform("sunr", himmel.getSunRefractedPosition());

    m_program->setUniform("modelView", himmel.getView());
    m_program->setUniform("inverseProjection", glm::inverse(himmel.getProjection()));
}

void DubeCloudLayerGeode::setupShader()
{
    m_program = new globjects::Program;
    auto vertexShader = globjects::Shader::fromFile(GL_VERTEX_SHADER
        , "data/shader/dubeCloudLayer.vert");
    auto fragmentShader = globjects::Shader::fromFile(GL_FRAGMENT_SHADER
        , "data/shader/dubeCloudLayer.frag");
    m_program->attach(vertexShader);
    m_program->attach(fragmentShader);
    m_program->use();

    m_program->setUniform("clouds", 0);
}


void DubeCloudLayerGeode::setupTextures()
{
    // use not the same seed as hight cloud layer
    srand(unsigned int(time(NULL))+1000);

    // precompute tilable permutations
    // generate textures 128, 256, 512, 1024 with rank 8, 16, 32, 64

    // TODO: the use of m_noise as an array insead of a texture array was due to problems
    // with osg and not enough time and vigor to fix this.. :D

    m_noise[0] = HighCloudLayerGeode::createNoiseArray(1 << 6, 3, 4);
    m_noise[1] = HighCloudLayerGeode::createNoiseArray(1 << 7, 4, 4);
    m_noise[2] = HighCloudLayerGeode::createNoiseArray(1 << 8, 5, 4);
    m_noise[3] = HighCloudLayerGeode::createNoiseArray(1 << 8, 6, 4);
}


const float DubeCloudLayerGeode::setAltitude(const float altitude)
{
    m_altitude = altitude;
    return getAltitude();
}

const float DubeCloudLayerGeode::getAltitude() const
{
    return m_altitude;
}

const float DubeCloudLayerGeode::defaultAltitude()
{
    return 2.f;
}


const float DubeCloudLayerGeode::setSharpness(const float sharpness)
{
    m_sharpness = sharpness;
    return getSharpness();
}

const float DubeCloudLayerGeode::getSharpness() const
{
    return m_sharpness;
}


const float DubeCloudLayerGeode::setCoverage(const float coverage)
{
    m_coverage = coverage;
    return getCoverage();
}

const float DubeCloudLayerGeode::getCoverage() const
{
    return m_coverage;
}


const glm::vec2 DubeCloudLayerGeode::setScale(const glm::vec2 &scale)
{
    m_scale = scale;
    return getScale();
}

const glm::vec2 DubeCloudLayerGeode::getScale() const
{
    return m_scale;
}

const glm::vec2 DubeCloudLayerGeode::defaultScale()
{
    return glm::vec2(128.f, 128.f);
}


const float DubeCloudLayerGeode::setChange(const float change)
{
    m_change = change;
    return getChange();
}

const float DubeCloudLayerGeode::getChange() const
{
    return m_change;
}

const float DubeCloudLayerGeode::defaultChange()
{
    return 0.1f;
}


const glm::vec2 DubeCloudLayerGeode::setWind(const glm::vec2 &wind)
{
    m_wind = wind;
    return getWind();
}

const glm::vec2 DubeCloudLayerGeode::getWind() const
{
    return m_wind;
}


const glm::vec3 DubeCloudLayerGeode::setBottomColor(const glm::vec3 &color)
{
    m_bcolor = color;
    return getBottomColor();
}

const glm::vec3 DubeCloudLayerGeode::getBottomColor() const
{
    return m_bcolor;
}


const glm::vec3 DubeCloudLayerGeode::setTopColor(const glm::vec3 &color)
{
    m_tcolor = color;
    return getTopColor();
}

const glm::vec3 DubeCloudLayerGeode::getTopColor() const
{
   return m_tcolor;
}


const float DubeCloudLayerGeode::setThickness(const float thickness)
{
    m_thickness = thickness;
    return getThickness();
}

const float DubeCloudLayerGeode::getThickness() const
{
    return m_thickness;
}


const float DubeCloudLayerGeode::setOffset(const float offset)
{
    m_offset = offset;
    return getOffset();
}

const float DubeCloudLayerGeode::getOffset() const
{
    return m_offset;
}

void DubeCloudLayerGeode::draw() const
{
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    m_preNoise->bindActive(0);
    m_program->use();
    m_screenAlignedQuad.draw();

    glDepthFunc(GL_LESS);
    glDisable(GL_DEPTH_TEST);

    glDisable(GL_BLEND);
}


} // namespace glHimmel