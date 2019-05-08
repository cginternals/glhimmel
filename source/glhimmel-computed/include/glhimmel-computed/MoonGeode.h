
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
#ifndef __MOONGEODE_H__
#define __MOONGEODE_H__

#include <glhimmel-computed/glhimmel-computed_api.h>
#include <glhimmel-base/ScreenAlignedQuad.h>
#include <globjects/Program.h>
#include <globjects/Texture.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>


namespace glHimmel
{

class ComputedHimmel;


class GLHIMMEL_COMPUTED_API MoonGeode
{
public:

    MoonGeode();
    virtual ~MoonGeode();

    void update(ComputedHimmel &himmel);

    const float setScale(const float scale);
    const float getScale() const;
    static const float defaultScale();

    const glm::vec3 setSunShineColor(const glm::vec3 &color);
    const glm::vec3 getSunShineColor() const;
    static const glm::vec3 defaultSunShineColor();

    const float setSunShineIntensity(const float intensity);
    const float getSunShineIntensity() const;
    static const float defaultSunShineIntensity();

    const glm::vec3 setEarthShineColor(const glm::vec3 &color);
    const glm::vec3 getEarthShineColor() const;
    static const glm::vec3 defaultEarthShineColor();

    const float setEarthShineIntensity(const float intensity);
    const float getEarthShineIntensity() const;
    static const float defaultEarthShineIntensity();

    globjects::ref_ptr<globjects::Texture> getMoonTextureCube();

    void draw() const;

protected:

    void setupTextures();

    void setupMoonTextureCube();

    void setupEclipseTexture();

    void setupShader();

protected:

    ScreenAlignedQuad m_screenAlignedQuad;

    globjects::ref_ptr<globjects::Program> m_program;

    globjects::ref_ptr<globjects::Texture> m_moonCube;
    globjects::ref_ptr<globjects::Texture> m_eclipse;

    glm::vec4 m_sunShine;

    float m_scale;

    float m_earthShineScale;
    glm::vec3 m_earthShineColor;
};

} // namespace glHimmel

#endif // __MOONGEODE_H__