#pragma once

#include <glhimmel-computed/glhimmel-computed_api.h>
#include <glm/vec3.hpp>
#include <string>
#include <globjects/Program.h>
#include <glhimmel-base/ScreenAlignedTriangle.h>
#include <glhimmel-computed/AtmospherePrecompute.h>

namespace globjects
{
    class Texture;
}

namespace glHimmel
{
class ComputedHimmel;

class GLHIMMEL_COMPUTED_API AtmosphereDrawable
{
public:

    AtmosphereDrawable();
    virtual ~AtmosphereDrawable();

    void update(const ComputedHimmel &himmel);
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

    void draw() const;

protected:

    void setupProgram();

    void setupTextures();

protected:

    ScreenAlignedTriangle m_screenAlignedTriangle;

    AtmospherePrecompute m_precompute;

    globjects::ref_ptr<globjects::Program> m_program;

    globjects::ref_ptr<globjects::Texture> m_transmittance;
    globjects::ref_ptr<globjects::Texture> m_irradiance;
    globjects::ref_ptr<globjects::Texture> m_inscatter;

    float m_sunScaleFactor;
    float m_exposure;
    glm::vec3 m_lheurebleueColor;
    float m_lheurebleueIntensity;

    float m_scale;

};

} // namespace glHimmel

