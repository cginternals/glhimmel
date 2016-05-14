#include <globjects/Shader.h>
#include <globjects/Texture.h>

#include <texturebased/PolarMappedHimmel.h>

using namespace gl;

namespace glHimmel
{
PolarMappedHimmel::PolarMappedHimmel(const MappingMode & mappingMode, const bool hBand, const bool fakeSun)
: AbstractMappedHimmel(fakeSun)
, m_mappingMode(mappingMode)
{
    if (hBand)
    {
        m_hBand = std::unique_ptr<HorizonBand>(new HorizonBand());
    }
}



void PolarMappedHimmel::update()
{
    AbstractMappedHimmel::update();
    m_program->setUniform("isHalf", m_mappingMode == MappingMode::Half);
}

std::string PolarMappedHimmel::fragmentShaderPath()
{
    return "data/shader/polarMappedHimmel.frag";
}

} // namespace glHimmel

