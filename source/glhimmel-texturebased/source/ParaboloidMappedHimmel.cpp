
#include <glhimmel-texturebased/ParaboloidMappedHimmel.h>

namespace glHimmel
{

ParaboloidMappedHimmel::ParaboloidMappedHimmel(const bool hBand, const bool fakeSun)
    : AbstractMappedHimmel(hBand, fakeSun)
{
}


ParaboloidMappedHimmel::~ParaboloidMappedHimmel()
{
}



std::string ParaboloidMappedHimmel::fragmentShaderPath() const
{
    return "data/shader/paraboloidMappedHimmel.frag";
}

} // namespace glHimmel
