
#include <texturebased/PolarMappedHimmel.h>
#include <texturebased/HorizonBand.h>


namespace glHimmel
{
PolarMappedHimmel::PolarMappedHimmel(const MappingMode & mappingMode, const bool hBand, const bool fakeSun)
{
    if (hBand)
    {
        m_hBand = std::make_unique<HorizonBand>();
    }
}

HorizonBand* PolarMappedHimmel::hBand() const
{
    return m_hBand.get();
}

} // namespace glHimmel

