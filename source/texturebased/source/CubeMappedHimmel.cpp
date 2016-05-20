#include <texturebased/CubeMappedHimmel.h>

#include <glbinding/gl/enum.h>


namespace glHimmel
{

CubeMappedHimmel::CubeMappedHimmel(
    const bool hBand
    , const bool fakeSun
    , const bool cubify)
    : AbstractMappedHimmel(hBand, fakeSun)
    , m_cubify(cubify)
{
};


CubeMappedHimmel::~CubeMappedHimmel()
{
};

GLenum CubeMappedHimmel::textureTarget() const
{
    return GL_TEXTURE_CUBE_MAP;
}

std::string CubeMappedHimmel::fragmentShaderPath() const
{
    return "data/shader/cubeMappedHimmel.frag";
}

} // namespace glHimmel