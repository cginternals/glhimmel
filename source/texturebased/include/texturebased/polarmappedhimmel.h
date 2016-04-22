#pragma once

#include <memory>
#include <texturebased/texturebased_api.h>
#include <texturebased/abstractmappedhimmel.h>

#include <map>


namespace osg
{
    class Texture2D;
}


namespace glHimmel
{

    class HorizonBand;


    // uses method presented by Blinn, James F. and Newell, Martin E.
    // in "Texture and reflection in computer generated images" 1976

    class TEXTUREBASED_API PolarMappedHimmel : public AbstractMappedHimmel
    {
    public:

        enum e_MappingMode
        {
            MM_Full
            , MM_Half
        };

    public:

        PolarMappedHimmel(
            const e_MappingMode &mappingMode = MM_Half
            , const bool hBand = false
            , const bool fakeSun = false);

        virtual ~PolarMappedHimmel();

        // Use this helper to work with pre-configured textures.
        osg::Texture2D* getOrCreateTexture2D(const GLint textureUnit);

        inline const e_MappingMode getMappingMode() const
        {
            return m_mappingMode;
        }

        // Use HorizonBand for horizon blending.
        HorizonBand *hBand();

    protected:

        // AbstractMappedHimmel interface

        //virtual osg::StateAttribute *getTextureAttribute(const GLint textureUnit) const;


        // AbstractHimmel interface

        virtual const std::string getFragmentShaderSource();

    protected:

        typedef std::map<GLint, std::unique_ptr<osg::Texture2D> > t_tex2DById;
        t_tex2DById m_tex2DsById;

        e_MappingMode m_mappingMode;

        HorizonBand *m_hBand;
    };

} // namespace glHimmel

