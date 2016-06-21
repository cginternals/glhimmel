
#include <glhimmel-computed/AbstractAstronomy.h>

#include <glm/common.hpp>


namespace glHimmel
{

AbstractAstronomy::AbstractAstronomy()
:   m_time(0.0)
,   m_latitude(0.f)
,   m_longitude(0.f)
{
}


AbstractAstronomy::~AbstractAstronomy()
{
}


void AbstractAstronomy::update(const astronomicalTime &aTime)
{
    m_astronomicalTime = aTime;
    m_time = julianDay(aTime);
}

const astronomicalTime & AbstractAstronomy::getAstronomicalTime() const
{
    return m_astronomicalTime;
}


float AbstractAstronomy::setLatitude(const float latitude)
{
    if(latitude != m_latitude)
        m_latitude = glm::clamp(latitude, -90.f, +90.f);

    return getLatitude();
}

float AbstractAstronomy::getLatitude() const
{
    return m_latitude;
}


float AbstractAstronomy::setLongitude(const float longitude)
{
    if(longitude != m_longitude)
        m_longitude = glm::clamp(longitude, -180.f, +180.f);

    return getLongitude();
}

float AbstractAstronomy::getLongitude() const
{
    return m_longitude;
}


glm::mat4 AbstractAstronomy::getMoonOrientation() const
{
    return moonOrientation(getAstronomicalTime(), getLatitude(), getLongitude());
}

glm::mat4 AbstractAstronomy::getMoonOrientation(
    const astronomicalTime &aTime
,   const float latitude
,   const float longitude) const
{
    return moonOrientation(aTime, latitude, longitude);
}


glm::vec3 AbstractAstronomy::getMoonPosition(
    const bool refractionCorrected) const
{
    return moonPosition(getAstronomicalTime(), getLatitude(), getLongitude(), refractionCorrected);
}

glm::vec3 AbstractAstronomy::getMoonPosition(
    const astronomicalTime &aTime
,   const float latitude
,   const float longitude
,   const bool refractionCorrected) const
{
    return moonPosition(aTime, latitude, longitude, refractionCorrected);
}


glm::vec3 AbstractAstronomy::getSunPosition(
    const bool refractionCorrected) const
{
    return sunPosition(getAstronomicalTime(), getLatitude(), getLongitude(), refractionCorrected);
}

glm::vec3 AbstractAstronomy::getSunPosition(
    const astronomicalTime &aTime
,   const float latitude
,   const float longitude
,   const bool refractionCorrected) const
{
    return sunPosition(aTime, latitude, longitude, refractionCorrected);
}


float AbstractAstronomy::getEarthShineIntensity() const
{
    return earthShineIntensity(getAstronomicalTime(), getLatitude(), getLongitude());
}

float AbstractAstronomy::getEarthShineIntensity(
    const astronomicalTime &aTime
,   const float latitude
,   const float longitude) const
{
    return earthShineIntensity(aTime, latitude, longitude);
}


float AbstractAstronomy::getSunDistance() const
{
    return sunDistance(time());
}

float AbstractAstronomy::getSunDistance(const astronomicalTime &aTime) const
{
    return sunDistance(julianDay(aTime));
}


float AbstractAstronomy::getAngularSunRadius() const
{
    return angularSunRadius(time());
}

float AbstractAstronomy::getAngularSunRadius(const astronomicalTime &aTime) const
{
    return angularSunRadius(julianDay(aTime));
}


float AbstractAstronomy::getMoonDistance() const
{
    return moonDistance(time());
}

float AbstractAstronomy::getMoonDistance(const astronomicalTime &aTime) const
{
    return moonDistance(julianDay(aTime));
}


float AbstractAstronomy::getMoonRadius() const
{
    return moonRadius();
}


float AbstractAstronomy::getAngularMoonRadius() const
{
    return angularMoonRadius(time());
}

float AbstractAstronomy::getAngularMoonRadius(const astronomicalTime &aTime) const
{
    return angularMoonRadius(julianDay(aTime));
}


glm::mat4 AbstractAstronomy::getEquToHorTransform() const
{
    return equToHorTransform(getAstronomicalTime(), getLatitude(), getLongitude());
}
 
glm::mat4 AbstractAstronomy::getEquToHorTransform(
    const astronomicalTime &aTime
,   const float latitude
,   const float longitude) const
{
    return equToHorTransform(aTime, latitude, longitude);
}

JulianDay AbstractAstronomy::time() const
{
    return m_time;
}

} // namespace glHimmel