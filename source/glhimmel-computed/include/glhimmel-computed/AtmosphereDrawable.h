#pragma once

#include <glhimmel-computed/glhimmel-computed_api.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <string>
#include <globjects/Program.h>
#include <glhimmel-base/ScreenAlignedTriangle.h>
#include <glhimmel-computed/AtmospherePrecompute.h>

namespace osg
{
    class Texture2D;
    class Texture3D;
}


namespace glHimmel
{
class Himmel;

class GLHIMMEL_COMPUTED_API AtmosphereGeode
{
public:

    AtmosphereGeode();
    virtual ~AtmosphereGeode();

    void update(const Himmel &himmel);
    //void precompute();

    const float setSunScale(const float scale);
    const float getSunScale() const;
    static const float defaultSunScale();

    const float setExposure(const float exposure);
    const float getExposure() const;
    static const float defaultExposure();

    const glm::vec3 setLHeureBleueColor(const glm::vec3 &color);
    const glm::vec3 getLHeureBleueColor() const;
    static const glm::vec3 defaultLHeureBleueColor();
    
    const float setLHeureBleueIntensity(const float intensity);
    const float getLHeureBleueIntensity() const;
    static const float defaultLHeureBleueIntensity();


    void setAverageGroundReflectance(const float reflectance); // [0;1]
    void setThicknessRayleigh(const float thickness); 
    void setScatteringRayleigh(const glm::vec3 &coefficients); 
    void setThicknessMie(const float thickness); 
    void setScatteringMie(const float coefficient); 
    void setPhaseG(const float g);  // [-1;+1]

protected:

    void precompute();

    void setupUniforms();
    void setupTextures();
    void setupShader();
    void updateShader();

    const std::string getVertexShaderSource();
    const std::string getFragmentShaderSource();

protected:

    ScreenAlignedTriangle m_screenAlignedTriangle;

    AtmospherePrecompute m_precompute;

    osg::Texture2D *m_transmittance;
    osg::Texture2D *m_irradiance;
    osg::Texture3D *m_inscatter;

    globjects::ref_ptr<globjects::Program> m_program;

    float m_sunScale;
    float m_exposure;
    glm::vec4 m_lheurebleue;

    float m_scale;

};

} // namespace glHimmel

