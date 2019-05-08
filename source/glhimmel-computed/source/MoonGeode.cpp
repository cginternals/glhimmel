
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

#include "glhimmel-computed/MoonGeode.h"
#include "glhimmel-computed/MoonGlareGeode.h"

#include "glhimmel-computed/ComputedHimmel.h"

#include "glhimmel-computed/AbstractAstronomy.h"
#include <glhimmel-computed/Earth.h>
#include <glhimmel-base/MathMacros.h>
#include <glhimmel-base/interpolate.h>

#include <assert.h>
#include <globjects/Shader.h>
#include <glbinding/gl/gl.h>
using namespace gl;

namespace glHimmel
{

// Transforms a quad to the moons position into the canopy. Then
// generates a circle with sphere normals (and normals from cube map)
// representing a perfect sphere in space.
// Applies lighting from sun - moon phase is always correct and no
// separate calculation is required. Correct Moon rotation is currently
// faked (face towards earth is incorrect due to missing librations etc).

MoonGeode::MoonGeode()
    : m_sunShine(defaultSunShineColor(), defaultSunShineIntensity())
,   m_scale(defaultScale())
,   m_earthShineScale(defaultEarthShineIntensity())
,   m_earthShineColor(defaultEarthShineColor())
{
    setupShader();
    setupTextures();
};


MoonGeode::~MoonGeode()
{
};


void MoonGeode::update(ComputedHimmel &himmel)
{
    const glm::vec3 sunv = himmel.astronomy()->getSunPosition(false);

    const glm::vec3 moonv = himmel.astronomy()->getMoonPosition(false);
    const glm::vec3 moonrv = himmel.astronomy()->getMoonPosition(true);

    // moons = apparent angular radius (not diameter!) or angular diameter in rad ???
    //const float moons = tan(himmel.astronomy()->getAngularMoonRadius() * m_scale);
    const float moons = himmel.astronomy()->getAngularMoonRadius() * m_scale;

    m_program->setUniform("moonCube", 0);
    m_program->setUniform("modelViewProjection", himmel.getProjection() * himmel.getView());
    m_program->setUniform("moonr", glm::vec4(moonrv, moons));
    m_program->setUniform("moon", glm::vec4(moonv, moons));

    // rgb coefficients for eclipse coloring
    m_program->setUniform("eclCoeffs", 1);

    m_program->setUniform("R", himmel.astronomy()->getMoonOrientation());
    m_program->setUniform("sun", himmel.getSunPosition());

    m_program->setUniform("earthShine", m_earthShineColor
        * himmel.astronomy()->getEarthShineIntensity() * m_earthShineScale);


    // TODO: starmap and planets  and stars also require / use this ... - find better place
    // issue 11..
    const float fov = himmel.getCameraFovHint();
    const float height = static_cast<float>(himmel.getViewSizeHeightHint());

    m_program->setUniform("q", static_cast<float>(tan(_rad(fov * 0.5)) / (height * 0.5)));


    // approximate umbra and penumbra size in moon radii

    float e0 = 0, e1 = 0, e2 = 0;

    float B = 0.f; // 0 hints that eclipse is not happening

    // This allows skipping of the 1d eclipse texture look-up
    // as well as all other eclipse related calcs.

    if (acos(glm::dot(sunv, moonv)) > _PI_2)
    {

        const float dm = himmel.astronomy()->getMoonDistance();
        const float ds  = himmel.astronomy()->getSunDistance();

        const float ids = 1.f / ds;

        // scale for the normalized earth-moon system
        e0 = himmel.astronomy()->getMoonRadius() / dm;
        e1 = static_cast<float>(3.6676 - (397.0001 * dm) * ids);
        e2 = static_cast<float>(3.6676 + (404.3354 * dm) * ids);

        // shortest, normalized distance between earth and moon-sun line
        const float Dm = glm::cross(moonv, sunv).length() / e0;

        if(Dm < (e2 + 1.f))
        {
            // transform the distance from 0 to e2 + 1 into [0,1] range for simpler brightness term
            const float t = Dm < (e1 + 1.f) ? Dm / (2.f + 2.f * e1) : 0.5f + (Dm - e1 - 1.f) / (2.f * (e2 - e1));

            // brightness scalar of the eclipse phenomena as function of the distance
            B = 1.0f + 29.f * (1.0f - smoothstepExt(t, 0.2f, 0.44f));
        }
    }

    // encode in uniform
    // e0, e1, e2, and brightness scalar
    m_program->setUniform("eclParams", glm::vec4(e0, e1, e2, B));

    m_program->setUniform("sunShine", glm::vec4(defaultSunShineColor(), defaultSunShineIntensity()));

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



    auto *moonGlare = himmel.moonGlare()->getProgram();
    moonGlare->setUniform("moonr", glm::vec4(moonrv, moons));
    moonGlare->setUniform("sunShine"
        , glm::vec4(defaultSunShineColor(), defaultSunShineIntensity()));
    moonGlare->setUniform("earthShine", m_earthShineColor
        * himmel.astronomy()->getEarthShineIntensity() * m_earthShineScale);
    moonGlare->setUniform("sun", himmel.getSunPosition());
}


void MoonGeode::setupShader()
{
    m_program = new globjects::Program;
    auto vertexShader = globjects::Shader::fromFile(GL_VERTEX_SHADER
        , "data/shader/moon.vert");
    auto fragmentShader = globjects::Shader::fromFile(GL_FRAGMENT_SHADER
        , "data/shader/moon.frag");
    m_program->attach(vertexShader);
    m_program->attach(fragmentShader);
    m_program->use();
}

void MoonGeode::setupTextures()
{
    setupMoonTextureCube();
    setupEclipseTexture();
}


void MoonGeode::setupMoonTextureCube()
{
    m_moonCube = globjects::Texture::createDefault(GL_TEXTURE_CUBE_MAP);

    m_moonCube->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_moonCube->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    m_moonCube->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    m_moonCube->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);// _MIPMAP_LINEAR);
    m_moonCube->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


void MoonGeode::setupEclipseTexture()
{
    // generate lunar eclipse 2d-texture

    const int sizeS = 128;

    std::vector<float> map(sizeS * 3);

    const glm::vec3 le0 = glm::vec3(1.0, 1.0, 1.0) * 0.900f;
    const glm::vec3 le1 = glm::vec3(1.0, 1.0, 1.0) * 0.088f;
    const glm::vec3 le2 = glm::vec3(0.4, 0.7, 1.0) * 0.023f;
    const glm::vec3 le3 = glm::vec3(0.3, 0.5, 1.0) * 0.040f;

    const float s_u = 0.05f;

    for(int s = 0; s < sizeS; ++s)
    {
        const float fs = static_cast<float>(s) / sizeS;
        const unsigned int i = s * 3;

                glm::vec3 l(1, 1, 1);

        // remove the penumbral soft shadow from the moons coloring
        l -= le0 * (1.0f - _clamp(0.0f, 1.0f, 2.f * fs - 1.f));
        // remove the umbral hard shadow from the moons coloring
        l -= le1 * (1.f - smoothstepExt(fs, 0.5f * (1.0f - s_u), 0.5f * (1.0f + s_u)));
        // add reddish darkening towards umbral center from atmosphere scattering, linear scaling within the umbral distance of e1
        l -= le2 * (fs < 0.5 ? _clamp(0.f, 1.f, - 2.0f * fs + 1.f) : 0);
        // account for blue scattered light visible at the outer edge of the umbral shadow
        l += le3 * (smoothstepExt(fs, 0.5f * (1.f - 4.f * s_u)
                    , 0.5f * (1.f + s_u))) * (1.f - smoothstepExt(fs, 0.5f, 1.0f));

        map[i + 0] = l[0] < 0 ? 0 : l[0];
        map[i + 1] = l[1] < 0 ? 0 : l[1];
        map[i + 2] = l[2] < 0 ? 0 : l[2];
    }

    //

    m_eclipse = globjects::Texture::createDefault(GL_TEXTURE_1D);
    m_eclipse->image1D(0, GL_RGB32F_ARB, sizeS, 0, GL_RGB, GL_FLOAT
        , reinterpret_cast<unsigned char*>(&map[0]));

    m_eclipse->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_eclipse->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    m_eclipse->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}



const float MoonGeode::setScale(const float scale)
{
    m_scale = scale;

    return getScale();
}

const float MoonGeode::getScale() const
{
    return m_scale;
}

const float MoonGeode::defaultScale()
{
    return 2.f;
}


const glm::vec3 MoonGeode::setSunShineColor(const glm::vec3 &color)
{
    m_sunShine[0] = color[0];
    m_sunShine[1] = color[1];
    m_sunShine[2] = color[2];

    return getSunShineColor();
}

const glm::vec3 MoonGeode::getSunShineColor() const
{
    return glm::vec3(m_sunShine[0], m_sunShine[1], m_sunShine[2]);
}

const glm::vec3 MoonGeode::defaultSunShineColor()
{
    return glm::vec3(0.923, 0.786, 0.636);
}


const float MoonGeode::setSunShineIntensity(const float intensity)
{
    m_sunShine[3] = intensity;

    return getSunShineIntensity();
}

const float MoonGeode::getSunShineIntensity() const
{
    return m_sunShine[3];
}

const float MoonGeode::defaultSunShineIntensity()
{
    return 56.f;
}


const glm::vec3 MoonGeode::setEarthShineColor(const glm::vec3 &color)
{
    m_earthShineColor = color;
    return m_earthShineColor;
}

const glm::vec3 MoonGeode::getEarthShineColor() const
{
    return m_earthShineColor;
}

const glm::vec3 MoonGeode::defaultEarthShineColor()
{
    //return osg::Vec3f(0.92, 0.96, 1.00);
    return glm::vec3(0.88, 0.96, 1.00);
}


const float MoonGeode::setEarthShineIntensity(const float intensity)
{
    m_earthShineScale = intensity;
    return m_earthShineScale;
}

const float MoonGeode::getEarthShineIntensity() const
{
    return m_earthShineScale;
}

const float MoonGeode::defaultEarthShineIntensity()
{
    return 1.0f;
}

globjects::ref_ptr<globjects::Texture> MoonGeode::getMoonTextureCube()
{
    return m_moonCube;
}

void MoonGeode::draw() const
{
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    m_moonCube->bindActive(0);
    m_eclipse->bindActive(1);
    m_program->use();
    m_screenAlignedQuad.draw();
}

} // namespace glHimmel