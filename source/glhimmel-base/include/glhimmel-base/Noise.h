
// Copyright (c) 2011-2012, Daniel M�ller <dm@g4t3.de>
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
#ifndef __NOISE_H__
#define __NOISE_H__

#include <glhimmel-base/glhimmel-base_api.h>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>


namespace glHimmel
{

namespace
{
    const unsigned int PERMSIZE    (0x100);
    const unsigned int MAXPERMINDEX(0xff);
}


class GLHIMMEL_BASE_API Noise
{
public:
    Noise(
        const unsigned int rank = 8
    ,   const float xOffset = 0
    ,   const float yOffset = 0);

    void generatePermutationMap(unsigned char *dest) const;


    const float noise2(
        const float s
    ,   const float t) const;
    const float noise2(
        const float s
    ,   const float t
    ,   const unsigned int r) const;


protected:

    static const float fade(const float t);

    const unsigned int hash(
        const unsigned int x
    ,   const unsigned int y) const;
    const unsigned int hash(
        const unsigned int x
    ,   const unsigned int y
    ,   const unsigned int r) const;

    const glm::vec2 grad2(
        const float x
	  ,   const float y) const;
    const glm::vec2 grad2(
			  const float x
		,   const float y
    ,   const unsigned int r) const;

    const glm::vec3 grad3(
        const unsigned int x
    ,   const unsigned int y) const;
    const glm::vec3 grad3(
        const unsigned int x
    ,   const unsigned int y
    ,   const unsigned int r) const;

    const glm::vec4 grad3h(
        const unsigned int x
    ,   const unsigned int y) const;
    const glm::vec4 grad3h(
        const unsigned int x
    ,   const unsigned int y
    ,   const unsigned int r) const;


    static const float mix(
        const float a
    ,   const float b
    ,   const float t);

    static const glm::vec2 mix(
        const glm::vec2 &a
    ,   const glm::vec2 &b
    ,   const float t);

private:

    /*const ?? throws warnings */ unsigned int m_size;

    float m_xoff;
    float m_yoff;

    static const unsigned char m_perm[PERMSIZE];
    static const float m_grad[16][3];
};

} // namespace glHimmel

#endif // __NOISE_H__