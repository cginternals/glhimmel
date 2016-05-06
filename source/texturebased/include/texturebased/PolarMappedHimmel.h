#pragma once

#include <memory>
#include <texturebased/texturebased_api.h>
#include <texturebased/AbstractMappedHimmel.h>

#include <globjects/base/ref_ptr.h>


#include <map>

namespace globjects {
    class Shader;
    class Texture;
}


namespace glHimmel
{

    class HorizonBand;


    // uses method presented by Blinn, James F. and Newell, Martin E.
    // in "Texture and reflection in computer generated images" 1976

    class TEXTUREBASED_API PolarMappedHimmel : public AbstractMappedHimmel
    {
    public:

        enum class MappingMode
        {
            Full
            , Half
        };

    public:

        PolarMappedHimmel(
            const MappingMode &mappingMode = MappingMode::Half
            , const bool hBand = false
            , const bool fakeSun = false);

        virtual ~PolarMappedHimmel() = default;

        // Use this helper to work with pre-configured textures.
        globjects::ref_ptr<globjects::Texture> getOrCreateTexture2D(const unsigned int textureUnit);

        MappingMode getMappingMode() const
        {
            return m_mappingMode;
        }

        // Use HorizonBand for horizon blending.
        HorizonBand* hBand() const;

        void update() override;

    protected:

        globjects::ref_ptr<globjects::Shader> getFragmentShader() override;

    protected:

        typedef std::map<GLint, globjects::ref_ptr<globjects::Texture>> t_tex2DById;
        t_tex2DById m_tex2DsById;

        MappingMode m_mappingMode;

        std::unique_ptr<HorizonBand> m_hBand;
    };

} // namespace glHimmel

