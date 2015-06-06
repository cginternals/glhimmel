
#pragma once
#ifndef __PARABOLOIDMAPPEDHIMMEL_H__
#define __PARABOLOIDMAPPEDHIMMEL_H__

#include <glhimmel-texturebased/glhimmel-texturebased_api.h>
#include <glhimmel-texturebased/abstractmappedhimmel.h>

#include <map>

namespace osg
{
    class Texture2D;
}

namespace glhimmel_texturebased
{

class HorizonBand;


// uses method presented by Heidrich, Wolfgang and Seidel, 
// Hans-Peter in "View-independent environment maps" 1998

// Dual-Paraboloid not supported yet...
// The map is mapped towards +z (top).

class GLHIMMEL_TEXTUREBASED_API ParaboloidMappedHimmel : public AbstractMappedHimmel
{
public:
    ParaboloidMappedHimmel(
        const bool horizonBand = false
    ,   const bool fakeSun = false);
    virtual ~ParaboloidMappedHimmel();

    // Use this helper to work with pre-configured textures.
    osg::Texture2D* getOrCreateTexture2D(const GLint textureUnit);

    // Use HorizonBand for horizon blending.
    HorizonBand *hBand();

protected:

    // AbstractMappedHimmel interface

    virtual osg::StateAttribute *getTextureAttribute(const GLint textureUnit) const;


    // AbstractHimmel interface

    virtual const std::string getFragmentShaderSource();

protected:

    typedef std::map<GLint, osg::ref_ptr<osg::Texture2D> > t_tex2DById;
    t_tex2DById m_tex2DsById;

    HorizonBand *m_hBand;
    const bool m_withHBand;
};

} // namespace glhimmel_texturebased

#endif // __PARABOLOIDMAPPEDHIMMEL_H__