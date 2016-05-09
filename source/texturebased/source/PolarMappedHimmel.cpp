#include <globjects/Shader.h>
#include <globjects/Texture.h>

#include <texturebased/PolarMappedHimmel.h>
#include <texturebased/HorizonBand.h>

using namespace gl;

namespace glHimmel
{
PolarMappedHimmel::PolarMappedHimmel(const MappingMode & mappingMode, const bool hBand, const bool fakeSun)
:   m_mappingMode(mappingMode)
{
    if (hBand)
    {
        m_hBand = std::unique_ptr<HorizonBand>(new HorizonBand());
    }
}

globjects::ref_ptr<globjects::Texture> PolarMappedHimmel::getOrCreateTexture2D(const unsigned int textureUnit)
{
    // Retrieve an existing texture.

    const auto existingTex2D = m_tex2DsById.find(textureUnit);
    if (existingTex2D != m_tex2DsById.end())
        return existingTex2D->second;

    // Create and configure new texture object.

    auto newTex2D = globjects::Texture::createDefault();

    newTex2D->setParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);

    m_tex2DsById[textureUnit] = newTex2D;

    // Assign some textures if there are none.

    if (m_tex2DsById.size() == 1)
        newTex2D->bindActive(textureUnit);
    if (m_tex2DsById.size() == 2)
        newTex2D->bindActive(textureUnit);

    return newTex2D;
}

void PolarMappedHimmel::update()
{
    m_hBand->updateUniforms(m_program);
    m_program->setUniform("hBand", m_hBand.get() != nullptr);
    m_program->setUniform("half", m_mappingMode == MappingMode::Half);
}

globjects::ref_ptr<globjects::Shader> PolarMappedHimmel::getFragmentShader()
{
    return globjects::Shader::fromFile(GL_VERTEX_SHADER, "data/shader/polarMappedHimmel.frag");
}

HorizonBand* PolarMappedHimmel::hBand() const
{
    return m_hBand.get();
}

} // namespace glHimmel

