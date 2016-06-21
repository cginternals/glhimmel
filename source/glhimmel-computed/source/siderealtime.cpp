#include <glhimmel-computed/siderealtime.h>

#include <glhimmel-base/julianday.h>

#include <glm/trigonometric.hpp>


namespace glHimmel
{

long double siderealTime(const AstronomicalTime &aTime)
{
    const AstronomicalTime gmt(makeUT(aTime));
    const JulianDay JD(jdUT(gmt));

    // (AA.11.4)

    const long double T(jCenturiesSinceSE(JD));
    const long double t =
        280.46061837 + 360.98564736629 * (jdSinceSE(JD))
        + T * T * (0.000387933 - T / 38710000.0);

    return std::fmod(t, 360.0);
}


long double siderealTime2(const AstronomicalTime &aTime)
{
    const AstronomicalTime gmt(makeUT(aTime));
    const JulianDay JD(jdUT(aTime));

    // ("A Physically-Based Night Sky Model" - 2001 - Wann Jensen et al.)

    const float T(static_cast<float>(jCenturiesSinceSE(JD)));
    const float t = 4.894961f + 230121.675315f * T;

    return std::fmod(glm::degrees(t), 360.0);
}

} // namespace glHimmel
