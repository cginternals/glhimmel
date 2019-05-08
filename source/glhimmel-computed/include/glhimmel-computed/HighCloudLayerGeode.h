
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
#ifndef __HIGHCLOUDLAYERGEODE_H__
#define __HIGHCLOUDLAYERGEODE_H__

#include <glhimmel-computed/glhimmel-computed_api.h>
#include <glhimmel-base/ScreenAlignedTriangle.h>
#include <globjects/Program.h>
#include <globjects/Texture.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace glHimmel
{

class ComputedHimmel;


/*
alt    9,30
change 0,02
scale 64,00
cover  0,40
sharp  0,32
windx -0,01
windy -0,02
*/

class GLHIMMEL_COMPUTED_API HighCloudLayerGeode
{
public:

    HighCloudLayerGeode(const int texSize = 2048);
    virtual ~HighCloudLayerGeode();

    void update(const ComputedHimmel &himmel);

    const float setCoverage(const float coverage);
    const float getCoverage() const;

    const float setSharpness(const float sharpness);
    const float getSharpness() const;

    const float setAltitude(const float altitude);
    const float getAltitude() const;
    static const float defaultAltitude();

    const glm::vec2 setScale(const glm::vec2 &scale);
    const glm::vec2 getScale() const;
    static const glm::vec2 defaultScale();

    const float setChange(const float change);
    const float getChange() const;
    static const float defaultChange();

    const glm::vec2 setWind(const glm::vec2 &wind);
    const glm::vec2 getWind() const;

    const glm::vec3 setColor(const glm::vec3 &color);
    const glm::vec3 getColor() const;

    void draw() const;

public:

    static void createPreRenderedNoise(
        const unsigned texSize
    ,   globjects::ref_ptr<globjects::Texture> &texture
    ,   float time
    ,   float coverage
    ,   float sharpness
    ,   float change
    ,   glm::vec2 wind
    ,   globjects::ref_ptr<globjects::Texture> *noise);

    static std::vector<float> createNoiseSlice(
        const unsigned int texSize
    ,   const unsigned int octave);

    static globjects::ref_ptr<globjects::Texture> createNoiseArray(
        const unsigned int texSize
    ,   const unsigned int octave
    ,   const unsigned int slices);

protected:

    virtual void setupTextures();
    virtual void setupShader  ();

protected:

    ScreenAlignedTriangle m_screenAlignedTriangle;

    int m_noiseSize;
    globjects::ref_ptr<globjects::Texture> m_noise[4];
    globjects::ref_ptr<globjects::Texture> m_preNoise;

    globjects::ref_ptr<globjects::Program> m_program;
    float m_coverage = 0.2f;
    float m_sharpness = 0.5f;
    float m_change;
    glm::vec2 m_wind;
    glm::vec3 m_color;
    float m_altitude;
    glm::vec2 m_scale;
};

} // namespace glHimmel

#endif // __HIGHCLOUDLAYERGEODE_H__