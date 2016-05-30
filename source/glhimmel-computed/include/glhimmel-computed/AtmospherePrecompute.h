#pragma once

#include <globjects/base/ref_ptr.h>
#include <glm/vec3.hpp>
#include <glbinding/gl/gl.h>

#include <map>
#include <vector>
#include <string>
#include <globjects/Program.h>


namespace osg
{
    class Texture2D;
    class Texture3D;
    class Image;
    class Uniform;
    class GraphicsContext;
    class Geode;
    class StateSet;
    class Program;
    class Camera;
    class Group;
}

namespace osgViewer
{
    class CompositeViewer;
}

using namespace gl;

namespace glHimmel
{

class AtmospherePrecompute
{
public:
    
    typedef struct PrecomputedTextureConfig
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

    } t_preTexCfg;

    typedef struct PhysicalModelConfig
    {
        float avgGroundReflectance;

        float HR; // Reighley const denisty Atmosphere height in km
        glm::vec3 betaR;

        float HM; // Mie const denisty Atmosphere height in km
        glm::vec3 betaMSca;
        glm::vec3 betaMEx;
        float mieG;

    } t_modelCfg;

    t_modelCfg &getModelConfig()
    {
        return m_modelCfg;
    }

protected:

    t_preTexCfg &getTextureConfig()
    {
        return m_preTexCfg;
    }

protected:

    typedef std::map<GLint, osg::Texture2D*> t_tex2DsByUnit;
    typedef std::map<GLint, osg::Texture3D*> t_tex3DsByUnit;

    typedef std::vector<osg::Uniform*> t_uniforms;

public:

    AtmospherePrecompute();
    virtual ~AtmospherePrecompute();

    osg::Texture2D *getTransmittanceTexture();
    osg::Texture2D *getIrradianceTexture();
    osg::Texture3D *getInscatterTexture();

    const bool compute(const bool ifDirtyOnly = true);
    void dirty();

    void substituteMacros(std::string &source);

protected:

    osg::Texture2D *getDeltaETexture();
    osg::Texture3D *getDeltaSRTexture();
    osg::Texture3D *getDeltaSMTexture();
    osg::Texture3D *getDeltaJTexture();

    osg::GraphicsContext *setupContext();

    osg::Geode *genQuad() const;

    osg::Texture2D *setupTexture2D(
        const char *name // used as sampler identifier
    ,   const GLenum internalFormat
    ,   const GLenum pixelFormat
    ,   const GLenum dataType
    ,   const int width
    ,   const int height
    ,   osg::Image *image = NULL);

    osg::Texture3D *setupTexture3D(
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
        osg::StateSet *stateSet
    ,   const int depth
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

    void assignUniforms(
        osg::StateSet *stateSet
    ,   t_uniforms &uniforms);

    void assignSamplers(
        osg::StateSet *stateSet
    ,   t_tex2DsByUnit &samplers2D
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

    bool m_dirty;

    t_preTexCfg m_preTexCfg;
    t_modelCfg m_modelCfg;

    globjects::ref_ptr<osg::Texture2D> m_transmittanceTexture;
    globjects::ref_ptr<osg::Texture2D> m_deltaETexture;
    globjects::ref_ptr<osg::Texture3D> m_deltaSRTexture;
    globjects::ref_ptr<osg::Texture3D> m_deltaSMTexture;
    globjects::ref_ptr<osg::Texture2D> m_irradianceTexture;
    globjects::ref_ptr<osg::Texture3D> m_inscatterTexture;
    globjects::ref_ptr<osg::Texture3D> m_deltaJTexture;

    globjects::ref_ptr<osg::Image> m_transmittanceImage;
    globjects::ref_ptr<osg::Image> m_irradianceImage;
    globjects::ref_ptr<osg::Image> m_inscatterImage;
};

} // namespace glHimmel


