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

class GLHIMMEL_COMPUTED_API AtmosphereDrawable
{
public:

    AtmosphereDrawable();
    virtual ~AtmosphereDrawable();

    void update(const Himmel &himmel);
    //void precompute();

    void setSunScaleFactor(const float scale);
    float getSunScaleFactor() const;
    static float defaultSunScaleFactor();

    void setExposure(const float exposure);
    float getExposure() const;
    static float defaultExposure();

    void setLHeureBleueColor(const glm::vec3 &color);
    glm::vec3 getLHeureBleueColor() const;
    static glm::vec3 defaultLHeureBleueColor();
    
    void setLHeureBleueIntensity(const float intensity);
    float getLHeureBleueIntensity() const;
    static float defaultLHeureBleueIntensity();


    void setAverageGroundReflectance(const float reflectance); // [0;1]
    void setThicknessRayleigh(const float thickness); 
    void setScatteringRayleigh(const glm::vec3 &coefficients); 
    void setThicknessMie(const float thickness); 
    void setScatteringMie(const float coefficient); 
    void setPhaseG(const float g);  // [-1;+1]

    void draw();

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

    globjects::ref_ptr<globjects::Program> m_program;

    osg::Texture2D *m_transmittance;
    osg::Texture2D *m_irradiance;
    osg::Texture3D *m_inscatter;

    float m_sunScaleFactor;
    float m_exposure;
    glm::vec3 m_lheurebleueColor;
    float m_lheurebleueIntensity;

    float m_scale;

};

} // namespace glHimmel

