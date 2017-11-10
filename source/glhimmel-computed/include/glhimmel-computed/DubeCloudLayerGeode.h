
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
#ifndef __DUBECLOUDLAYERGEODE_H__
#define __DUBECLOUDLAYERGEODE_H__

#include <glhimmel-computed/glhimmel-computed_api.h>
#include <glhimmel-base/ScreenAlignedQuad.h>
#include <globjects/Program.h>
#include <globjects/Texture.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace glHimmel
{

class ComputedHimmel;


class GLHIMMEL_COMPUTED_API DubeCloudLayerGeode
{
public:

    DubeCloudLayerGeode(const int texSize = 2048);
    virtual ~DubeCloudLayerGeode();

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

    const float setThickness(const float thickness);
    const float getThickness() const;

    const float setOffset(const float offset);
    const float getOffset() const;

    const glm::vec3 setBottomColor(const glm::vec3 &color);
    const glm::vec3 getBottomColor() const;

    const glm::vec3 setTopColor(const glm::vec3 &color);
    const glm::vec3 getTopColor() const;

    const glm::vec2 setWind(const glm::vec2 &wind);
    const glm::vec2 getWind() const;

    void draw() const;

protected:

    virtual void setupTextures();
    virtual void setupShader  ();

protected:

    ScreenAlignedQuad m_screenAlignedQuad;

    globjects::ref_ptr<globjects::Texture> m_preNoise;
    globjects::ref_ptr<globjects::Texture> m_noise[4];

    int m_noiseSize;

    globjects::ref_ptr<globjects::Program> m_program;

    float m_coverage;
    float m_sharpness;
    float m_change;
    glm::vec2 m_wind;
    float m_altitude;
    glm::vec2 m_scale;
    float m_thickness;
    float m_offset;
    glm::vec3 m_bcolor;
    glm::vec3 m_tcolor;
};

} // namespace glHimmel

#endif // __DUBECLOUDLAYERGEODE_H__