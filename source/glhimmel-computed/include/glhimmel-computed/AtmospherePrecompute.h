#pragma once

#include <globjects/base/ref_ptr.h>
#include <glm/vec3.hpp>
#include <glbinding/gl/gl.h>

#include <map>
#include <vector>
#include <string>
#include <globjects/Program.h>
#include <glhimmel-base/ScreenAlignedTriangle.h>


using namespace gl;

namespace globjects
{
    class Texture;
}

namespace glHimmel
{

class AtmospherePrecompute
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

    PhysicalModelConfig &getModelConfig()
    {
        return m_modelCfg;
    }

protected:

    PrecomputedTextureConfig &getTextureConfig()
    {
        return m_preTexCfg;
    }

protected:

    typedef std::map<GLint, globjects::ref_ptr<globjects::Texture>> t_tex2DsByUnit;
    typedef std::map<GLint, globjects::ref_ptr<globjects::Texture>> t_tex3DsByUnit;

public:

    AtmospherePrecompute();
    virtual ~AtmospherePrecompute();

    globjects::ref_ptr<globjects::Texture> getTransmittanceTexture();
    globjects::ref_ptr<globjects::Texture> getIrradianceTexture();
    globjects::ref_ptr<globjects::Texture> getInscatterTexture();

    const bool compute();

    void substituteMacros(std::string &source);

protected:

    globjects::ref_ptr<globjects::Texture> getDeltaETexture();
    globjects::ref_ptr<globjects::Texture> getDeltaSRTexture();
    globjects::ref_ptr<globjects::Texture> getDeltaSMTexture();
    globjects::ref_ptr<globjects::Texture> getDeltaJTexture();

    osg::GraphicsContext *setupContext();

    osg::Geode *genQuad() const;

    globjects::ref_ptr<globjects::Texture> setupTexture2D(
        const GLenum internalFormat
    ,   const GLenum pixelFormat
    ,   const GLenum dataType
    ,   const int width
    ,   const int height);

    globjects::ref_ptr<globjects::Texture> setupTexture3D(
        const GLenum internalFormat
    ,   const GLenum pixelFormat
    ,   const GLenum dataType
    ,   const int width
    ,   const int height
    ,   const int depth);

    osg::Image *getLayerFrom3DImage(
        osg::Image *source
    ,   const int layer);

    void setupLayerUniforms(
        globjects::ref_ptr<globjects::Program> program
    ,   const int depth
    ,   const int layer);

    globjects::ref_ptr<globjects::Program> setupProgram(
        const std::string &fragmentShaderSource);

    void setUniforms(globjects::ref_ptr<globjects::Program> program);

    void dirtyTargets(t_tex2DsByUnit &targets2D);
    void dirtyTargets(t_tex3DsByUnit &targets3D);

    void render2D(
        std::vector<globjects::ref_ptr<globjects::Texture>> &targets2D
    ,   globjects::ref_ptr<globjects::Program> program);

    void render3D(
        std::vector<globjects::ref_ptr<globjects::Texture>> &targets3D
    ,   globjects::ref_ptr<globjects::Program> program);

protected:
    globjects::ref_ptr<globjects::Program> m_brunetonTransmittanceProgram;
    globjects::ref_ptr<globjects::Program> m_brunetonIrradianceProgram;
    globjects::ref_ptr<globjects::Program> m_brunetonInscatterProgram;
    globjects::ref_ptr<globjects::Program> m_brunetonCopyIrradianceProgram;
    globjects::ref_ptr<globjects::Program> m_brunetonCopyInscatterProgram;   
    globjects::ref_ptr<globjects::Program> m_brunetonIrradianceProgramN;
    globjects::ref_ptr<globjects::Program> m_brunetonInscatterProgramN;
    globjects::ref_ptr<globjects::Program> m_brunetonInscatterProgramS;
    globjects::ref_ptr<globjects::Program> m_brunetonCopyInscatterProgramN;

    PrecomputedTextureConfig m_preTexCfg;
    PhysicalModelConfig m_modelCfg;

    globjects::ref_ptr<globjects::Texture> m_transmittanceTexture;
    globjects::ref_ptr<globjects::Texture> m_deltaETexture;
    globjects::ref_ptr<globjects::Texture> m_deltaSRTexture;
    globjects::ref_ptr<globjects::Texture> m_deltaSMTexture;
    globjects::ref_ptr<globjects::Texture> m_irradianceTexture;
    globjects::ref_ptr<globjects::Texture> m_inscatterTexture;
    globjects::ref_ptr<globjects::Texture> m_deltaJTexture;

    ScreenAlignedTriangle m_triangle;
};

} // namespace glHimmel


