#pragma once

#include <glhimmel-computed/glhimmel-computed_api.h>
#include <glhimmel-base/AstronomicalTime.h>
#include <glhimmel-base/julianday.h>

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>


namespace glHimmel
{

class GLHIMMEL_COMPUTED_API AbstractAstronomy
{
public:

    AbstractAstronomy();
    virtual ~AbstractAstronomy();


    void update(const astronomicalTime &aTime);

    const astronomicalTime& getAstronomicalTime() const;

    float setLatitude(const float latitude);
    float getLatitude() const;

    float setLongitude(const float longitude);
    float getLongitude() const;


    glm::mat4 getMoonOrientation() const;
    glm::mat4 getMoonOrientation(
        const astronomicalTime &aTime
    ,   const float latitude
    ,   const float longitude) const;

    glm::vec3 getMoonPosition(const bool refractionCorrected) const;
    glm::vec3 getMoonPosition(
        const astronomicalTime &aTime
    ,   const float latitude
    ,   const float longitude
    ,   const bool refractionCorrected) const;

    glm::vec3 getSunPosition(const bool refractionCorrected) const;
    glm::vec3 getSunPosition(
        const astronomicalTime &aTime
    ,   const float latitude
    ,   const float longitude
    ,   const bool refractionCorrected) const;

    float getEarthShineIntensity() const;
    float getEarthShineIntensity(
        const astronomicalTime &aTime
    ,   const float latitude
    ,   const float longitude) const;

    float getSunDistance() const;
    float getSunDistance(const astronomicalTime &aTime) const;

    float getAngularSunRadius() const;
    float getAngularSunRadius(const astronomicalTime &aTime) const;

    float getMoonDistance() const;
    float getMoonDistance(const astronomicalTime &aTime) const;

    float getMoonRadius() const;

    float getAngularMoonRadius() const;
    float getAngularMoonRadius(const astronomicalTime &aTime) const;

    glm::mat4 getEquToHorTransform() const;
    glm::mat4 getEquToHorTransform(
        const astronomicalTime &aTime
    ,   const float latitude
    ,   const float longitude) const;


protected:

    virtual glm::vec3 moonPosition(
        const astronomicalTime &aTime
    ,   const float latitude
    ,   const float longitude
    ,   const bool refractionCorrected) const = 0;

    virtual glm::vec3 sunPosition(
        const astronomicalTime &aTime
    ,   const float latitude
    ,   const float longitude
    ,   const bool refractionCorrected) const = 0;

    virtual glm::mat4 moonOrientation(
        const astronomicalTime &aTime
    ,   const float latitude
    ,   const float longitude) const = 0;

    virtual float earthShineIntensity(
        const astronomicalTime &aTime
    ,   const float latitude
    ,   const float longitude) const = 0;

    virtual float sunDistance(const JulianDay t) const = 0;
    virtual float angularSunRadius(const JulianDay t) const = 0;

    virtual float moonRadius() const = 0;
    virtual float moonDistance(const JulianDay t) const = 0;
    virtual float angularMoonRadius(const JulianDay t) const = 0;

    virtual glm::mat4 equToHorTransform(
        const astronomicalTime &aTime
    ,   const float latitude
    ,   const float longitude) const = 0;

    JulianDay time() const;

protected:

    astronomicalTime m_astronomicalTime;
    JulianDay m_time;

    float m_latitude;
    float m_longitude;
};

} // namespace glHimmel
