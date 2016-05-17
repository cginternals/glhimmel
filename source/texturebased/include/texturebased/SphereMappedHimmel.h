#pragma once

#include <texturebased/AbstractMappedHimmel.h>
#include <texturebased/texturebased_api.h>
#include <globjects/base/ref_ptr.h>
#include <glm/vec3.hpp>

#include <map>

namespace globjects {
    class Shader;
    class Texture;
}



namespace glHimmel
{

// uses method presented by Miller, Gene S. and Hoffman, Robert C. in "Illumination 
// and Reflection Maps: Simulated Objects in Simulated and Real Environments" 1984

class TEXTUREBASED_API SphereMappedHimmel : public AbstractMappedHimmel
{
public:
    SphereMappedHimmel(const bool hBand = false, const bool fakeSun = false);

    virtual ~SphereMappedHimmel();

protected:

    // AbstractHimmel interface

    std::string fragmentShaderPath() const override;

protected:

    typedef std::map<GLint, globjects::ref_ptr<globjects::Texture>> t_tex2DById;
    t_tex2DById m_tex2DsById;

    glm::vec3 m_transform;
};

} // namespace glHimmel
