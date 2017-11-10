#pragma once

#include <glhimmel-computed/glhimmel-computed_api.h>
#include <globjects/base/ref_ptr.h>
#include <glm/vec3.hpp>
#include <glbinding/gl/gl.h>

#include <vector>
#include <string>
#include <globjects/Framebuffer.h>
#include <globjects/Program.h>
#include <glhimmel-base/ScreenAlignedTriangle.h>
#include <globjects/Texture.h>


using namespace gl;

namespace globjects
{
    class Texture;
}

namespace glHimmel
{

class GLHIMMEL_COMPUTED_API AtmospherePrecompute
{
public:
    
    struct PrecomputedTextureConfig
    {
        int transmittanceWidth;
        int transmittanceHeight;
    
        int skyHeight;
        int skyWidth;

        int resR;
        int resMu;
        int resMuS;
        int resNu;

        int transmittanceIntegralSamples;
        int inscatterIntegralSamples;
        int irradianceIntegralSamples;
        int inscatterSphericalIntegralSamples;
    };

    struct PhysicalModelConfig
    {
        float avgGroundReflectance;

        float HR; // Reighley const denisty Atmosphere height in km
        glm::vec3 betaR;

        float HM; // Mie const denisty Atmosphere height in km
        glm::vec3 betaMSca;
        glm::vec3 betaMEx;
        float mieG;
    } ;

    PhysicalModelConfig& getModelConfig();
    void setAltitude(float altitude);
    void setSeed(float seed);

protected:

    PrecomputedTextureConfig &getTextureConfig();

public:

    AtmospherePrecompute();
    virtual ~AtmospherePrecompute();

    globjects::ref_ptr<globjects::Texture> getTransmittanceTexture() const;
    globjects::ref_ptr<globjects::Texture> getIrradianceTexture() const;
    globjects::ref_ptr<globjects::Texture> getInscatterTexture() const;

    void setUniforms(globjects::ref_ptr<globjects::Program> program) const;

    void compute();
    void fetchShaderIncludes() const;

protected:

    globjects::ref_ptr<globjects::Texture> setupTexture2D(
        const GLenum internalFormat
    ,   const GLenum pixelFormat
    ,   const GLenum dataType
    ,   const int width
    ,   const int height) const;

    globjects::ref_ptr<globjects::Texture> setupTexture3D(
        const GLenum internalFormat
    ,   const GLenum pixelFormat
    ,   const GLenum dataType
    ,   const int width
    ,   const int height
    ,   const int depth) const;

    void setupLayerUniforms(
        globjects::ref_ptr<globjects::Program> program
    ,   const int depth
    ,   const int layer);

    globjects::ref_ptr<globjects::Program> setupProgram(
        const std::string &fragmentShaderSource) const;

    void render2D(
        std::vector<globjects::ref_ptr<globjects::Texture>> &targets2D
    ,   globjects::ref_ptr<globjects::Program> program) const;

    void render3D(
        std::vector<globjects::ref_ptr<globjects::Texture>> &targets3D
    ,   globjects::ref_ptr<globjects::Program> program);

protected:
    globjects::ref_ptr<globjects::Program> m_transmittanceProgram;
    globjects::ref_ptr<globjects::Program> m_irradiance1Program;
    globjects::ref_ptr<globjects::Program> m_inscatter1Program;
    globjects::ref_ptr<globjects::Program> m_copyIrradianceProgram;
    globjects::ref_ptr<globjects::Program> m_copyInscatter1Program;   
    globjects::ref_ptr<globjects::Program> m_irradianceNProgram;
    globjects::ref_ptr<globjects::Program> m_inscatterNProgram;
    globjects::ref_ptr<globjects::Program> m_inscatterSProgram;
    globjects::ref_ptr<globjects::Program> m_copyInscatterNProgram;

    PrecomputedTextureConfig m_preTextureConfig;
    PhysicalModelConfig m_modelConfig;

    globjects::ref_ptr<globjects::Texture> m_transmittanceTexture;
    globjects::ref_ptr<globjects::Texture> m_deltaETexture;
    globjects::ref_ptr<globjects::Texture> m_deltaSRTexture;
    globjects::ref_ptr<globjects::Texture> m_deltaSMTexture;
    globjects::ref_ptr<globjects::Texture> m_irradianceTexture;
    globjects::ref_ptr<globjects::Texture> m_irradianceTextureTmp;
    globjects::ref_ptr<globjects::Texture> m_inscatterTexture;
    globjects::ref_ptr<globjects::Texture> m_inscatterTextureTmp;
    globjects::ref_ptr<globjects::Texture> m_deltaJTexture;

    globjects::ref_ptr<globjects::Framebuffer> m_fbo;
    ScreenAlignedTriangle m_screenAlignedTriangle;

    float m_altitude;
    float m_seed;
};

} // namespace glHimmel


