
#pragma once
#ifndef __HIMMELQUAD_H__
#define __HIMMELQUAD_H__

#include <glhimmel-texturebased/glhimmel-texturebased_api.h>
#include <osg/Geometry>


namespace glhimmel_texturebased
{

class GLHIMMEL_TEXTUREBASED_API HimmelQuad : public osg::Geometry
{
public:
    HimmelQuad();
};

} // namespace glhimmel_texturebased

#endif // __HIMMELQUAD_H__