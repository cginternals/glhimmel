#pragma once

#include <glhimmel-computed/glhimmel-computed_api.h>
#include <glhimmel-base/julianday.h>
#include <glhimmel-base/coordinates.h>


namespace glHimmel
{

class GLHIMMEL_COMPUTED_API Sun
{
public:

    static long double meanAnomaly(const JulianDay t);
    static long double meanLongitude(const JulianDay t);

    static long double center(const JulianDay t);

    static long double trueAnomaly(const JulianDay t);
    static long double trueLongitude(const JulianDay t);

    static EquatorialCoords<long double> apparentPosition(const JulianDay t);
    static HorizontalCoords<long double> horizontalPosition(
        const AstronomicalTime &aTime
    ,   const long double latitude
    ,   const long double longitude);

    static long double distance(const JulianDay t);

    static long double meanRadius();
};

} // namespace glHimmel

