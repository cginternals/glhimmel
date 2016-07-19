 #include <glhimmel-computed/AtmospherePrecompute.h>

#include <glhimmel-computed/ComputedHimmel.h>
#include <glhimmel-computed/Earth.h>

#include <cassert>
#include <cmath>
#include <globjects/Texture.h>
#include <globjects/Shader.h>
#include <globjects/Framebuffer.h>

#include <chrono>
#include <iostream>
#include <globjects/base/File.h>
#include <globjects/NamedString.h>

namespace
{
    int getTextureWidth(globjects::ref_ptr<globjects::Texture> texture)
    {
        int width;
        texture->bind();
        glGetTexLevelParameteriv(texture->target(), 0, GL_TEXTURE_WIDTH, &width);
        return width;
    }

    int getTextureHeight(globjects::ref_ptr<globjects::Texture> texture)
    {
        int height;
        texture->bind();
        glGetTexLevelParameteriv(texture->target(), 0, GL_TEXTURE_HEIGHT, &height);
        return height;
    }

    int getTextureDepth(globjects::ref_ptr<globjects::Texture> texture)
    {
        int depth;
        texture->bind();
        glGetTexLevelParameteriv(GL_TEXTURE_3D, 0, GL_TEXTURE_DEPTH, &depth);
        return depth;
    }

}

namespace glHimmel
{
AtmospherePrecompute::AtmospherePrecompute()
:   m_altitude(ComputedHimmel::defaultAltitude())
,   m_seed(0.f)
{
    m_preTextureConfig.transmittanceWidth  = 256;
    m_preTextureConfig.transmittanceHeight =  64;

    m_preTextureConfig.skyWidth  =  64;
    m_preTextureConfig.skyHeight =  16;

    m_preTextureConfig.resR   =  32;
    m_preTextureConfig.resMu  = 128;
    m_preTextureConfig.resMuS =  32;
    m_preTextureConfig.resNu  =   8;

    m_preTextureConfig.transmittanceIntegralSamples      = 500;
    m_preTextureConfig.inscatterIntegralSamples          =  50;
    m_preTextureConfig.irradianceIntegralSamples         =  32;
    m_preTextureConfig.inscatterSphericalIntegralSamples =  16;

    m_modelConfig.avgGroundReflectance = 0.1f;

    m_modelConfig.HR = 8.f;
    m_modelConfig.betaR = glm::vec3(5.8e-3, 1.35e-2, 3.31e-2);

    m_modelConfig.HM = 6.f; //1.2f;
    m_modelConfig.betaMSca = glm::vec3(1.0, 1.0, 1.0) * 20e-3f, //8e-3;
    m_modelConfig.betaMEx = m_modelConfig.betaMSca / 0.9f;
    m_modelConfig.mieG = 0.6; //0.76;


    // Setup Textures
    m_transmittanceTexture = setupTexture2D(GL_RGB16F_ARB, GL_RGB, GL_FLOAT, getTextureConfig().transmittanceWidth, getTextureConfig().transmittanceHeight);
    m_deltaETexture        = setupTexture2D(GL_RGB16F_ARB, GL_RGB, GL_FLOAT, getTextureConfig().skyWidth, getTextureConfig().skyHeight);
    m_deltaSRTexture       = setupTexture3D(GL_RGB16F_ARB, GL_RGB, GL_FLOAT, getTextureConfig().resMuS * getTextureConfig().resNu, getTextureConfig().resMu, getTextureConfig().resR);
    m_deltaSMTexture       = setupTexture3D(GL_RGB16F_ARB, GL_RGB, GL_FLOAT, getTextureConfig().resMuS * getTextureConfig().resNu, getTextureConfig().resMu, getTextureConfig().resR);
    m_irradianceTexture    = setupTexture2D(GL_RGB16F_ARB, GL_RGB, GL_FLOAT, getTextureConfig().skyWidth, getTextureConfig().skyHeight);
    m_inscatterTexture     = setupTexture3D(GL_RGBA16F_ARB, GL_RGBA, GL_FLOAT, getTextureConfig().resMuS * getTextureConfig().resNu, getTextureConfig().resMu, getTextureConfig().resR);
    m_deltaJTexture        = setupTexture3D(GL_RGB16F_ARB, GL_RGB, GL_FLOAT, getTextureConfig().resMuS * getTextureConfig().resNu, getTextureConfig().resMu, getTextureConfig().resR);

    fetchShaderIncludes();

    // Setup Program
    m_copyInscatter1Program = setupProgram("data/shader/bruneton/copyInscatter1.frag");
    m_copyInscatterNProgram = setupProgram("data/shader/bruneton/copyInscatterN.frag");
    m_copyIrradianceProgram = setupProgram("data/shader/bruneton/copyIrradiance.frag");
    m_inscatter1Program = setupProgram("data/shader/bruneton/inscatter1.frag");
    m_inscatterNProgram = setupProgram("data/shader/bruneton/inscatterN.frag");
    m_inscatterSProgram = setupProgram("data/shader/bruneton/inscatterS.frag");
    m_irradiance1Program = setupProgram("data/shader/bruneton/irradiance1.frag");
    m_irradianceNProgram = setupProgram("data/shader/bruneton/irradianceN.frag");
    m_transmittanceProgram = setupProgram("data/shader/bruneton/transmittance.frag");
}

AtmospherePrecompute::~AtmospherePrecompute()
{
}

AtmospherePrecompute::PhysicalModelConfig& AtmospherePrecompute::getModelConfig()
{
    return m_modelConfig;
}

void AtmospherePrecompute::setAltitude(float altitude)
{
    m_altitude = altitude;
}

void AtmospherePrecompute::setSeed(float seed)
{
    m_seed = seed;
}

AtmospherePrecompute::PrecomputedTextureConfig& AtmospherePrecompute::getTextureConfig()
{
    return m_preTextureConfig;
}

globjects::ref_ptr<globjects::Texture> AtmospherePrecompute::getTransmittanceTexture() const
{
    return m_transmittanceTexture;
}

globjects::ref_ptr<globjects::Texture> AtmospherePrecompute::getIrradianceTexture() const
{
    return m_irradianceTexture;
}

globjects::ref_ptr<globjects::Texture> AtmospherePrecompute::getInscatterTexture() const
{
    return m_inscatterTexture;
}

void AtmospherePrecompute::compute()
{
    auto startTime = std::chrono::steady_clock::now();
    glClearColor(0.f, 0.f, 0.f, 0.f);
    std::vector<globjects::ref_ptr<globjects::Texture>> targets;

    m_transmittanceTexture->bindActive(0);
    m_deltaETexture->bindActive(1);
    m_deltaSMTexture->bindActive(2);
    m_deltaSRTexture->bindActive(3);
    m_deltaJTexture->bindActive(4);
    m_irradianceTexture->bindActive(5);
    m_inscatterTexture->bindActive(6);

    // computes transmittance texture T (line 1 in algorithm 4.1)
    targets = { m_transmittanceTexture };
    render2D(targets, m_transmittanceProgram);

    // computes irradiance texture deltaE (line 2 in algorithm 4.1)
    targets = { m_deltaETexture };
    m_irradiance1Program->setUniform("transmittanceSampler", 0);
    render2D(targets, m_irradiance1Program);

    // computes single scattering texture deltaS (line 3 in algorithm 4.1)
    targets = { m_deltaSRTexture, m_deltaSMTexture };
    m_inscatter1Program->setUniform("transmittanceSampler", 0);
    render3D(targets, m_copyInscatter1Program);

    // copies deltaE into irradiance texture E (line 4 in algorithm 4.1)
    // THIS PATH SEEMS UNREQUIRED - since k = 0 nothing gets copied? At least it would zero the texture...
    targets = { m_irradianceTexture };
    m_copyIrradianceProgram->setUniform("deltaESampler", 1);
    m_copyIrradianceProgram->setUniform("irradianceSampler", 5);
    m_copyIrradianceProgram->setUniform("k", 0.f);
    render2D(targets, m_copyIrradianceProgram);

    // copies deltaS into inscatter texture S (line 5 in algorithm 4.1)
    targets = { m_inscatterTexture };
    m_copyInscatter1Program->setUniform("deltaSRSampler", 3);
    m_copyInscatter1Program->setUniform("deltaSMTexture", 2);
    render3D(targets, m_copyInscatter1Program);

    // loop for each scattering order (line 6 in algorithm 4.1)
    for(int order = 2; order <= 4; ++order)
    {
        const float first = order == 2 ? 1.f : 0.f;

        // computes deltaJ (line 7 in algorithm 4.1)
        targets = { m_deltaJTexture };
        m_inscatterSProgram->setUniform("deltaESampler", 1);
        m_inscatterSProgram->setUniform("deltaSRSampler", 3);
        m_inscatterSProgram->setUniform("deltaSMSampler", 2);
        m_inscatterSProgram->setUniform("transmittanceSampler", 0);
        m_inscatterSProgram->setUniform("first", first);
        render3D(targets, m_inscatterSProgram);

        // computes deltaE (line 8 in algorithm 4.1)
        targets = { m_deltaETexture };
        m_irradianceNProgram->setUniform("transmittanceSampler", 0);
        m_irradianceNProgram->setUniform("deltaSRSampler", 3);
        m_irradianceNProgram->setUniform("deltaSMTexture", 2);
        m_irradianceNProgram->setUniform("first", first);
        render2D(targets, m_irradianceNProgram);

        // computes deltaS (line 9 in algorithm 4.1)
        targets = { m_deltaSRTexture };
        m_inscatterNProgram->setUniform("transmittanceSampler", 0);
        m_inscatterNProgram->setUniform("deltaJSampler", 4);
        m_inscatterNProgram->setUniform("first", first);
        render3D(targets, m_inscatterNProgram);


        // NOTE: http://www.opengl.org/wiki/GLSL_:_common_mistakes#Sampling_and_Rendering_to_the_Same_Texture
        // adds deltaE into irradiance texture E (line 10 in algorithm 4.1)
        targets = { m_irradianceTexture };
        m_copyIrradianceProgram->setUniform("deltaESampler", 1);
        m_copyIrradianceProgram->setUniform("irradianceSampler", 5);
        m_copyIrradianceProgram->setUniform("k", 1.f);
        render2D(targets, m_copyIrradianceProgram);

        // adds deltaS into inscatter texture S (line 11 in algorithm 4.1)
        targets = { m_inscatterTexture };
        m_copyInscatterNProgram->setUniform("inscatterSampler", 6);
        m_copyInscatterNProgram->setUniform("deltaSRSampler", 3);
        render3D(targets, m_copyInscatterNProgram);
    }

    auto endTime = std::chrono::steady_clock::now();
    std::chrono::duration<double> duration = endTime - startTime;
    //std::cout << "Atmopshere Precomputed (took " << duration.count() << " s)" << std::endl;
}

globjects::ref_ptr<globjects::Texture> AtmospherePrecompute::setupTexture2D(
    const GLenum internalFormat
,   const GLenum pixelFormat
,   const GLenum dataType
,   const int width
,   const int height) const
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
,   const int depth) const
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
    r = std::sqrt(Rg2 + r * (Rt2 - Rg2)) + (layer == 0 ? 0.01 : (layer == getTextureConfig().resR - 1 ? -0.001 : 0.0));

    double dmin  = Rt - r;
    double dmax  = std::sqrt(r * r - Rg2) + std::sqrt(Rt2 - Rg2);
    double dminp = r - Rg;
    double dmaxp = std::sqrt(r * r - Rg2);

    program->setUniform("u_r", static_cast<float>(r));

    program->setUniform("u_dhdH", glm::vec4(dmin, dmax, dminp, dmaxp));
}

void AtmospherePrecompute::fetchShaderIncludes() const
{
    std::vector<std::string> fileNames = {
        "analyticTransmittance", "constants", "irradiance", "irradianceRMuS",
        "limit", "muMuSNu", "phaseFunction", "texture4D", "transmittance",
        "uniforms"
    };
    for(auto & fileName : fileNames)
    {
        globjects::NamedString::create("/data/shader/bruneton/include/" + fileName + ".glsl",
            new globjects::File("data/shader/bruneton/include/" + fileName + ".glsl", false));

    }
}


globjects::ref_ptr<globjects::Program> AtmospherePrecompute::setupProgram(
    const std::string &fragmentShaderPath) const
{
    assert(!fragmentShaderPath.empty());

    globjects::ref_ptr<globjects::Program> program(new globjects::Program);

    auto vertexShader = globjects::Shader::fromFile(GL_VERTEX_SHADER, "data/shader/bruneton/bruneton.vert");
    auto fragmentShader = globjects::Shader::fromFile(GL_FRAGMENT_SHADER, fragmentShaderPath);

    program->attach(vertexShader);
    program->attach(fragmentShader);

    return program;
}

void AtmospherePrecompute::setUniforms(globjects::ref_ptr<globjects::Program> program) const
{
    program->setUniform("u_HM", m_modelConfig.HM);
    program->setUniform("u_HR", m_modelConfig.HR);
    program->setUniform("u_averageGroundReflectance", m_modelConfig.avgGroundReflectance);
    program->setUniform("u_betaMEx", m_modelConfig.betaMEx);
    program->setUniform("u_betaMSca", m_modelConfig.betaMSca);
    program->setUniform("u_betaR", m_modelConfig.betaR);
    program->setUniform("u_mieG", m_modelConfig.mieG);

    program->setUniform("u_altitude", m_altitude);
    program->setUniform("u_apparentAngularRadius", static_cast<float>(Earth::meanRadius()));
    program->setUniform("u_radiusUpToEndOfAtmosphere", static_cast<float>(Earth::meanRadius() + Earth::atmosphereThicknessNonUniform()));
    program->setUniform("u_seed", m_seed);
}

// Note: The first targets size is used to setup the camera, and
// it is required that all targets have the same dimensions.

void AtmospherePrecompute::render2D(
    std::vector<globjects::ref_ptr<globjects::Texture>> &targets2D
,   globjects::ref_ptr<globjects::Program> program) const
{
    assert(targets2D.size() > 0);

    const int width  = getTextureWidth(targets2D.front());
    const int height = getTextureHeight(targets2D.front());

    for(auto& target : targets2D)
    {
        assert(getTextureWidth(target)  == width);
        assert(getTextureHeight(target) == height);
    }

    globjects::ref_ptr<globjects::Framebuffer> fbo = new globjects::Framebuffer();

    setUniforms(program);

    glViewport(0, 0, width, height);

    // Assign Textures and Samplers
    GLint i = 0;
    for(auto & target : targets2D)
    {
        fbo->attachTexture(GL_COLOR_ATTACHMENT0 + i, target);
    }

    fbo->bind(gl::GL_FRAMEBUFFER);
    program->use();
    m_screenAlignedTriangle.draw();

    fbo->unbind();
}


void AtmospherePrecompute::render3D(
    std::vector<globjects::ref_ptr<globjects::Texture>> &targets3D
,   globjects::ref_ptr<globjects::Program> program)
{
    assert(targets3D.size() > 0);

    const int width  = getTextureWidth(targets3D.front());
    const int height = getTextureHeight(targets3D.front());
    const int depth  = getTextureDepth(targets3D.front());

    for(auto & target : targets3D)
    {
        assert(getTextureWidth(target)  == width);
        assert(getTextureHeight(target) == height);
        assert(getTextureDepth(target)  == depth);
    }

    globjects::ref_ptr<globjects::Framebuffer> fbo = new globjects::Framebuffer();

    setUniforms(program);

    fbo->bind(gl::GL_FRAMEBUFFER);
    program->use();
    glViewport(0, 0, width, height);

    for(int layer = 0; layer < depth; ++layer)
    {
        setupLayerUniforms(program, depth, layer);

        // Assign Textures and Samplers

        for(auto & target: targets3D)
        {
            glFramebufferTexture3D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_3D, target->id(), 0, layer);
            m_screenAlignedTriangle.draw();
        }
    }
}

} // namespace glHimmel
