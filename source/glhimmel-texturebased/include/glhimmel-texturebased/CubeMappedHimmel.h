#pragma once

#include <glhimmel-texturebased/AbstractMappedHimmel.h>
#include <glhimmel-texturebased/glhimmel-texturebased_api.h>

namespace globjects {
    class Shader;
    class Texture;
}

namespace glHimmel
{

    // uses method presented by Greene, Ned in "Environment 
    // Mapping and Other Applications of World Projections" 1986

    class GLHIMMEL_TEXTUREBASED_API CubeMappedHimmel : public AbstractMappedHimmel
    {
    public:
        CubeMappedHimmel(const bool hBand = false, const bool fakeSun = false);

        virtual ~CubeMappedHimmel();

    protected:

        // AbstractHimmel interface

        std::string fragmentShaderPath() const override;
        GLenum textureTarget() const override;

    };

} // namespace glHimmel
