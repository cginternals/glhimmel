
#pragma once
#ifndef __SPHEREMAPPEDHIMMEL_H__
#define __SPHEREMAPPEDHIMMEL_H__

#include <glhimmel-texturebased/glhimmel-texturebased_api.h>
#include <glhimmel-texturebased/abstractmappedhimmel.h>

#include <map>

namespace osg
{
    class Texture2D;
}


namespace glhimmel_texturebased
{

// uses method presented by Miller, Gene S. and Hoffman, Robert C. in "Illumination 
// and Reflection Maps: Simulated Objects in Simulated and Real Environments" 1984

class GLHIMMEL_TEXTUREBASED_API SphereMappedHimmel : public AbstractMappedHimmel
{
public:
    SphereMappedHimmel(
        const osg::Vec3f &center = osg::Vec3f(0.0, 0.0, 1.0)
    ,   const bool fakeSun = false);

    virtual ~SphereMappedHimmel();

    // Use this helper to work with pre-configured textures.
    osg::Texture2D* getOrCreateTexture2D(const GLint textureUnit);

protected:

    // AbstractMappedHimmel interface

    virtual osg::StateAttribute *getTextureAttribute(const GLint textureUnit) const;


    // AbstractHimmel interface

    virtual const std::string getVertexShaderSource();
    virtual const std::string getFragmentShaderSource();

protected:

    typedef std::map<GLint, osg::ref_ptr<osg::Texture2D> > t_tex2DById;
    t_tex2DById m_tex2DsById;

    osg::ref_ptr<osg::Uniform> u_transform;
};

} // namespace glhimmel_texturebased

#endif // __SPHEREMAPPEDHIMMEL_H__