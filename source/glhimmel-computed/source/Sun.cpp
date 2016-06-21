#include <glhimmel-computed/Sun.h>

#include <glhimmel-computed/Earth.h>
#include <glhimmel-computed/Moon.h>
#include <glhimmel-computed/siderealtime.h>

#include <cassert>
#include <cmath>
#include <glm/trigonometric.hpp>

namespace glHimmel
{

// Mean anomaly (AA.45.3).
long double Sun::meanAnomaly(const JulianDay t)
{
    const JulianDay T(jCenturiesSinceSE(t));

    // seems most accurate... :o
    const long double M = 357.5291092
        + T * (+ 35999.0502909
        + T * (-     0.0001536
        + T * (+ 1.0 / 24490000.0)));

    // AA uses different coefficients all over the book...
    // ...taken the (probably) most accurate above

    // (AA.21 ...)
    //const long double M = 357.52772
    //    + T * (+  35999.050340
    //    + T * (-      0.0001603
    //    + T * (- 1.0 / 300000.0)));

    // (AA.24.3)
    //const long double M = 357.52910
    //    + T * (+ 35999.05030
    //    + T * (-     0.0001559
    //    + T * (-     0.00000048)));

    return std::fmod(M, 360.0);
}

long double Sun::meanLongitude(const JulianDay t)
{
    const JulianDay T(jCenturiesSinceSE(t));

    const long double L0 = 280.46645
        + T * (+ 36000.76983
        + T * (+     0.0003032));

    return std::fmod(L0, 360.0);
}

// (AA p152)
long double Sun::center(const JulianDay t)
{
    const JulianDay T(jCenturiesSinceSE(t));

    const long double M = glm::radians(meanAnomaly(t));

    const long double C =
        + (1.914600 - T * (0.004817 - T * 0.000014)) * std::sin(M)
        + (0.019993 - T *  0.000101) * std::sin(2.0 * M)
        +  0.000290 * std::sin(3.0 * M);

    return C;
}

long double Sun::trueAnomaly(const JulianDay t)
{
    return meanAnomaly(t) + center(t); // v = M + C
}


// True geometric longitude referred to the mean equinox of the date.
long double Sun::trueLongitude(const JulianDay t)
{
    return meanLongitude(t) + center(t); // Θ
}

EquatorialCoords<long double> Sun::apparentPosition(const JulianDay t)
{
    EquatorialCoords<long double> equ;

    const long double O = glm::radians(Moon::meanOrbitLongitude(t));
    const long double e = glm::radians(Earth::trueObliquity(t) + 0.00256 * std::cos(O));
    const long double l = glm::radians(trueLongitude(t) - 0.00569 - 0.00478 * std::sin(O));

    const long double sinl = std::sin(l);

    equ.right_ascension = std::fmod(glm::degrees(std::atan2(std::cos(e) * sinl, std::cos(l))), 360.0);
    equ.declination = glm::degrees(std::asin(std::sin(e) * sinl));

    return equ;
}

HorizontalCoords<long double> Sun::horizontalPosition(
    const AstronomicalTime &aTime
,   const long double latitude
,   const long double longitude)
{
    JulianDay t(julianDay(aTime));
    JulianDay s(siderealTime(aTime));

    EquatorialCoords<long double> equ = Sun::apparentPosition(t);

    return equ.toHorizontal(s, latitude, longitude);
}


// NOTE: This gives the distance from the center of the sun to the
// center of the earth.
long double Sun::distance(const JulianDay t)
{
    // (AA.24.5)
    const long double e = Earth::orbitEccentricity(t);

    const long double R = 1.000001018 * (1.0 - e * e) /
        (1.0 + e * std::cos(glm::radians(trueAnomaly(t))));  // in AU

    // astronomical units to km
    return R * 149598000;
}

long double Sun::meanRadius()
{
    // http://nssdc.gsfc.nasa.gov/planetary/factsheet/sunfact.html

    return 0.696e+6; // in kilometers
}

} // namespace glHimmel
