
#include <texturebased/SphereMappedHimmel.h>

namespace glHimmel
{

SphereMappedHimmel::SphereMappedHimmel(const bool hBand, const bool fakeSun)
:   AbstractMappedHimmel(hBand, fakeSun)
{
}


SphereMappedHimmel::~SphereMappedHimmel()
{
}



std::string SphereMappedHimmel::fragmentShaderPath() const
{
    return "data/shader/sphereMappedHimmel.frag";
}

} // namespace osgHimmel
