
#pragma once
#ifndef __CUBEMAPPEDHIMMEL_H__
#define __CUBEMAPPEDHIMMEL_H__

#include <glhimmel-texturebased/glhimmel-texturebased_api.h>
#include <glhimmel-texturebased/abstractmappedhimmel.h>

#include <map>

namespace osg
{
    class TextureCubeMap;
}


namespace glhimmel_texturebased
{

// uses method presented by Greene, Ned in "Environment 
// Mapping and Other Applications of World Projections" 1986

class GLHIMMEL_TEXTUREBASED_API CubeMappedHimmel : public AbstractMappedHimmel
{
public:
    CubeMappedHimmel(
        const bool fakeSun = false
    ,   const bool cubify = false);

    virtual ~CubeMappedHimmel();

    // Use this helper to work with pre-configured textures.
    osg::TextureCubeMap *getOrCreateTextureCubeMap(const GLint textureUnit);

    void setCubify(const bool cubify);  
    const bool isCubified() const;

protected:

    // AbstractMappedHimmel interface

    virtual osg::StateAttribute *getTextureAttribute(const GLint textureUnit) const;

    // AbstractHimmel interface

    virtual const std::string getFragmentShaderSource();

protected:

    typedef std::map<GLubyte, osg::ref_ptr<osg::TextureCubeMap> > t_tcmById;
    t_tcmById m_tcmsById;

    // this overcomes the texel per samplingfield-angle issue of cube mapping - see cpp for more details.
    bool m_cubify;
};

} // namespace glhimmel_texturebased

#endif // __CUBEMAPPEDHIMMEL_H__