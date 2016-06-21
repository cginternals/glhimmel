#pragma once

#include <glhimmel-computed/glhimmel-computed_api.h>
#include <glhimmel-base/julianday.h>


namespace glHimmel
{

class GLHIMMEL_COMPUTED_API Earth
{
public:

    static long double orbitEccentricity(const JulianDay t);

    static long double apparentAngularSunDiameter(const JulianDay t);
    static long double apparentAngularMoonDiameter(const JulianDay t);

    static long double longitudeNutation(const JulianDay t);
    static long double obliquityNutation(const JulianDay t);

    static long double meanObliquity(const JulianDay t);
    static long double trueObliquity(const JulianDay t);

    static long double atmosphericRefraction(const long double altitude);

    static long double viewDistanceWithinAtmosphere(
        const long double y /* height component of the view direction on ground into the sky */
    ,   const bool refractionCorrected = false);

    static long double meanRadius();
    static long double atmosphereThickness(); // if its density were uniform...
    static long double atmosphereThicknessNonUniform();

    static long double apparentMagnitudeLimit();
};

} // namespace glHimmel
