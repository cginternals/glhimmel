#include <glhimmel-computed/AtmospherePrecompute.h>

#include <glhimmel-computed/ComputedHimmel.h>
#include <glhimmel-computed/Earth.h>

#include <assert.h>
#include <cstring>
#include <globjects/Texture.h>
#include <globjects/Shader.h>
#include <globjects/Framebuffer.h>


namespace glHimmel
{

AtmospherePrecompute::AtmospherePrecompute()
{
    m_preTexCfg.transmittanceWidth  = 256;
    m_preTexCfg.transmittanceHeight =  64;

    m_preTexCfg.skyWidth  =  64;
    m_preTexCfg.skyHeight =  16;

    m_preTexCfg.resR   =  32;
    m_preTexCfg.resMu  = 128;
    m_preTexCfg.resMuS =  32;
    m_preTexCfg.resNu  =   8;

    m_preTexCfg.transmittanceIntegralSamples      = 500;
    m_preTexCfg.inscatterIntegralSamples          =  50;
    m_preTexCfg.irradianceIntegralSamples         =  32;
    m_preTexCfg.inscatterSphericalIntegralSamples =  16;

    m_modelCfg.avgGroundReflectance = 0.1f;

    m_modelCfg.HR = 8.f;
    m_modelCfg.betaR = glm::vec3(5.8e-3, 1.35e-2, 3.31e-2);
    
    m_modelCfg.HM = 6.f; //1.2f;
    m_modelCfg.betaMSca = glm::vec3(1.0, 1.0, 1.0) * 20e-3f, //8e-3; 
    m_modelCfg.betaMEx = m_modelCfg.betaMSca / 0.9f;
    m_modelCfg.mieG = 0.6; //0.76;


    // Setup Textures

    m_transmittanceTexture = getTransmittanceTexture();
    m_deltaETexture        = getDeltaETexture();
    m_deltaSRTexture       = getDeltaSRTexture();
    m_deltaSMTexture       = getDeltaSMTexture();
    m_irradianceTexture    = getIrradianceTexture();
    m_inscatterTexture     = getInscatterTexture();
    m_deltaJTexture        = getDeltaJTexture();

    // Setup Program
    setupProgram();
}


AtmospherePrecompute::~AtmospherePrecompute()
{
}


globjects::ref_ptr<globjects::Texture> AtmospherePrecompute::getTransmittanceTexture()
{
    return setupTexture2D("transmittance", GL_RGB16F_ARB, GL_RGB, GL_FLOAT
        , getTextureConfig().transmittanceWidth, getTextureConfig().transmittanceHeight);
}
globjects::ref_ptr<globjects::Texture> AtmospherePrecompute::getIrradianceTexture()
{
    return setupTexture2D("irradiance", GL_RGB16F_ARB, GL_RGB, GL_FLOAT
        , getTextureConfig().skyWidth, getTextureConfig().skyHeight);
}
globjects::ref_ptr<globjects::Texture> AtmospherePrecompute::getInscatterTexture()
{
    return setupTexture3D("inscatter", GL_RGBA16F_ARB, GL_RGBA, GL_FLOAT
        , getTextureConfig().resMuS * getTextureConfig().resNu, getTextureConfig().resMu, getTextureConfig().resR);
}


const bool AtmospherePrecompute::compute(const bool ifDirtyOnly)
{
    osg::Timer_t t = osg::Timer::instance()->tick();

    // Setup Viewer

    osgViewer::CompositeViewer *viewer = new osgViewer::CompositeViewer;
    osgViewer::View *view = new osgViewer::View();

    viewer->addView(view);

    // Setup Context and Camera

    osg::GraphicsContext *gc(setupContext());

    if(!gc->valid())
    {
        OSG_FATAL << "Initialize PBuffer graphics context failed" << std::endl;
        return false;
    }


    osg::Camera *camera = view->getCamera();
    camera->setGraphicsContext(gc);
    camera->setClearColor(glm::vec4(0.f, 0.f, 0.f, 0.f));
    camera->setViewport(0, 0, 1, 1);

    osg::Group *group(new osg::Group);
    view->setSceneData(group);

    osg::Geode *quad = genQuad();

    osg::ref_ptr<osg::Uniform> u_common = ComputedHimmel::cmnUniform();
    group->getOrCreateStateSet()->addUniform(u_common);


    // Render

    t_tex2DsByUnit targets2D, samplers2D;
    t_tex3DsByUnit targets3D, samplers3D;

    m_transmittanceTexture->bindActive(0);
    m_deltaETexture->bindActive(1);
    m_deltaSMTexture->bindActive(2);
    m_deltaSRTexture->bindActive(3);
    m_deltaJTexture->bindActive(4);
    m_irradianceTexture->bindActive(5);
    m_inscatterTexture->bindActive(6);

    // computes transmittance texture T (line 1 in algorithm 4.1)
        
    targets2D[0]  = m_transmittanceTexture;

    render2D(viewer, quad, targets2D, samplers2D, samplers3D, uniforms, glsl_bruneton_f_transmittance().c_str());

    // computes irradiance texture deltaE (line 2 in algorithm 4.1)

    targets2D[0]  = m_deltaETexture;
    m_brunetonIrradianceProgram->setUniform("transmittanceSampler", 0);

    render2D(viewer, quad, targets2D, samplers2D, samplers3D, uniforms, glsl_bruneton_f_irradiance1().c_str());

    // computes single scattering texture deltaS (line 3 in algorithm 4.1)

    targets3D[0]  = m_deltaSRTexture;
    targets3D[1]  = m_deltaSMTexture;
    m_brunetonInscatterProgram->setUniform("transmittanceSampler", 0);

    render3D(viewer, quad, targets3D, samplers2D, samplers3D, uniforms, glsl_bruneton_f_inscatter1().c_str());

    // copies deltaE into irradiance texture E (line 4 in algorithm 4.1)

    // THIS PATH SEEMS UNREQUIRED - since k = 0 nothing gets copied? At least it would zero the texture...

    targets2D[0]  = m_irradianceTexture;
    m_brunetonCopyIrradianceProgram->setUniform("deltaESampler", 1);
    m_brunetonCopyIrradianceProgram->setUniform("irradianceSampler", 5);
    uniforms.push_back(new osg::Uniform("k", 0.f));

    render2D(viewer, quad, targets2D, samplers2D, samplers3D, uniforms, glsl_bruneton_f_copyIrradiance().c_str());

    // copies deltaS into inscatter texture S (line 5 in algorithm 4.1)

    targets3D[0]  = m_inscatterTexture;
    samplers3D[0] = m_deltaSRTexture;
    samplers3D[1] = m_deltaSMTexture;
    m_brunetonInscatterProgram->setUniform("deltaSRSampler", 3);
    m_brunetonInscatterProgram->setUniform("deltaSMTexture", 2);

    render3D(viewer, quad, targets3D, samplers2D, samplers3D, uniforms, glsl_bruneton_f_copyInscatter1().c_str());
     
    // loop for each scattering order (line 6 in algorithm 4.1)

    for(int order = 2; order <= 4; ++order)
    {
        const float first = order == 2 ? 1.f : 0.f;

        //// computes deltaJ (line 7 in algorithm 4.1)

        targets3D[0]  = m_deltaJTexture;
        m_brunetonInscatterProgram->setUniform("deltaESampler", 1);
        m_brunetonInscatterProgram->setUniform("deltaSRSampler", 3);
        m_brunetonInscatterProgram->setUniform("deltaSMSampler", 2);
        m_brunetonInscatterProgram->setUniform("transmittanceSampler", 0);
        uniforms.push_back(new osg::Uniform("first", first));

        render3D(viewer, quad, targets3D, samplers2D, samplers3D, uniforms, glsl_bruneton_f_inscatterS().c_str());

        // computes deltaE (line 8 in algorithm 4.1)

        targets2D[0]  = m_deltaETexture;
        m_brunetonIrradianceProgramN->setUniform("transmittanceSampler", 0);
        m_brunetonIrradianceProgramN->setUniform("deltaSRSampler", 3);
        m_brunetonIrradianceProgramN->setUniform("deltaSMTexture", 2);
        uniforms.push_back(new osg::Uniform("first", first));

        render2D(viewer, quad, targets2D, samplers2D, samplers3D, uniforms, glsl_bruneton_f_irradianceN().c_str());

        // computes deltaS (line 9 in algorithm 4.1)

        targets3D[0]  = m_deltaSRTexture;

        m_brunetonInscatterProgramN->setUniform("transmittanceSampler", 0);
        m_brunetonInscatterProgramN->setUniform("deltaJSampler", 4);

        uniforms.push_back(new osg::Uniform("first", first));

        render3D(viewer, quad, targets3D, samplers2D, samplers3D, uniforms, glsl_bruneton_f_inscatterN().c_str());


        // NOTE: http://www.opengl.org/wiki/GLSL_:_common_mistakes#Sampling_and_Rendering_to_the_Same_Texture

        // adds deltaE into irradiance texture E (line 10 in algorithm 4.1)

        targets2D[0]  = m_irradianceTexture;

        m_brunetonCopyIrradianceProgram->setUniform("deltaESampler", 1);
        m_brunetonCopyIrradianceProgram->setUniform("irradianceSampler", 5);

        uniforms.push_back(new osg::Uniform("k", 1.f));

        render2D(viewer, quad, targets2D, samplers2D, samplers3D, uniforms, glsl_bruneton_f_copyIrradiance().c_str());

        // adds deltaS into inscatter texture S (line 11 in algorithm 4.1)
      
        targets3D[0]  = m_inscatterTexture;
          
        m_brunetonCopyInscatterProgramN->setUniform("inscatterSampler", 6);
        m_brunetonCopyInscatterProgramN->setUniform("deltaSRSampler", 3);

        render3D(viewer, quad, targets3D, samplers2D, samplers3D, uniforms, glsl_bruneton_f_copyInscatterN().c_str());
    }

    // Unref

    delete viewer;

    OSG_NOTICE << "Atmopshere Precomputed (took " 
        << osg::Timer::instance()->delta_s(t,  osg::Timer::instance()->tick()) << " s)" << std::endl;

    return true;
}


globjects::ref_ptr<globjects::Texture> AtmospherePrecompute::getDeltaETexture()
{
    return setupTexture2D("deltaE", GL_RGB16F_ARB, GL_RGB, GL_FLOAT
        , getTextureConfig().skyWidth, getTextureConfig().skyHeight);
}
globjects::ref_ptr<globjects::Texture> AtmospherePrecompute::getDeltaSRTexture()
{
    return setupTexture3D("deltaSR", GL_RGB16F_ARB, GL_RGB, GL_FLOAT
        , getTextureConfig().resMuS * getTextureConfig().resNu, getTextureConfig().resMu, getTextureConfig().resR);
}
globjects::ref_ptr<globjects::Texture> AtmospherePrecompute::getDeltaSMTexture()
{
    return setupTexture3D("deltaSM", GL_RGB16F_ARB, GL_RGB, GL_FLOAT
        , getTextureConfig().resMuS * getTextureConfig().resNu, getTextureConfig().resMu, getTextureConfig().resR);
}
globjects::ref_ptr<globjects::Texture> AtmospherePrecompute::getDeltaJTexture()
{
    return setupTexture3D("deltaJ", GL_RGB16F_ARB, GL_RGB, GL_FLOAT
        , getTextureConfig().resMuS * getTextureConfig().resNu, getTextureConfig().resMu, getTextureConfig().resR);
}


osg::GraphicsContext *AtmospherePrecompute::setupContext()
{
    // (pbuffer by http://forum.openscenegraph.org/viewtopic.php?t=9025)

    osg::GraphicsContext::Traits *traits = new osg::GraphicsContext::Traits;

    traits->screenNum = 0;
    traits->x = 0;
    traits->y = 0;
    traits->width = 1;
    traits->height = 1;
    traits->windowDecoration = false;
    traits->doubleBuffer = false;
    traits->sharedContext = NULL;
    traits->pbuffer = true;
    //traits->samples = 16;


    return osg::GraphicsContext::createGraphicsContext(traits);
}


osg::Geode *AtmospherePrecompute::genQuad() const
{
    osg::Geometry *quad(new osg::Geometry);

    osg::Vec3Array *vertices = new osg::Vec3Array();
    vertices->push_back(glm::vec3(-1.f, -1.f, 0.f));
    vertices->push_back(glm::vec3(-1.f,  1.f, 0.f));
    vertices->push_back(glm::vec3( 1.f,  1.f, 0.f));
    vertices->push_back(glm::vec3( 1.f, -1.f, 0.f));

    quad->setVertexArray(vertices);   
    quad->addPrimitiveSet(
        new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));

    osg::Geode *geode(new osg::Geode);
    geode->addDrawable(quad);

    return geode;
}


globjects::ref_ptr<globjects::Texture> AtmospherePrecompute::setupTexture2D(
    const GLenum internalFormat
,   const GLenum pixelFormat
,   const GLenum dataType
,   const int width
,   const int height)
{
    auto texture = globjects::Texture::createDefault();

    texture->image2D(0, internalFormat, width, height, 0, pixelFormat, dataType, nullptr);

    texture->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    texture->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    texture->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return texture;
}


globjects::ref_ptr<globjects::Texture> AtmospherePrecompute::setupTexture3D(
    const GLenum internalFormat
,   const GLenum pixelFormat
,   const GLenum dataType
,   const int width
,   const int height
,   const int depth)
{
    auto texture = globjects::Texture::createDefault(GL_TEXTURE_3D);

    texture->image3D(0, internalFormat, width, height, depth, 0, pixelFormat, dataType, nullptr);

    texture->setParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    texture->setParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texture->setParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    texture->setParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    texture->setParameter(GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return texture;
}
    

// (from Emmanuel Roche e.g. here http://www.mail-archive.com/osg-users@lists.openscenegraph.org/msg42832.html)
// Creates a "view" to a single layer of an 3d image. This is 
// required for rendering into a Texture3D based on its image
// (at least when using one camera per layer...).

osg::Image *AtmospherePrecompute::getLayerFrom3DImage(
    osg::Image *source
,   const int layer)
{
    assert(source);

    osg::Image *image(new osg::Image());

    unsigned char *data = source->data(0, 0, layer);

    image->setImage(source->s(), source->t(), 1
        , source->getInternalTextureFormat()
        , source ->getPixelFormat()
        , source->getDataType()
        , data, osg::Image::NO_DELETE, source->getPacking());

    return image;
}


void AtmospherePrecompute::setupLayerUniforms(
    globjects::ref_ptr<globjects::Program> program
,   const int depth
,   const int layer)
{
    const double Rg = Earth::meanRadius();
    const double Rt = Earth::meanRadius() + Earth::atmosphereThicknessNonUniform();

    const double Rg2 = Rg * Rg;
    const double Rt2 = Rt * Rt;

    double r = layer / (depth - 1.0);
    r *= r;
    r = sqrt(Rg2 + r * (Rt2 - Rg2)) + (layer == 0 ? 0.01 : (layer == getTextureConfig().resR - 1 ? -0.001 : 0.0));

    double dmin  = Rt - r;
    double dmax  = sqrt(r * r - Rg2) + sqrt(Rt2 - Rg2);
    double dminp = r - Rg;
    double dmaxp = sqrt(r * r - Rg2);

    program->setUniform("u_r", static_cast<float>(r));
    
    program->setUniform("u_dhdH", glm::vec4(dmin, dmax, dminp, dmaxp));
}


globjects::ref_ptr<globjects::Program> AtmospherePrecompute::setupProgram(
    const std::string &fragmentShaderPath)
{
    assert(!fragmentShaderPath.empty());

    globjects::ref_ptr<globjects::Program> program(new globjects::Program);
    
    auto vertexShader = globjects::Shader::fromFile(GL_VERTEX_SHADER, "data/shader/bruneton/bruneton.vert");
    auto fragmentShader = globjects::Shader::fromFile(GL_FRAGMENT_SHADER, fragmentShaderPath);
    
    program->attach(vertexShader);
    program->attach(fragmentShader);

    return program;
}

void AtmospherePrecompute::setUniforms(globjects::ref_ptr<globjects::Program> program)
{
    program->setUniform("u_HM", m_modelCfg.HM);
    program->setUniform("u_HR", m_modelCfg.HR);
    program->setUniform("u_averageGroundReflectance", m_modelCfg.avgGroundReflectance);
    program->setUniform("u_betaMEx", m_modelCfg.betaMEx);
    program->setUniform("u_betaMSca", m_modelCfg.betaMSca);
    program->setUniform("u_betaR", m_modelCfg.betaR);
    program->setUniform("u_mieG", m_modelCfg.mieG);
    
    program->setUniform("u_altitude", ComputedHimmel::defaultAltitude());
    program->setUniform("u_apparentAngularRadius", Earth::meanRadius());
    program->setUniform("u_radiusUpToEndOfAtmosphere", Earth::meanRadius() + Earth::atmosphereThicknessNonUniform());
    program->setUniform("u_seed", 0);
}
    
// Note: The first targets size is used to setup the camera, and 
// it is required that all targets have the same dimensions.
    
void AtmospherePrecompute::render2D(
    std::vector<globjects::ref_ptr<globjects::Texture>> &targets2D
,   globjects::ref_ptr<globjects::Program> program)
{
    assert(targets2D.size() > 0);

    auto t2End = targets2D.end();

    auto i2 = targets2D.begin();
    const int width  = i2->second-> getTextureWidth();
    const int height = i2->second->getTextureHeight();

    for(i2 = targets2D.begin(); i2 != t2End; ++i2)
    {
        assert(i2->second->getTextureWidth()  == width);
        assert(i2->second->getTextureHeight() == height);
    }
        
    globjects::ref_ptr<globjects::Framebuffer> fbo = new globjects::Framebuffer();

    setUniforms(program);

    // Setup local camera

    glViewport(0, 0, width, height);

    // Assign Textures and Samplers
    GLint i = 0;
    for(auto & target : targets2D)
    {
        fbo->attachTexture(GL_COLOR_ATTACHMENT0 + i, target);
    }
    
    fbo->bind(gl::GL_FRAMEBUFFER);
    program->use();
    m_triangle.draw();

    viewer->frame(); // Render single frame
    cleanUp(viewer);
}


void AtmospherePrecompute::render3D(
    std::vector<globjects::ref_ptr<globjects::Texture>> &targets3D
,   globjects::ref_ptr<globjects::Program> program)
{
    assert(targets3D.size() > 0);

    t_tex3DsByUnit::const_iterator i3;
    const t_tex3DsByUnit::const_iterator t3End = targets3D.end();

    i3 = targets3D.begin();
    const int width  = i3->second->getTextureWidth();
    const int height = i3->second->getTextureHeight();
    const int depth  = i3->second->getTextureDepth();

    for(i3 = targets3D.begin(); i3 != t3End; ++i3)
    {
        assert(i3->second->getTextureWidth()  == width);
        assert(i3->second->getTextureHeight() == height);
        assert(i3->second->getTextureDepth()  == depth);
    }

    assignSamplers(ss, samplers2D, samplers3D);
    setUniforms(program);

    // 

    for(int layer = 0; layer < depth; ++layer)
    {
        // Setup local camera

        osg::ref_ptr<osg::Camera> camera = setupCamera(width, height, geode, layer);
        group->addChild(camera.get());

        setupLayerUniforms(camera->getOrCreateStateSet(), depth, layer);

        // Assign Textures and Samplers

        for(i3 = targets3D.begin(); i3 != t3End; ++i3)
        {
            if(i3->second->getImage())
            {
                // workaround: use a slice here instead of the whole image, since osg does not support this directly...
                osg::Image *slice = getLayerFrom3DImage(i3->second->getImage(), layer);
                camera->attach(static_cast<osg::Camera::BufferComponent>(osg::Camera::COLOR_BUFFER0 + i3->first), slice);
            }
            else
                camera->attach(static_cast<osg::Camera::BufferComponent>(osg::Camera::COLOR_BUFFER0 + i3->first), i3->second, 0U, layer);
        }
    }
    viewer->frame(); // Render single frame

    dirtyTargets(targets3D);
    targets3D.clear();
}

} // namespace glHimmel
