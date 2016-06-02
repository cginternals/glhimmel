#pragma once

#include <globjects/base/ref_ptr.h>
#include <glm/vec3.hpp>
#include <glbinding/gl/gl.h>

#include <map>
#include <vector>
#include <string>
#include <globjects/Program.h>


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

    globjects::Texture *setupTexture2D(
        const char *name // used as sampler identifier
    ,   const GLenum internalFormat
    ,   const GLenum pixelFormat
    ,   const GLenum dataType
    ,   const int width
    ,   const int height
    ,   osg::Image *image = NULL);

    globjects::Texture *setupTexture3D(
        const char *name // used as sampler identifier
    ,   const GLenum internalFormat
    ,   const GLenum pixelFormat
    ,   const GLenum dataType
    ,   const int width
    ,   const int height
    ,   const int depth
    ,   osg::Image *image = NULL);

    osg::Image *getLayerFrom3DImage(
        osg::Image *source
    ,   const int layer);

    void setupLayerUniforms(
        const int depth
    ,   const int layer);

    globjects::ref_ptr<globjects::Program> setupProgram(
        const std::string &fragmentShaderSource);

    osg::Camera *setupCamera(
        const int viewportWidth
    ,   const int viewportHeight
    ,   osg::Geode *geode
    ,   const int orderNum);

    osg::Group *setupGroup(
        osgViewer::CompositeViewer *viewer);

    void cleanUp(
        osgViewer::CompositeViewer *viewer);

    void assignUniforms(t_uniforms &uniforms);

    void assignSamplers(
        t_tex2DsByUnit &samplers2D
    ,   t_tex3DsByUnit &samplers3D);

    void dirtyTargets(t_tex2DsByUnit &targets2D);
    void dirtyTargets(t_tex3DsByUnit &targets3D);

    void render2D(
        osgViewer::CompositeViewer *viewer
    ,   osg::Geode *geode
    ,   t_tex2DsByUnit &targets2D
    ,   t_tex2DsByUnit &samplers2D
    ,   t_tex3DsByUnit &samplers3D
    ,   t_uniforms &uniforms
    ,   const char* fragmentShaderSource);

    void render3D(
        osgViewer::CompositeViewer *viewer
    ,   osg::Geode *geode
    ,   t_tex3DsByUnit &targets3D
    ,   t_tex2DsByUnit &samplers2D
    ,   t_tex3DsByUnit &samplers3D
    ,   t_uniforms &uniforms
    ,   const char* fragmentShaderSource);

protected:

    PrecomputedTextureConfig m_preTexCfg;
    PhysicalModelConfig m_modelCfg;

    globjects::ref_ptr<globjects::Texture> m_transmittanceTexture;
    globjects::ref_ptr<globjects::Texture> m_deltaETexture;
    globjects::ref_ptr<globjects::Texture> m_deltaSRTexture;
    globjects::ref_ptr<globjects::Texture> m_deltaSMTexture;
    globjects::ref_ptr<globjects::Texture> m_irradianceTexture;
    globjects::ref_ptr<globjects::Texture> m_inscatterTexture;
    globjects::ref_ptr<globjects::Texture> m_deltaJTexture;
};

} // namespace glHimmel


