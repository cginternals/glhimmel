
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

#include "glhimmel-computed/MoonGlareGeode.h"

#include "glhimmel-computed/ComputedHimmel.h"
#include "glhimmel-computed/AbstractAstronomy.h"
#include <glhimmel-base/MathMacros.h>
#include <globjects/Shader.h>
#include <glbinding/gl/gl.h>
using namespace gl;

namespace glHimmel
{

MoonGlareGeode::MoonGlareGeode()
:   m_scale(defaultScale())
,   m_intensity(defaultIntensity())
{
    setupShader();
};


MoonGlareGeode::~MoonGlareGeode()
{
};


void MoonGlareGeode::update(const ComputedHimmel &himmel)
{
    const glm::vec3 moonv = himmel.astronomy()->getMoonPosition(false);
    const glm::vec3 sunv = himmel.astronomy()->getSunPosition(false);

    m_program->setUniform("modelViewProjection"
        , himmel.getProjection() * himmel.getView());
    m_program->setUniform("intensity", m_intensity);
    m_program->setUniform("scale", m_scale);
    m_program->setUniform("phase", static_cast<float>(acos(glm::dot(moonv, sunv))));
}


void MoonGlareGeode::setupShader()
{
    m_program = new globjects::Program;
    auto vertexShader = globjects::Shader::fromFile(GL_VERTEX_SHADER
        , "data/shader/moonGlare.vert");
    auto fragmentShader = globjects::Shader::fromFile(GL_FRAGMENT_SHADER
        , "data/shader/moonGlare.frag");
    m_program->attach(vertexShader);
    m_program->attach(fragmentShader);
    m_program->use();
}


const float MoonGlareGeode::setScale(const float scale)
{
    m_scale = scale;
    return getScale();
}

const float MoonGlareGeode::getScale() const
{
    return m_scale;
}

const float MoonGlareGeode::defaultScale()
{
    return 128.f;
}


const float MoonGlareGeode::setIntensity(const float intensity)
{
    m_intensity = intensity;
    return getIntensity();
}

const float MoonGlareGeode::getIntensity() const
{
    return m_intensity;
}

const float MoonGlareGeode::defaultIntensity()
{
    return 0.5f;
}

globjects::Program *MoonGlareGeode::getProgram()
{
    return m_program.get();
}

void MoonGlareGeode::draw() const
{
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    glEnable(GL_BLEND);

    m_program->use();
    m_screenAlignedQuad.draw();

    glDepthFunc(GL_LESS);
    glDisable(GL_DEPTH_TEST);

    glDisable(GL_BLEND);
}

} // namespace glHimmel