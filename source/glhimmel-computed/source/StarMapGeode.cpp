
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

#include "glhimmel-computed/StarMapGeode.h"
#include "glhimmel-computed/ComputedHimmel.h"

#include "glhimmel-computed/AbstractAstronomy.h"
#include <glhimmel-computed/Earth.h>
#include <glhimmel-base/MathMacros.h>

#include <assert.h>
#include <globjects/Shader.h>
#include <glbinding/gl/gl.h>
using namespace gl;


namespace glHimmel
{

StarMapGeode::StarMapGeode()
	: m_color(glm::vec4(defaultColor(), defaultColorRatio()))
	, m_deltaM(1.f)
	, m_scattering(defaultScattering())
{
    setupShader();
    setApparentMagnitude(defaultApparentMagnitude()); // This calls updateScaledB.
    setupTextures();
};


StarMapGeode::~StarMapGeode()
{
};


void StarMapGeode::update(const ComputedHimmel &himmel)
{
    // TODO: starmap and planets also require this ... - find better place
    const float fov = himmel.getCameraFovHint();
    const float height = static_cast<float>(himmel.getViewSizeHeightHint());

    m_program->setUniform("q", static_cast<float>(sqrt(2.0) * 2.0 * tan(_rad(fov * 0.5)) / height));
    m_program->setUniform("R", himmel.astronomy()->getEquToHorTransform());
    m_program->setUniform("sun", himmel.getSunPosition());

    m_program->setUniform("starmapCube", 0);
    m_program->setUniform("color", m_color);
    m_program->setUniform("deltaM", m_deltaM);
    m_program->setUniform("scattering", m_scattering);

    m_program->setUniform("modelView", himmel.getView());
    m_program->setUniform("inverseProjection", glm::inverse(himmel.getProjection()));

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


void StarMapGeode::setupShader()
{
    m_program = new globjects::Program;
    auto vertexShader = globjects::Shader::fromFile(GL_VERTEX_SHADER
        , "data/shader/starMap.vert");
    auto fragmentShader = globjects::Shader::fromFile(GL_FRAGMENT_SHADER
        , "data/shader/starMap.frag");
    m_program->attach(vertexShader);
    m_program->attach(fragmentShader);
    m_program->use();
}


void StarMapGeode::setupTextures()
{
    m_starMapCube = globjects::Texture::createDefault(GL_TEXTURE_CUBE_MAP);

    m_starMapCube->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    m_starMapCube->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    m_starMapCube->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    m_starMapCube->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);// _MIPMAP_LINEAR);
    m_starMapCube->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}


void StarMapGeode::updateScaledB()
{
    // Precompute brightness based on logarithmic scale.
    // (Similar to starsgeode vertex shader.)

    m_deltaM = pow(2.512f, m_apparentMagnitude - static_cast<float>(Earth::apparentMagnitudeLimit()));
}


const glm::vec3 StarMapGeode::setColor(const glm::vec3 &color)
{
    m_color[0] = color[0];
    m_color[1] = color[1];
    m_color[2] = color[2];

    return getColor();
}

const glm::vec3 StarMapGeode::getColor() const
{
    return glm::vec3(m_color[0], m_color[1], m_color[2]);
}

const glm::vec3 StarMapGeode::defaultColor()
{
    return glm::vec3(0.66, 0.78, 1.00);
}


const float StarMapGeode::setColorRatio(const float ratio)
{
    m_color[3] = ratio;

    return getColorRatio();
}

const float StarMapGeode::getColorRatio() const
{
    return m_color[3];
}

const float StarMapGeode::defaultColorRatio()
{
    return 0.33f;
}


const float StarMapGeode::setApparentMagnitude(const float vMag)
{
    m_apparentMagnitude = vMag;
    updateScaledB();

    return getApparentMagnitude();
}

const float StarMapGeode::getApparentMagnitude() const
{
    return m_apparentMagnitude;
}

const float StarMapGeode::defaultApparentMagnitude()
{
    return 6.0f;
}


const float StarMapGeode::setScattering(const float scattering)
{
    m_scattering = scattering;
    return getScattering();
}

const float StarMapGeode::getScattering() const
{
    return m_scattering;
}

const float StarMapGeode::defaultScattering()
{
    return 2.0f;
}

globjects::ref_ptr<globjects::Texture> StarMapGeode::getStarMapCube()
{
	return m_starMapCube;
}

void StarMapGeode::draw() const
{
	//glDepthFunc(GL_LEQUAL);
	//glEnable(GL_DEPTH_TEST);
	glDisable(GL_DEPTH_TEST);

	m_starMapCube->bindActive(0);
	m_program->use();
	m_screenAlignedTriangle.draw();

	//glDepthFunc(GL_LESS);
	//glDisable(GL_DEPTH_TEST);
}

} // namespace glHimmel