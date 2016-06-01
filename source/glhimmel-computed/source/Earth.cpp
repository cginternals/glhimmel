#include <glhimmel-computed/Earth.h>

#include <glhimmel-computed/sun.h>
#include <glhimmel-computed/moon.h>

#include <cassert>
#include <cmath>
#include <glm/trigonometric.hpp>

namespace
{
    long double timeToDecimal(long double degrees, long double minutes, long double seconds)
    {
        return degrees + (minutes + seconds / 60.0) / 60.0;
    }

    long double apparentDiameter(long double distance, long double radius)
    {
        return 2 * std::atan(radius / distance);
    }
}

namespace glHimmel
{

// The linear eccentricity of the earth orbit is about 2.5 * 10^6 km.
// Compared to the avg. distance of 149.6 * 10^6 km this is not much.
// http://www.greier-greiner.at/hc/ekliptik.htm

// P. Bretagnon, "Théorie du mouvement de l'ensamble des planètes. Solution VSOP82", 1982

const long double Earth::orbitEccentricity(const JulianDay t)
{
    const JulianDay T(jCenturiesSinceSE(t));

    const long double E = 0.01670862
        + T * (- 0.000042037
        + T * (- 0.0000001236
        + T * (+ 0.00000000004)));

    // (AA.24.4)
    //const long double E = 0.016708617
    //    + T * (- 0.000042037
    //    + T * (- 0.0000001236));

    return std::fmod(E, 360.0);
}


const long double Earth::apparentAngularSunDiameter(const JulianDay t)
{
    return apparentDiameter(Sun::distance(t), Sun::meanRadius());
}


const long double Earth::apparentAngularMoonDiameter(const JulianDay t)
{
    return apparentDiameter(Moon::distance(t), Moon::meanRadius());
}


const long double Earth::longitudeNutation(const JulianDay t)
{
    const JulianDay T(jCenturiesSinceSE(t));

    const long double sM = glm::radians(Sun::meanAnomaly(t));

    const long double mM = glm::radians(Moon::meanAnomaly(t));
    const long double mD = glm::radians(Moon::meanElongation(t));
    const long double mF = glm::radians(Moon::meanLatitude(t));
    const long double O  = glm::radians(Moon::meanOrbitLongitude(t));

    // (AA.21.A)

    long double Dr = 0.0;

    Dr -= (17.1996 - 0.01742 * T) * std::sin(                                   + 1 * O);
    Dr -= ( 1.3187 - 0.00016 * T) * std::sin(-2 * mD                   + 2 * mF + 2 * O);
    Dr -= ( 0.2274 - 0.00002 * T) * std::sin(                          + 2 * mF + 2 * O);
    Dr += ( 0.2062 + 0.00002 * T) * std::sin(                                   + 2 * O);
    Dr += ( 0.1426 - 0.00034 * T) * std::sin(        + 1 * sM                          );
    Dr += ( 0.0712 + 0.00001 * T) * std::sin(                 + 1 * mM                 );
    Dr += ( 0.0517 + 0.00012 * T) * std::sin(-2 * mD + 1 * sM          + 2 * mF + 2 * O);
    Dr -= ( 0.0386 - 0.00004 * T) * std::sin(                          + 2 * mF + 1 * O);
    Dr -= ( 0.0301              ) * std::sin(                 + 1 * mM + 2 * mF + 2 * O);
    Dr += ( 0.0217 - 0.00005 * T) * std::sin(-2 * mD - 1 * sM          + 2 * mF + 2 * O);
    Dr -= ( 0.0158              ) * std::sin(-2 * mD          + 1 * mM                 );
    Dr += ( 0.0129 + 0.00001 * T) * std::sin(-2 * mD                   + 2 * mF + 1 * O);
    Dr += ( 0.0123              ) * std::sin(                 - 1 * mM + 2 * mF + 2 * O);
    Dr += ( 0.0063              ) * std::sin( 2 * mD                                   );
    Dr += ( 0.0063 + 0.00001 * T) * std::sin(                 + 1 * mM          + 1 * O);
    Dr -= ( 0.0059              ) * std::sin( 2 * mD          - 1 * mM + 2 * mF + 2 * O);
    Dr -= ( 0.0058 - 0.00001 * T) * std::sin(                 - 1 * mM          + 1 * O);
    Dr -= ( 0.0051              ) * std::sin(                 + 1 * mM + 2 * mF + 1 * O);
    Dr += ( 0.0048              ) * std::sin(-2 * mD          + 2 * mM                 );
    Dr += ( 0.0046              ) * std::sin(                 - 2 * mM + 2 * mF + 1 * O);
    Dr -= ( 0.0038              ) * std::sin( 2 * mD                   + 2 * mF + 2 * O);
    Dr -= ( 0.0031              ) * std::sin(                 + 2 * mM + 2 * mF + 2 * O);
    Dr += ( 0.0029              ) * std::sin(                 + 2 * mM                 );
    Dr += ( 0.0029              ) * std::sin( 2 * mD          + 1 * mM + 2 * mF + 2 * O);
    Dr += ( 0.0026              ) * std::sin(                          + 2 * mF        );
    Dr -= ( 0.0022              ) * std::sin(-2 * mD                   + 2 * mF        );
    Dr += ( 0.0021              ) * std::sin(                 - 1 * mM + 2 * mF + 1 * O);
    Dr += ( 0.0017 - 0.00001 * T) * std::sin(        + 2 * sM                          );
    Dr += ( 0.0016              ) * std::sin( 2 * mD          - 1 * mM          + 1 * O);
    Dr -= ( 0.0016 + 0.00001 * T) * std::sin(-2 * mD + 2 * sM          + 2 * mF + 2 * O);
    Dr -= ( 0.0015              ) * std::sin(        + 1 * sM                   + 1 * O);
    Dr -= ( 0.0013              ) * std::sin(-2 * mD          + 1 * mM          + 1 * O);
    Dr -= ( 0.0012              ) * std::sin(        - 1 * sM                   + 1 * O);
    Dr += ( 0.0011              ) * std::sin(                 + 2 * mM - 2 * mF        );
    Dr -= ( 0.0010              ) * std::sin( 2 * mD          - 1 * mM + 2 * mF + 1 * O);
    Dr -= ( 0.0008              ) * std::sin( 2 * mD          + 1 * mM + 2 * mF + 2 * O);
    Dr += ( 0.0007              ) * std::sin(        + 1 * sM          + 2 * mF + 2 * O);
    Dr += ( 0.0007              ) * std::sin(-2 * mD + 1 * sM + 1 * mM                 );
    Dr -= ( 0.0007              ) * std::sin(        - 1 * sM          + 2 * mF + 2 * O);
    Dr -= ( 0.0007              ) * std::sin( 2 * mD                   + 2 * mF + 1 * O);
    Dr += ( 0.0006              ) * std::sin( 2 * mD          + 1 * mM                 );
    Dr += ( 0.0006              ) * std::sin(-2 * mD          + 2 * mM + 2 * mF + 2 * O);
    Dr += ( 0.0006              ) * std::sin(-2 * mD          + 1 * mM + 2 * mF + 1 * O);
    Dr -= ( 0.0006              ) * std::sin( 2 * mD          - 2 * mM          + 1 * O);
    Dr -= ( 0.0006              ) * std::sin( 2 * mD                            + 1 * O);
    Dr += ( 0.0005              ) * std::sin(        - 1 * sM + 1 * mM                 );
    Dr += ( 0.0005              ) * std::sin(-2 * mD - 1 * sM          + 2 * mF + 1 * O);
    Dr -= ( 0.0005              ) * std::sin(-2 * mD                            + 1 * O);
    Dr -= ( 0.0005              ) * std::sin(                 + 2 * mM + 2 * mF + 1 * O);
    Dr += ( 0.0004              ) * std::sin(-2 * mD          + 2 * mM          + 1 * O);
    Dr += ( 0.0004              ) * std::sin(-2 * mD + 1 * sM          + 2 * mF + 1 * O);
    Dr += ( 0.0004              ) * std::sin(                 + 1 * mM - 2 * mF        );
    Dr -= ( 0.0004              ) * std::sin(-1 * mD          + 1 * mM                 );
    Dr -= ( 0.0004              ) * std::sin(-2 * mD + 1 * sM                          );
    Dr -= ( 0.0004              ) * std::sin( 1 * mD                                   );
    Dr += ( 0.0003              ) * std::sin(                 + 1 * mM + 2 * mF        );
    Dr -= ( 0.0003              ) * std::sin(                 - 2 * mM + 2 * mF + 2 * O);
    Dr -= ( 0.0003              ) * std::sin(-1 * mD - 1 * sM + 1 * mM                 );
    Dr -= ( 0.0003              ) * std::sin(        + 1 * sM + 1 * mM                 );
    Dr -= ( 0.0003              ) * std::sin(        - 1 * sM + 1 * mM + 2 * mF + 2 * O);
    Dr -= ( 0.0003              ) * std::sin( 2 * mD - 1 * sM - 1 * mM + 2 * mF + 2 * O);
    Dr -= ( 0.0003              ) * std::sin(                 + 3 * mM + 2 * mF + 2 * O);
    Dr -= ( 0.0003              ) * std::sin( 2 * mD - 1 * sM          + 2 * mF + 2 * O);

    return timeToDecimal(0, 0, Dr);
}


// (AA.21)

const long double Earth::obliquityNutation(const JulianDay t)
{
    const JulianDay T(jCenturiesSinceSE(t));

    const long double sM = glm::radians(Sun::meanAnomaly(t));

    const long double mM = glm::radians(Moon::meanAnomaly(t));
    const long double mD = glm::radians(Moon::meanElongation(t));
    const long double mF = glm::radians(Moon::meanLatitude(t));
    const long double O  = glm::radians(Moon::meanOrbitLongitude(t));

    // (AA.21.A)

    long double De = 0.0;

    De += ( 9.2025 + 0.00089 * T) * std::cos(                                   + 1 * O);
    De += ( 0.5736 - 0.00031 * T) * std::cos(-2 * mD                   + 2 * mF + 2 * O);
    De += ( 0.0977 - 0.00005 * T) * std::cos(                          + 2 * mF + 2 * O);
    De -= ( 0.0895 + 0.00005 * T) * std::cos(                                   + 2 * O);
    De += ( 0.0054 - 0.00001 * T) * std::cos(        + 1 * sM                          );
    De -= ( 0.0007              ) * std::cos(                 + 1 * mM                 );
    De += ( 0.0224 - 0.00006 * T) * std::cos(-2 * mD + 1 * sM          + 2 * mF + 2 * O);
    De += ( 0.0200              ) * std::cos(                          + 2 * mF + 1 * O);
    De += ( 0.0129 - 0.00001 * T) * std::cos(                 + 1 * mM + 2 * mF + 2 * O);
    De -= ( 0.0095 + 0.00003 * T) * std::cos(-2 * mD - 1 * sM          + 2 * mF + 2 * O);
    De -= ( 0.0070              ) * std::cos(-2 * mD                   + 2 * mF + 1 * O);
    De -= ( 0.0053              ) * std::cos(                 - 1 * mM + 2 * mF + 2 * O);
    De -= ( 0.0033              ) * std::cos(                 + 1 * mM          + 1 * O);
    De += ( 0.0026              ) * std::cos( 2 * mD          - 1 * mM + 2 * mF + 2 * O);
    De += ( 0.0032              ) * std::cos(                 - 1 * mM          + 1 * O);
    De += ( 0.0027              ) * std::cos(                 + 1 * mM + 2 * mF + 1 * O);
    De -= ( 0.0024              ) * std::cos(                 - 2 * mM + 2 * mF + 1 * O);
    De += ( 0.0016              ) * std::cos( 2 * mD                   + 2 * mF + 2 * O);
    De += ( 0.0013              ) * std::cos(                 + 2 * mM + 2 * mF + 2 * O);
    De -= ( 0.0012              ) * std::cos( 2 * mD          + 1 * mM + 2 * mF + 2 * O);
    De -= ( 0.0010              ) * std::cos(                 - 1 * mM + 2 * mF + 1 * O);
    De -= ( 0.0008              ) * std::cos( 2 * mD          - 1 * mM          + 1 * O);
    De += ( 0.0007              ) * std::cos(-2 * mD + 2 * sM          + 2 * mF + 2 * O);
    De += ( 0.0009              ) * std::cos(        + 1 * sM                   + 1 * O);
    De += ( 0.0007              ) * std::cos(-2 * mD          + 1 * mM          + 1 * O);
    De += ( 0.0006              ) * std::cos(        - 1 * sM                   + 1 * O);
    De += ( 0.0005              ) * std::cos( 2 * mD          - 1 * mM + 2 * mF + 1 * O);
    De += ( 0.0003              ) * std::cos( 2 * mD          + 1 * mM + 2 * mF + 2 * O);
    De -= ( 0.0003              ) * std::cos(        + 1 * sM          + 2 * mF + 2 * O);
    De += ( 0.0003              ) * std::cos(        - 1 * sM          + 2 * mF + 2 * O);
    De += ( 0.0003              ) * std::cos( 2 * mD                   + 2 * mF + 1 * O);
    De -= ( 0.0003              ) * std::cos(-2 * mD          + 2 * mM + 2 * mF + 2 * O);
    De -= ( 0.0003              ) * std::cos(-2 * mD          + 1 * mM + 2 * mF + 1 * O);
    De += ( 0.0003              ) * std::cos( 2 * mD          - 2 * mM          + 1 * O);
    De += ( 0.0003              ) * std::cos( 2 * mD                            + 1 * O);
    De += ( 0.0003              ) * std::cos(-2 * mD - 1 * sM          + 2 * mF + 1 * O);
    De += ( 0.0003              ) * std::cos(-2 * mD                            + 1 * O);
    De += ( 0.0003              ) * std::cos(                 + 2 * mM + 2 * mF + 1 * O);

    return timeToDecimal(0, 0, De);
}


const long double Earth::trueObliquity(const JulianDay t)
{
    return meanObliquity(t) + obliquityNutation(t); // e
}


// Inclination of the Earth's axis of rotation. (AA.21.3)
// By J. Laskar, "Astronomy and Astrophysics" 1986

const long double Earth::meanObliquity(const JulianDay t)
{
    const JulianDay U = jCenturiesSinceSE(t) * 0.01;

    assert(std::abs(U) < 1.0);

    const long double e0 = 0.0
        + U * (- 4680.93
        + U * (-    1.55
        + U * (+ 1999.25
        + U * (-   51.38
        + U * (-  249.67
        + U * (-   39.05
        + U * (+    7.12
        + U * (+   27.87
        + U * (+    5.79
        + U * (+    2.45))))))))));

    return timeToDecimal(23, 26, 21.448) + timeToDecimal(0, 0, e0);
}


// This is, if required, approximatelly refraction corrected...

const long double Earth::viewDistanceWithinAtmosphere(
    const long double y
,   const bool refractionCorrected)
{
    const long double t = atmosphereThickness();
    const long double r = meanRadius();

    // This works, since dot product of [0, 1, 0] and
    // eye with [x, y, z] gives y.

    long double h = std::asin(y * (1.0 - 1.e-12)); // correction is required to
                       // gain t at y = 1.0 - fix for long double accuracy.

    if(refractionCorrected)
        h += glm::radians(atmosphericRefraction(glm::degrees(std::asin(y))));

    const long double cosa = std::cos(h);
    const long double rt = r + t;

    // Using law of sine for arbitrary triangle with two sides and one angle known.
    // Since the angle is (π/2 + a), cos is used instead of sine.

    const long double distance = std::cos(h + std::asin(cosa * r / rt)) * rt / cosa;

    return distance;
}


// Effect of refraction for true altitudes (AA.15.4).
// G.G. Bennet, "The Calculation of the Astronomical Refraction in marine Navigation", 1982
// and Porsteinn Saemundsson, "Sky and Telescope" 1982

const long double Earth::atmosphericRefraction(const long double altitude)
{
    long double R = 1.02 /
        std::tan(glm::radians(altitude + 10.3 / (altitude + 5.11))) + 0.0019279;

    return timeToDecimal(0, R, 0); // (since R is in minutes)
}


const long double Earth::meanRadius()
{
    // http://nssdc.gsfc.nasa.gov/planetary/factsheet/earthfact.html

    return  6371.0; // in kilometers
}


const long double Earth::atmosphereThickness()
{
    // Thickness of atmosphere if the density were uniform.

    // 8000 ("Precomputed Atmospheric Scattering" - 2008 - Bruneton, Neyret)
    // 7994 ("Display of the earth taking into account atmospheric scattering" - 1993 - Nishita et al.)
    return 7.994;
}


const long double Earth::atmosphereThicknessNonUniform()
{
    // Thickness of atmosphere.
    return 85.0; // ~
}


const long double Earth::apparentMagnitudeLimit()
{
    // http://www.astronomynotes.com/starprop/s4.htm
    return 6.5;
}

} // namespace glHimmel
