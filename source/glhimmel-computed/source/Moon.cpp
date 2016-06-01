#include <glhimmel-computed/moon.h>

#include <glhimmel-computed/sun.h>
#include <glhimmel-computed/earth.h>
#include <glhimmel-computed/siderealtime.h>

#include <cassert>
#include <glm/trigonometric.hpp>
#include <glm/gtc/constants.hpp>

namespace glHimmel
{

// Mean longitude, referred to the mean equinox of the date (AA.45.1).

const long double Moon::meanLongitude(const JulianDay t)
{
    const JulianDay T(jCenturiesSinceSE(t));

    const long double L0 = 218.3164591
        + T * (+ 481267.88134236
        + T * (-      0.0013268
        + T * (+ 1.0 / 528841.0
        + T * (- 1.0 / 65194000.0))));

    return std::fmod(L0, 360.0);
}


// Mean elongation (AA.45.2).

const long double Moon::meanElongation(const JulianDay t)
{
    const JulianDay T(jCenturiesSinceSE(t));

    const long double D = 297.8502042
        + T * (+ 445267.1115168
        + T * (-      0.0016300
        + T * (+ 1.0 / 545868.0
        + T * (- 1.0 / 113065000.0))));

    return std::fmod(D, 360.0);
}


// Mean anomaly (AA.45.4).

const long double Moon::meanAnomaly(const JulianDay t)
{
    const JulianDay T(jCenturiesSinceSE(t));

    const long double M = 134.9634114
        + T * (+ 477198.8676313
        + T * (+      0.0089970
        + T * (+ 1.0 / 69699.0
        + T * (- 1.0 / 14712000.0))));

    return std::fmod(M, 360.0);
}


// Mean distance of the Moon from its ascending node (AA.45.5)

const long double Moon::meanLatitude(const JulianDay t)
{
    const JulianDay T(jCenturiesSinceSE(t));

    const long double F = 93.2720993
        + T * (+ 483202.0175273
        + T * (-      0.0034029
        + T * (- 1.0 / 3526000.0
        + T * (+ 1.0 / 863310000.0))));

    return std::fmod(F, 360.0);
}


const long double Moon::meanOrbitLongitude(const JulianDay t)
{
    const JulianDay T(jCenturiesSinceSE(t));

    const long double O = 125.04452
        + T * (- 1934.136261
        + T * (+    0.0020708
        + T * (+ 1.0 / 450000.0)));

    return std::fmod(O, 360.0);
}


const EclipticalCoords<long double> Moon::position(const JulianDay t)
{
    const long double sM = glm::radians(Sun::meanAnomaly(t));

    const long double mL = glm::radians(meanLongitude(t));
    const long double mM = glm::radians(meanAnomaly(t));
    const long double mD = glm::radians(meanElongation(t));
    const long double mF = glm::radians(meanLatitude(t));

    const JulianDay T(jCenturiesSinceSE(t));

    const long double A1 = glm::radians(std::fmod(119.75 +    131.849 * T, 360.0));
    const long double A2 = glm::radians(std::fmod( 53.09 + 479264.290 * T, 360.0));
    const long double A3 = glm::radians(std::fmod(313.45 + 481266.484 * T, 360.0));

    //const long double E = earth_orbitEccentricity(t);
    // -> does not apply here - the eccentricity of the earths' orbit
    // in 45.6 is about 60. times the earth_orbitEccentricity...?

    // Correction for eccentricity of the Earth's orbit around the sun.

    // (AA.45.6)
    const long double E = 1.0
        + T * (- 0.002516
        + T * (- 0.0000074));

    const long double EE = E * E;

    // (AA.45.A)

    long double Sl = 0.0;

    Sl += 6288.774 * std::sin(                 + 1 * mM         );
    Sl += 1274.027 * std::sin( 2 * mD          - 1 * mM         );
    Sl +=  658.314 * std::sin( 2 * mD                           );
    Sl +=  213.618 * std::sin(                 + 2 * mM         );
    Sl -=  185.116 * std::sin(        + 1 * sM                  ) * E;
    Sl -=  114.332 * std::sin(                          + 2 * mF);
    Sl +=   58.793 * std::sin( 2 * mD          - 2 * mM         );
    Sl +=   57.066 * std::sin( 2 * mD - 1 * sM - 1 * mM         ) * E;
    Sl +=   53.322 * std::sin( 2 * mD          + 1 * mM         );
    Sl +=   45.758 * std::sin( 2 * mD - 1 * sM                  ) * E;
    Sl -=   40.923 * std::sin(        + 1 * sM - 1 * mM         ) * E;
    Sl -=   34.720 * std::sin( 1 * mD                           );
    Sl -=   30.383 * std::sin(        + 1 * sM + 1 * mM         ) * E;
    Sl +=   15.327 * std::sin( 2 * mD                   - 2 * mF);
    Sl -=   12.528 * std::sin(                 + 1 * mM + 2 * mF);
    Sl +=   10.980 * std::sin(                 + 1 * mM - 2 * mF);
    Sl +=   10.675 * std::sin( 4 * mD          - 1 * mM         );
    Sl +=   10.034 * std::sin(                 + 3 * mM         );
    Sl +=    8.548 * std::sin( 4 * mD          - 2 * mM         );
    Sl -=    7.888 * std::sin( 2 * mD + 1 * sM - 1 * mM         ) * E;
    Sl -=    6.766 * std::sin( 2 * mD + 1 * sM                  ) * E;
    Sl -=    5.163 * std::sin( 1 * mD          - 1 * mM         );
    Sl +=    4.987 * std::sin( 1 * mD + 1 * sM                  ) * E;
    Sl +=    4.036 * std::sin( 2 * mD - 1 * sM + 1 * mM         ) * E;
    Sl +=    3.994 * std::sin( 2 * mD          + 2 * mM         );
    Sl +=    3.861 * std::sin( 4 * mD                           );
    Sl +=    3.665 * std::sin( 2 * mD          - 3 * mM         );
    Sl -=    2.689 * std::sin(        + 1 * sM - 2 * mM         ) * E;
    Sl -=    2.602 * std::sin( 2 * mD          - 1 * mM + 2 * mF);
    Sl +=    2.390 * std::sin( 2 * mD - 1 * sM - 2 * mM         ) * E;
    Sl -=    2.348 * std::sin( 1 * mD          + 1 * mM         );
    Sl +=    2.236 * std::sin( 2 * mD - 2 * sM                  ) * EE;
    Sl -=    2.120 * std::sin(        + 1 * sM + 2 * mM         ) * E;
    Sl -=    2.069 * std::sin(        + 2 * sM                  ) * EE;
    Sl +=    2.048 * std::sin( 2 * mD - 2 * sM - 1 * mM         ) * EE;
    Sl -=    1.773 * std::sin( 2 * mD          + 1 * mM - 2 * mF);
    Sl -=    1.595 * std::sin( 2 * mD                   + 2 * mF);
    Sl +=    1.215 * std::sin( 4 * mD - 1 * sM - 1 * mM         ) * E;
    Sl -=    1.110 * std::sin(                 + 2 * mM + 2 * mF);
    Sl -=    0.892 * std::sin( 3 * mD          - 1 * mM         );
    Sl -=    0.810 * std::sin( 2 * mD + 1 * sM + 1 * mM         ) * E;
    Sl +=    0.759 * std::sin( 4 * mD - 1 * sM - 2 * mM         ) * E;
    Sl -=    0.713 * std::sin(        + 2 * sM - 1 * mM         ) * EE;
    Sl -=    0.700 * std::sin( 2 * mD + 2 * sM - 1 * mM         ) * EE;
    Sl +=    0.691 * std::sin( 2 * mD + 1 * sM - 2 * mM         );
    Sl +=    0.596 * std::sin( 2 * mD - 1 * sM          - 2 * mF) * E;
    Sl +=    0.549 * std::sin( 4 * mD          + 1 * mM         );
    Sl +=    0.537 * std::sin(                 + 4 * mM         );
    Sl +=    0.520 * std::sin( 4 * mD - 1 * sM                  ) * E;
    Sl -=    0.487 * std::sin( 1 * mD          - 2 * mM         );
    Sl -=    0.399 * std::sin( 2 * mD + 1 * sM          - 2 * mF) * E;
    Sl -=    0.381 * std::sin(                 + 2 * mM - 2 * mF);
    Sl +=    0.351 * std::sin( 1 * mD + 1 * sM + 1 * mM         ) * E;
    Sl -=    0.340 * std::sin( 3 * mD          - 2 * mM         );
    Sl +=    0.330 * std::sin( 4 * mD          - 3 * mM         );
    Sl +=    0.327 * std::sin( 2 * mD - 1 * sM + 2 * mM         ) * E;
    Sl -=    0.323 * std::sin(        + 2 * sM + 1 * mM         ) * EE;
    Sl +=    0.299 * std::sin( 1 * mD + 1 * sM - 1 * mM         ) * E;
    Sl +=    0.294 * std::sin( 2 * mD          + 3 * mM         );

    // (AA.45.B)

    long double Sb = 0.0;

    Sb += 5128.122 * std::sin(                          + 1 * mF);
    Sb +=  280.602 * std::sin(                 + 1 * mM + 1 * mF);
    Sb +=  277.693 * std::sin(                 + 1 * mM - 1 * mF);
    Sb +=  173.237 * std::sin( 2 * mD                   - 1 * mF);
    Sb +=   55.413 * std::sin( 2 * mD          - 1 * mM + 1 * mF);
    Sb +=   46.271 * std::sin( 2 * mD          - 1 * mM - 1 * mF);
    Sb +=   32.573 * std::sin( 2 * mD                   + 1 * mF);
    Sb +=   17.198 * std::sin(                 + 2 * mM + 1 * mF);
    Sb +=    9.266 * std::sin( 2 * mD          + 1 * mM - 1 * mF);
    Sb +=    8.822 * std::sin(                 + 2 * mM - 1 * mF);
    Sb +=    8.216 * std::sin( 2 * mD - 1 * sM          - 1 * mF) * E;
    Sb +=    4.324 * std::sin( 2 * mD          - 2 * mM - 1 * mF);
    Sb +=    4.200 * std::sin( 2 * mD          + 1 * mM + 1 * mF);
    Sb -=    3.359 * std::sin( 2 * mD + 1 * sM          - 1 * mF) * E;
    Sb +=    2.463 * std::sin( 2 * mD - 1 * sM - 1 * mM + 1 * mF) * E;
    Sb +=    2.211 * std::sin( 2 * mD - 1 * sM          + 1 * mF) * E;
    Sb +=    2.065 * std::sin( 2 * mD - 1 * sM - 1 * mM - 1 * mF) * E;
    Sb -=    1.870 * std::sin(        + 1 * sM - 1 * mM - 1 * mF) * E;
    Sb +=    1.828 * std::sin( 4 * mD          - 1 * mM - 1 * mF);
    Sb -=    1.794 * std::sin(        + 1 * sM          + 1 * mF) * E;
    Sb -=    1.749 * std::sin(                          + 3 * mF);
    Sb -=    1.565 * std::sin(        + 1 * sM - 1 * mM + 1 * mF) * E;
    Sb -=    1.491 * std::sin( 1 * mD                   + 1 * mF);
    Sb -=    1.475 * std::sin(        + 1 * sM + 1 * mM + 1 * mF) * E;
    Sb -=    1.410 * std::sin(        + 1 * sM + 1 * mM - 1 * mF) * E;
    Sb -=    1.344 * std::sin(        + 1 * sM          - 1 * mF) * E;
    Sb -=    1.335 * std::sin( 1 * mD                   - 1 * mF);
    Sb +=    1.107 * std::sin(                 + 3 * mM + 1 * mF);
    Sb +=    1.024 * std::sin( 4 * mD                   - 1 * mF);
    Sb +=    0.833 * std::sin( 4 * mD          - 1 * mM + 1 * mF);
    Sb +=    0.777 * std::sin(                 + 1 * mM - 3 * mF);
    Sb +=    0.671 * std::sin( 4 * mD          - 2 * mM + 1 * mF);
    Sb +=    0.607 * std::sin( 2 * mD                   - 3 * mF);
    Sb +=    0.596 * std::sin( 2 * mD          + 2 * mM - 1 * mF);
    Sb +=    0.491 * std::sin( 2 * mD - 1 * sM + 1 * mM - 1 * mF) * E;
    Sb -=    0.451 * std::sin( 2 * mD          - 2 * mM + 1 * mF);
    Sb +=    0.439 * std::sin(                 + 3 * mM - 1 * mF);
    Sb +=    0.422 * std::sin( 2 * mD          + 2 * mM + 1 * mF);
    Sb +=    0.421 * std::sin( 2 * mD          - 3 * mM - 1 * mF);
    Sb -=    0.366 * std::sin( 2 * mD + 1 * sM - 1 * mM + 1 * mF) * E;
    Sb -=    0.351 * std::sin( 2 * mD + 1 * sM          + 1 * mF) * E;
    Sb +=    0.331 * std::sin( 4 * mD                   + 1 * mF);
    Sb +=    0.315 * std::sin( 2 * mD - 1 * sM + 1 * mM + 1 * mF) * E;
    Sb +=    0.302 * std::sin( 2 * mD - 2 * sM          - 1 * mF) * EE;
    Sb -=    0.283 * std::sin(                 + 1 * mM + 3 * mF);
    Sb -=    0.229 * std::sin( 2 * mD + 1 * sM + 1 * mM - 1 * mF) * E;
    Sb +=    0.223 * std::sin( 1 * mD + 1 * sM          - 1 * mF) * E;
    Sb +=    0.223 * std::sin( 1 * mD + 1 * sM          + 1 * mF) * E;
    Sb -=    0.220 * std::sin(        + 1 * sM - 2 * mM - 1 * mF) * E;
    Sb -=    0.220 * std::sin( 2 * mD + 1 * sM - 1 * mM - 1 * mF) * E;
    Sb -=    0.185 * std::sin( 1 * mD          + 1 * mM + 1 * mF);
    Sb +=    0.181 * std::sin( 2 * mD - 1 * sM - 2 * mM - 1 * mF) * E;
    Sb -=    0.177 * std::sin(        + 1 * sM + 2 * mM + 1 * mF) * E;
    Sb +=    0.176 * std::sin( 4 * mD          - 2 * mM - 1 * mF);
    Sb +=    0.166 * std::sin( 4 * mD - 1 * sM - 1 * mM - 1 * mF) * E;
    Sb -=    0.164 * std::sin( 1 * mD          + 1 * mM - 1 * mF);
    Sb +=    0.132 * std::sin( 4 * mD          + 1 * mM - 1 * mF);
    Sb -=    0.119 * std::sin( 1 * mD          - 1 * mM - 1 * mF);
    Sb +=    0.115 * std::sin( 4 * mD - 1 * sM          - 1 * mF) * E;
    Sb +=    0.107 * std::sin( 2 * mD - 2 * sM          + 1 * mF) * EE;

    // Add corrective Terms

    Sl +=  3.958 * std::sin(A1)
         + 1.962 * std::sin(mL - mF)
         + 0.318 * std::sin(A2);

    Sb += -2.235 * std::sin(mL)
         + 0.382 * std::sin(A3)
         + 0.175 * std::sin(A1 - mF)
         + 0.175 * std::sin(A1 + mF)
         + 0.127 * std::sin(mL - mM)
         - 0.115 * std::sin(mL + mM);

    EclipticalCoords<long double> ecl;

    ecl.longitude = meanLongitude(t) + Sl * 0.001 + Earth::longitudeNutation(t);
    ecl.latitude = Sb * 0.001;

    return ecl;
}


const EquatorialCoords<long double> Moon::apparentPosition(const JulianDay t)
{
    EclipticalCoords<long double> ecl = position(t);
    ecl.longitude += Earth::longitudeNutation(t);

    return ecl.toEquatorial(Earth::meanObliquity(t));
}


const HorizontalCoords<long double> Moon::horizontalPosition(
    const AstronomicalTime &aTime
,   const long double latitude
,   const long double longitude)
{
    JulianDay t(jd(aTime));
    JulianDay s(siderealTime(aTime));

    EquatorialCoords<long double> equ = apparentPosition(t);

    return equ.toHorizontal(s, latitude, longitude);
}


// NOTE: This gives the distance from the center of the moon to the
// center of the earth.

const long double Moon::distance(const JulianDay t)
{
    const long double sM = glm::radians(Sun::meanAnomaly(t));

    const long double mM = glm::radians(meanAnomaly(t));
    const long double mD = glm::radians(meanElongation(t));
    const long double mF = glm::radians(meanLatitude(t));

    const JulianDay T(jCenturiesSinceSE(t));

    // Correction for eccentricity of the Earth's orbit around the sun.

    // (AA.45.6)

    const long double E = 1.0
        + T * (- 0.002516
        + T * (- 0.0000074));

    const long double EE = E * E;

    // (AA.45.A)

    long double Sr = 0.0;

    Sr -= 20905.355 * std::cos(                 + 1 * mM         );
    Sr -=  3699.111 * std::cos( 2 * mD          - 1 * mM         );
    Sr -=  2955.968 * std::cos( 2 * mD                           );
    Sr -=   569.925 * std::cos(                 + 2 * mM         );
    Sr +=    48.888 * std::cos(        + 1 * sM                  ) * E;
    Sr -=     3.149 * std::cos(                          + 2 * mF);
    Sr +=   246.158 * std::cos( 2 * mD          - 2 * mM         );
    Sr -=   152.138 * std::cos( 2 * mD - 1 * sM - 1 * mM         ) * E;
    Sr -=   170.733 * std::cos( 2 * mD          + 1 * mM         );
    Sr -=   204.586 * std::cos( 2 * mD - 1 * sM                  ) * E;
    Sr -=   129.620 * std::cos(        + 1 * sM - 1 * mM         ) * E;
    Sr +=   108.743 * std::cos( 1 * mD                           );
    Sr +=   104.755 * std::cos(        + 1 * sM + 1 * mM         ) * E;
    Sr +=    10.321 * std::cos( 2 * mD                   - 2 * mF);
    Sr +=    79.661 * std::cos(                 + 1 * mM - 2 * mF);
    Sr -=    34.782 * std::cos( 4 * mD          - 1 * mM         );
    Sr -=    23.210 * std::cos(                 + 3 * mM         );
    Sr -=    21.636 * std::cos( 4 * mD          - 2 * mM         );
    Sr +=    24.208 * std::cos( 2 * mD + 1 * sM - 1 * mM         ) * E;
    Sr +=    30.824 * std::cos( 2 * mD + 1 * sM                  ) * E;
    Sr -=     8.379 * std::cos( 1 * mD          - 1 * mM         );
    Sr -=    16.675 * std::cos( 1 * mD + 1 * sM                  ) * E;
    Sr -=    12.831 * std::cos( 2 * mD - 1 * sM + 1 * mM         ) * E;
    Sr -=    10.445 * std::cos( 2 * mD          + 2 * mM         );
    Sr -=    11.650 * std::cos( 4 * mD                           );
    Sr +=    14.403 * std::cos( 2 * mD          - 3 * mM         );
    Sr -=     7.003 * std::cos(        + 1 * sM - 2 * mM         ) * E;
    Sr +=    10.056 * std::cos( 2 * mD - 1 * sM - 2 * mM         ) * E;
    Sr +=     6.322 * std::cos( 1 * mD          + 1 * mM         );
    Sr -=     9.884 * std::cos( 2 * mD - 2 * sM                  ) * EE;
    Sr +=     5.751 * std::cos(        + 1 * sM + 2 * mM         ) * E;
    Sr -=     4.950 * std::cos( 2 * mD - 2 * sM - 1 * mM         ) * EE;
    Sr +=     4.130 * std::cos( 2 * mD          + 1 * mM - 2 * mF);
    Sr -=     3.958 * std::cos( 4 * mD - 1 * sM - 1 * mM         ) * E;
    Sr +=     3.258 * std::cos( 3 * mD          - 1 * mM         );
    Sr +=     2.616 * std::cos( 2 * mD + 1 * sM + 1 * mM         ) * E;
    Sr -=     1.897 * std::cos( 4 * mD - 1 * sM - 2 * mM         ) * E;
    Sr -=     2.117 * std::cos(        + 2 * sM - 1 * mM         ) * EE;
    Sr +=     2.354 * std::cos( 2 * mD + 2 * sM - 1 * mM         ) * EE;
    Sr -=     1.423 * std::cos( 4 * mD          + 1 * mM         );
    Sr -=     1.117 * std::cos(                 + 4 * mM         );
    Sr -=     1.571 * std::cos( 4 * mD - 1 * sM                  ) * E;
    Sr -=     1.739 * std::cos( 1 * mD          - 2 * mM         );
    Sr -=     4.421 * std::cos(                 + 2 * mM - 2 * mF);
    Sr +=     1.165 * std::cos(        + 2 * sM + 1 * mM         ) * EE;
    Sr +=     8.752 * std::cos( 2 * mD          - 1 * mM - 2 * mF);

    const long double D = 385000.56 + Sr; // in kilometers

    return D;
}


void Moon::opticalLibrations(
    const JulianDay t
,   long double &l /* librations in longitude */
,   long double &b /* librations in latitude  */)
{
    // (AA.51.1)

    const long double Dr = glm::radians(Earth::longitudeNutation(t));

    const long double F  = glm::radians(meanLatitude(t));
    const long double O  = glm::radians(meanOrbitLongitude(t));

    const EclipticalCoords<long double> ecl = position(t);
    const long double lo = glm::radians(ecl.longitude);
    const long double la = glm::radians(ecl.latitude);

    static const long double I = glm::radians(1.54242);

    const long double cos_la = std::cos(la);
    const long double sin_la = std::sin(la);
    const long double cos_I  = std::cos(I);
    const long double sin_I  = std::sin(I);

    const long double W  = std::fmod(lo - Dr - O, glm::two_pi<long double>());
    const long double sin_W  = std::sin(W);

    const long double A  = std::fmod(std::atan2(sin_W * cos_la * cos_I - sin_la * sin_I, std::cos(W) * cos_la), glm::two_pi<long double>());

    l = glm::degrees(A - F);
    b = glm::degrees(std::asin(-sin_W * cos_la * sin_I - sin_la * cos_I));
}


const long double Moon::parallacticAngle(
    const AstronomicalTime &aTime
,   const long double latitude
,   const long double longitude)
{
    // (AA.13.1)

    const JulianDay t(jd(aTime));

    const long double la = glm::radians(latitude);
    const long double lo = glm::radians(longitude);

    const EquatorialCoords<long double> pos = apparentPosition(t);
    const long double ra = glm::radians(pos.right_ascension);
    const long double de = glm::radians(pos.declination);

    const long double s  = glm::radians(siderealTime(aTime));

    // (AA.p88) - local hour angle

    const long double H = s + lo - ra;

    const long double cos_la = std::cos(la);
    const long double P = std::atan2(std::sin(H) * cos_la, std::sin(la) * std::cos(de) - std::sin(de) * cos_la * std::cos(H));

    return glm::degrees(P);
}


const long double Moon::positionAngleOfAxis(const JulianDay t)
{
    // (AA.p344)

    const EquatorialCoords<long double> pos = apparentPosition(t);

    const long double a  = glm::radians(pos.right_ascension);
    const long double e  = glm::radians(Earth::meanObliquity(t));

    const long double Dr = glm::radians(Earth::longitudeNutation(t));
    const long double O  = glm::radians(meanOrbitLongitude(t));

    const long double V  = O + Dr;

    static const long double I = glm::radians(1.54242);
    const long double sin_I  = std::sin(I);

    const long double X  = sin_I * std::sin(V);
    const long double Y  = sin_I * std::cos(V) * std::cos(e) - std::cos(I) * std::sin(e);

    // optical libration in latitude

    const EclipticalCoords<long double> ecl = position(t);

    const long double lo = glm::radians(ecl.longitude);
    const long double la = glm::radians(ecl.latitude);

    const long double W  = std::fmod(lo - Dr - O, glm::two_pi<long double>());
    const long double b = std::asin(-std::sin(W) * std::cos(la) * sin_I - std::sin(la) * std::cos(I));

    // final angle

    const long double w  = std::fmod(std::atan2(X, Y), glm::two_pi<long double>());
    const long double P = std::asin(sqrt(X * X + Y * Y) * std::cos(a - w) / std::cos(b));

    return glm::degrees(P);
}


const long double Moon::meanRadius()
{
    // http://nssdc.gsfc.nasa.gov/planetary/factsheet/moonfact.html

    static const long double r = 1737.1; // in kilometers

    return r;
}

} // namespace glHimmel
