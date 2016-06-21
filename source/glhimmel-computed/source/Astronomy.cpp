
#include <glhimmel-computed/Astronomy.h>

#include <glm/trigonometric.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/transform.hpp>
#include <glhimmel-computed/Earth.h>
#include <glhimmel-computed/Sun.h>
#include <glhimmel-computed/Moon.h>
//#include "stars.h"
#include <glhimmel-computed/siderealtime.h>


namespace glHimmel
{

Astronomy::Astronomy()
{
}


float Astronomy::sunDistance(const JulianDay t) const
{
    return Sun::distance(t);
}

float Astronomy::angularSunRadius(const JulianDay t) const
{
    return Earth::apparentAngularSunDiameter(t) * 0.5;
}


float Astronomy::moonRadius() const
{
    return Moon::meanRadius();
}


float Astronomy::moonDistance(const JulianDay t) const
{
    return Moon::distance(t);
}

float Astronomy::angularMoonRadius(const JulianDay t) const
{
    return Earth::apparentAngularMoonDiameter(t) * 0.5;
}


glm::vec3 Astronomy::moonPosition(
    const astronomicalTime &aTime
,   const float latitude
,   const float longitude
,   const bool refractionCorrected) const
{
    HorizontalCoords<long double> moon = Moon::horizontalPosition(aTime, latitude, longitude);
    if(refractionCorrected)
        moon.altitude += Earth::atmosphericRefraction(moon.altitude);

    glm::vec3 moonv  = moon.toEuclidean();
    moonv = glm::normalize(moonv);

    return moonv;
}


glm::vec3 Astronomy::sunPosition(
    const astronomicalTime &aTime
,   const float latitude
,   const float longitude
,   const bool refractionCorrected) const
{
    HorizontalCoords<long double> sun = Sun::horizontalPosition(aTime, latitude, longitude);
    if(refractionCorrected)
        sun.altitude += Earth::atmosphericRefraction(sun.altitude);

    glm::vec3 sunv  = sun.toEuclidean();
    sunv = glm::normalize(sunv);

    return sunv;
}


glm::mat4 Astronomy::moonOrientation(
    const astronomicalTime &aTime
,   const float latitude
,   const float longitude) const
{    
    const JulianDay t(julianDay(aTime));

    long double l, b;
    Moon::opticalLibrations(t, l, b);

    const glm::mat4 libLat = glm::rotate(glm::radians<float>(b), glm::vec3(-1.0, 0.0, 0.0));
    const glm::mat4 libLon = glm::rotate(glm::radians<float>(l), glm::vec3(0.0, 1.0, 0.0));

    const float a = glm::radians(Moon::positionAngleOfAxis(t));
    const float p = glm::radians(Moon::parallacticAngle(aTime, latitude, longitude));

    const glm::mat4 zenith = glm::rotate(p - a, glm::vec3(0.0, 0.0, 1.0));

    // finalOrientationWithLibrations
    const glm::mat4 R(libLat * libLon * zenith);

    return R;
}


float Astronomy::earthShineIntensity(
    const astronomicalTime &aTime
,   const float latitude
,   const float longitude) const
{
    const glm::vec3 moon = moonPosition(aTime, latitude, longitude, false);
    const glm::vec3 sun = sunPosition(aTime, latitude, longitude, false);

    // ("Multiple Light Scattering" - 1980 - Van de Hulst) and 
    // ("A Physically-Based Night Sky Model" - 2001 - Wann Jensen et al.) -> the 0.19 is the earth full intensity
    
    const float ep  = (glm::pi<float>() - acos(glm::dot(sun, -moon))) * 0.5;
    const float Eem = 0.19 * 0.5 * (1.0 - sin(ep) * tan(ep) * log(1.0 / tan(ep * 0.5)));

    return Eem;
}


glm::mat4 Astronomy::equToHorTransform(
    const astronomicalTime &aTime
,   const float latitude
,   const float longitude) const
{
    const JulianDay T(jCenturiesSinceSE(julianDay(aTime)));
    const float s = siderealTime(aTime);

    return glm::scale(glm::tvec3<long double>(-1.0, 1.0, 1.0))
        * glm::rotate( glm::radians<long double>(latitude) - glm::pi<long double>() * 0.5, glm::tvec3<long double>(1, 0, 0) )
        * glm::rotate( -glm::radians<long double>(s + longitude), glm::tvec3<long double>(0.0, 0.0, 1.0) )
        // precession as suggested in (Jensen et al. 2001)
        * glm::rotate(0.01118 * T, glm::tvec3<long double>(0.0, 0.0, 1.0))
        * glm::rotate(-0.00972 * T, glm::tvec3<long double>(1.0, 0.0, 0.0))
        * glm::rotate( 0.01118 * T, glm::tvec3<long double>(0.0, 0.0, 1.0));
}

} // namespace gHimmel