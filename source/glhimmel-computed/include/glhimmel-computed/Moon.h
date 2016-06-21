#pragma once

#include <glhimmel-computed/glhimmel-computed_api.h>
#include <glhimmel-base/julianday.h>
#include <glhimmel-base/coordinates.h>


namespace glHimmel
{

class GLHIMMEL_COMPUTED_API Moon
{
public:
    static long double meanLongitude(const JulianDay t);
    static long double meanElongation(const JulianDay t);
    static long double meanAnomaly(const JulianDay t);
    static long double meanLatitude(const JulianDay t);

    static long double meanOrbitLongitude(const JulianDay t);

    static EclipticalCoords<long double> position(const JulianDay t);
    static EquatorialCoords<long double> apparentPosition(const JulianDay t);

    static HorizontalCoords<long double> horizontalPosition(
        const AstronomicalTime &aTime
    ,   const long double latitude
    ,   const long double longitude);

    static long double distance(const JulianDay t);

    static void opticalLibrations(
        const JulianDay t
    ,   long double &l /* librations in longitude */
    ,   long double &b /* librations in latitude  */);

    static long double parallacticAngle(
        const AstronomicalTime &aTime
    ,   const long double latitude
    ,   const long double longitude);

    static long double positionAngleOfAxis(const JulianDay t);

    static long double meanRadius();
};

} // namespace glHimmel

