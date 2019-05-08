
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


#include "glhimmel-computed/HighCloudLayerGeode.h"
#include "glhimmel-computed/ComputedHimmel.h"
#include <glhimmel-computed/Earth.h>
#include "glhimmel-base/Debugging.h"
#include "glhimmel-base/Noise.h"
#include <glhimmel-base/MathMacros.h>
#include <glhimmel-base/Timef.h>
#include <globjects/Framebuffer.h>
#include <globjects/Shader.h>
#include <glbinding/gl/gl.h>
using namespace gl;

namespace glHimmel
{

HighCloudLayerGeode::HighCloudLayerGeode(const int texSize)
:   m_noiseSize(texSize)
,   m_change(defaultChange())
,   m_wind(glm::vec2(0.f, 0.f))
,   m_color(glm::vec3(1.f, 1.f, 1.f))
,   m_altitude(defaultAltitude())
,   m_scale(defaultScale())
{
    setupShader();
    setupTextures();
};


HighCloudLayerGeode::~HighCloudLayerGeode()
{
};


void HighCloudLayerGeode::createPreRenderedNoise(
    const unsigned texSize
,   globjects::ref_ptr<globjects::Texture> &texture
,   float time
,   float coverage
,   float sharpness
,   float change
,   glm::vec2 wind
,   globjects::ref_ptr<globjects::Texture> *noise)
{
    // setup texture
    texture = globjects::Texture::createDefault();

    // GL_LUMINANCE caused invalid enum
    texture->image2D(0, GL_LUMINANCE16F_ARB, texSize, texSize
        , 0, GL_RED, GL_FLOAT, nullptr);

    texture->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    texture->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture->setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
    texture->setParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);

    // setup fbo
    static globjects::ref_ptr<globjects::Framebuffer> fbo = new globjects::Framebuffer();
    fbo->attachTexture(GL_COLOR_ATTACHMENT0, texture);
    fbo->bind(gl::GL_FRAMEBUFFER);
    glViewport(0, 0, texSize, texSize);

    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    // setup shaders

    static globjects::ref_ptr<globjects::Program> program;
    if (!program.get())
    {
        auto vertexShader = globjects::Shader::fromFile(GL_VERTEX_SHADER
            , "data/shader/highCloudLayerPreRenderNoise.vert");
        auto fragmentShader = globjects::Shader::fromFile(GL_FRAGMENT_SHADER
            , "data/shader/highCloudLayerPreRenderNoise.frag");

        program = new globjects::Program;
        program->attach(vertexShader);
        program->attach(fragmentShader);

        program->setUniform("noise0", 0);
        program->setUniform("noise1", 1);
        program->setUniform("noise2", 2);
        program->setUniform("noise3", 3);
    }

    program->setUniform("noiseSize", float(texSize));
    program->setUniform("time", time);
    program->setUniform("coverage", coverage);
    program->setUniform("sharpness", sharpness);
    program->setUniform("change", change);
    program->setUniform("wind", wind);

    noise[0]->bindActive(0);
    noise[1]->bindActive(1);
    noise[2]->bindActive(2);
    noise[3]->bindActive(3);

    program->use();

    static ScreenAlignedTriangle screenAlignedTriangle;
    screenAlignedTriangle.draw();

    fbo->unbind();
    logGLError("HighCloudLayerGeode::createPreRenderedNoise");
}


// TODO: move to noise class/area

std::vector<float> HighCloudLayerGeode::createNoiseSlice(
    const unsigned int texSize
,   const unsigned int octave)
{
    const unsigned int size2 = texSize * texSize;
    const float oneOverTexSize = 1.f / static_cast<float>(texSize);

    Noise n(1 << (octave + 2), _randf(0.f, 1.f), _randf(0.f, 1.f));

    std::vector<float> noise(size2);
    unsigned int o;

    for(unsigned int s = 0; s < texSize; ++s)
    for(unsigned int t = 0; t < texSize; ++t)
    {
        o = t * texSize + s;
        noise[o] = n.noise2(s * oneOverTexSize, t * oneOverTexSize, octave) * 0.5f + 0.5f;
    }

    return std::move(noise);
}


globjects::ref_ptr<globjects::Texture> HighCloudLayerGeode::createNoiseArray(
    const unsigned int texSize
,   const unsigned int octave
,   const unsigned int slices)
{
    auto texture = globjects::Texture::createDefault(GL_TEXTURE_3D);

    // doesn't work with GL_LUMINANCE instead of GL_RED
    texture->image3D(0, GL_LUMINANCE16F_ARB, texSize, texSize, slices
        , 0, GL_RED, GL_FLOAT, nullptr);

    for (unsigned int s = 0; s < slices; ++s)
    {
        const auto image = createNoiseSlice(texSize, octave);
        texture->subImage3D(0, 0, 0, s, texSize, texSize, 1
            , GL_RED, GL_FLOAT, &image[0]);
    }


    texture->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    texture->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture->setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
    texture->setParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
    texture->setParameter(GL_TEXTURE_WRAP_R, GL_REPEAT);

    return texture;
}


void HighCloudLayerGeode::update(const ComputedHimmel &himmel)
{
    createPreRenderedNoise(m_noiseSize, m_preNoise
    , static_cast<float>(himmel.getTime()->getf())
    , m_coverage
    , m_sharpness
    , m_change
    , m_wind
    , m_noise);

    m_program->setUniform("noise", 0);
    m_program->setUniform("color", m_color);
    m_program->setUniform("altitude", m_altitude);
    m_program->setUniform("scale", m_scale);

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


void HighCloudLayerGeode::setupShader()
{
    m_program = new globjects::Program;
    auto vertexShader = globjects::Shader::fromFile(GL_VERTEX_SHADER
        , "data/shader/highCloudLayer.vert");
    auto fragmentShader = globjects::Shader::fromFile(GL_FRAGMENT_SHADER
        , "data/shader/highCloudLayer.frag");
    m_program->attach(vertexShader);
    m_program->attach(fragmentShader);
    m_program->use();
}


void HighCloudLayerGeode::setupTextures()
{
    srand(unsigned int(time(NULL)));

    // precompute tilable permutations
    // generate textures 128, 256, 512, 1024 with rank 8, 16, 32, 64

    // TODO: the use of m_noise as an array insead of a texture array was due to problems
    // with osg and not enough time and vigor to fix this.. :D

    m_noise[0] = createNoiseArray(1 << 6, 3, 4);
    m_noise[1] = createNoiseArray(1 << 7, 4, 4);
    m_noise[2] = createNoiseArray(1 << 8, 5, 4);
    m_noise[3] = createNoiseArray(1 << 8, 6, 4);

    m_program->setUniform("preNoise", 0);
}


const float HighCloudLayerGeode::setAltitude(const float altitude)
{
    m_altitude = altitude;
    return getAltitude();
}

const float HighCloudLayerGeode::getAltitude() const
{
    return m_altitude;
}

const float HighCloudLayerGeode::defaultAltitude()
{
    return 8.f;
}


const float HighCloudLayerGeode::setSharpness(const float sharpness)
{
    m_sharpness = sharpness;
    return getSharpness();
}

const float HighCloudLayerGeode::getSharpness() const
{
    return m_sharpness;
}


const float HighCloudLayerGeode::setCoverage(const float coverage)
{
    m_coverage = coverage;
    return getCoverage();
}

const float HighCloudLayerGeode::getCoverage() const
{
    return m_coverage;
}


const glm::vec2 HighCloudLayerGeode::setScale(const glm::vec2 &scale)
{
    m_scale = scale;
    return getScale();
}

const glm::vec2 HighCloudLayerGeode::getScale() const
{
    return m_scale;
}

const glm::vec2 HighCloudLayerGeode::defaultScale()
{
    return glm::vec2(32.f, 32.f);
}


const float HighCloudLayerGeode::setChange(const float change)
{
    m_change = change;
    return getChange();
}

const float HighCloudLayerGeode::getChange() const
{
    return m_change;
}

const float HighCloudLayerGeode::defaultChange()
{
    return 0.1f;
}


const glm::vec2 HighCloudLayerGeode::setWind(const glm::vec2 &wind)
{
    m_wind = wind;
    return getWind();
}

const glm::vec2 HighCloudLayerGeode::getWind() const
{
    return m_wind;
}


const glm::vec3 HighCloudLayerGeode::setColor(const glm::vec3 &color)
{
    m_color = color;
    return getColor();
}

const glm::vec3 HighCloudLayerGeode::getColor() const
{
    return m_color;
}

void HighCloudLayerGeode::draw() const
{
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_BLEND);

    m_preNoise->bindActive(0);
    m_program->use();
    m_screenAlignedTriangle.draw();

    glDepthFunc(GL_LESS);
    glDisable(GL_DEPTH_TEST);

    glDisable(GL_BLEND);
}


} // namespace osgHimmel