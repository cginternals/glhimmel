
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

#pragma once
#ifndef __STARSGEODE_H__
#define __STARSGEODE_H__

#include <glhimmel-computed/glhimmel-computed_api.h>
#include <glhimmel-base/ScreenAlignedTriangle.h>
#include <globjects/Program.h>
#include <globjects/Texture.h>
#include <globjects/VertexArray.h>
#include <globjects/Buffer.h>
#include <glm/vec4.hpp>


namespace glHimmel
{

class ComputedHimmel;


class GLHIMMEL_COMPUTED_API StarsGeode
{
public:

    StarsGeode(const char *brightStarsFilePath);
    virtual ~StarsGeode();

    void update(const ComputedHimmel &himmel);

    const float setApparentMagnitude(const float vMag);
    const float getApparentMagnitude() const;
    static const float defaultApparentMagnitude();

    const glm::vec3 setColor(const glm::vec3 color);
    const glm::vec3 getColor() const;
    static const glm::vec3 defaultColor();

    const float setColorRatio(const float ratio);
    const float getColorRatio() const;
    static const float defaultColorRatio();


    const float setGlareIntensity(const float intensity);
    const float getGlareIntensity() const;

    const float setGlareScale(const float scale);
    const float getGlareScale() const;
    static const float defaultGlareScale();

    const float setScintillation(const float scintillation);
    const float getScintillation() const;
    static const float defaultScintillation();

    const float setScattering(const float scattering);
    const float getScattering() const;
    static const float defaultScattering();

    const float setScale(const float scale);
    const float getScale() const;

    void draw() const;

protected:

    void setupTextures();
    void setupShader  ();

    void setupStars(
    const char *brightStarsFilePath);

protected:

    globjects::ref_ptr<globjects::VertexArray> m_vao;
    globjects::ref_ptr<globjects::Buffer> m_colorBuffer;
    globjects::ref_ptr<globjects::Buffer> m_vertexBuffer;
    std::vector<glm::vec4> m_cAry;
    std::vector<glm::vec4> m_vAry;

    globjects::ref_ptr<globjects::Program> m_program;
    globjects::ref_ptr<globjects::Texture> m_noise;

    glm::vec4 m_colorAndRatio;
    float m_glareIntensity;
    float m_glareScale;
    float m_apparentMagnitude;
    float m_scattering;
    float m_scintillations;
    float m_scale;
};

} // namespace glHimmel

#endif // __STARSGEODE_H__