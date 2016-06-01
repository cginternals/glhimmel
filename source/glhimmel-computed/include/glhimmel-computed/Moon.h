#pragma once

#include <glhimmel-computed/glhimmel-computed_api.h>
#include <glhimmel-base/julianday.h>
#include <glhimmel-base/coordinates.h>


namespace glHimmel
{

class GLHIMMEL_COMPUTED_API Moon
{
public:
    static const long double meanLongitude(const JulianDay t);
    static const long double meanElongation(const JulianDay t);
    static const long double meanAnomaly(const JulianDay t);
    static const long double meanLatitude(const JulianDay t);

    static const long double meanOrbitLongitude(const JulianDay t);

    static const EclipticalCoords<long double> position(const JulianDay t);
    static const EquatorialCoords<long double> apparentPosition(const JulianDay t);

    static const HorizontalCoords<long double> horizontalPosition(
        const AstronomicalTime &aTime
    ,   const long double latitude
    ,   const long double longitude);

    static const long double distance(const JulianDay t);

    static void opticalLibrations(
        const JulianDay t
    ,   long double &l /* librations in longitude */
    ,   long double &b /* librations in latitude  */);

    static const long double parallacticAngle(
        const AstronomicalTime &aTime
    ,   const long double latitude
    ,   const long double longitude);

    static const long double positionAngleOfAxis(const JulianDay t);

    static const long double meanRadius();
};

} // namespace glHimmel

