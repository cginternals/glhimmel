#pragma once

#include <texturebased/AbstractMappedHimmel.h>
#include <texturebased/texturebased_api.h>

namespace globjects {
    class Shader;
    class Texture;
}

namespace glHimmel
{

    // uses method presented by Greene, Ned in "Environment 
    // Mapping and Other Applications of World Projections" 1986

    class TEXTUREBASED_API CubeMappedHimmel : public AbstractMappedHimmel
    {
    public:
        CubeMappedHimmel(const bool hBand = false, const bool fakeSun = false, const bool cubify = false);

        virtual ~CubeMappedHimmel();

        void setCubify(const bool cubify);
        const bool isCubified() const;

    protected:

        // AbstractHimmel interface

        std::string fragmentShaderPath() const override;
        GLenum textureTarget() const override;

        bool m_cubify;

    };

} // namespace glHimmel
