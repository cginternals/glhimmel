#pragma once

#include <texturebased/AbstractMappedHimmel.h>
#include <texturebased/texturebased_api.h>

namespace globjects {
    class Shader;
    class Texture;
}

namespace glHimmel
{

    // uses method presented by Heidrich, Wolfgang and Seidel, 
    // Hans-Peter in "View-independent environment maps" 1998

    // Dual-Paraboloid not supported yet...
    // The map is mapped towards +z (top).

    class TEXTUREBASED_API ParaboloidMappedHimmel : public AbstractMappedHimmel
    {
    public:
        ParaboloidMappedHimmel(const bool hBand = false, const bool fakeSun = false);

        virtual ~ParaboloidMappedHimmel();

    protected:

        // AbstractHimmel interface

        std::string fragmentShaderPath() const override;

    };

} // namespace glHimmel



