#pragma once

#include <glhimmel-computed/glhimmel-computed_api.h>
#include <glhimmel-base/julianday.h>
#include <glhimmel-base/coordinates.h>


namespace glHimmel
{

class GLHIMMEL_COMPUTED_API Sun
{
public:

    static const long double meanAnomaly(const JulianDay t);
    static const long double meanLongitude(const JulianDay t);

    static const long double center(const JulianDay t);

    static const long double trueAnomaly(const JulianDay t);
    static const long double trueLongitude(const JulianDay t);

    static const EquatorialCoords<long double> apparentPosition(const JulianDay t);
    static const HorizontalCoords<long double> horizontalPosition(
        const AstronomicalTime &aTime
    ,   const long double latitude
    ,   const long double longitude);

    static const long double distance(const JulianDay t);

    static const long double meanRadius();
};

} // namespace glHimmel

