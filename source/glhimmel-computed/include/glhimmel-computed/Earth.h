#pragma once

#include <glhimmel-computed/glhimmel-computed_api.h>
#include <glhimmel-base/julianday.h>


namespace glHimmel
{

class GLHIMMEL_COMPUTED_API Earth
{
public:

    static const long double orbitEccentricity(const JulianDay t);

    static const long double apparentAngularSunDiameter(const JulianDay t);
    static const long double apparentAngularMoonDiameter(const JulianDay t);

    static const long double longitudeNutation(const JulianDay t);
    static const long double obliquityNutation(const JulianDay t);

    static const long double meanObliquity(const JulianDay t);
    static const long double trueObliquity(const JulianDay t);

    static const long double atmosphericRefraction(const long double altitude);

    static const long double viewDistanceWithinAtmosphere(
        const long double y /* height component of the view direction on ground into the sky */
    ,   const bool refractionCorrected = false);

    static const long double meanRadius();
    static const long double atmosphereThickness(); // if its density were uniform...
    static const long double atmosphereThicknessNonUniform();

    static const long double apparentMagnitudeLimit();
};

} // namespace glHimmel
