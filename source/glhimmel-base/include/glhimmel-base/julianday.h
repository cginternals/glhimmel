
#pragma once

#include <glhimmel-base/glhimmel-base_api.h>

#include <glhimmel-base/AstronomicalTime.h>


namespace glHimmel
{

typedef long double JulianDay;


GLHIMMEL_BASE_API JulianDay julianDay(astronomicalTime aTime);

GLHIMMEL_BASE_API JulianDay jdUT(const astronomicalTime &aTime);
GLHIMMEL_BASE_API JulianDay jd0UT(astronomicalTime aTime);

GLHIMMEL_BASE_API JulianDay mjd(astronomicalTime aTime);


GLHIMMEL_BASE_API astronomicalTime makeTime(
    JulianDay julianDate
,   const short GMTOffset = 0);

GLHIMMEL_BASE_API astronomicalTime makeUT(const astronomicalTime &aTime);


GLHIMMEL_BASE_API JulianDay standardEquinox();

GLHIMMEL_BASE_API JulianDay j2050();
GLHIMMEL_BASE_API JulianDay j2000();
GLHIMMEL_BASE_API JulianDay b1900();
GLHIMMEL_BASE_API JulianDay b1950();

GLHIMMEL_BASE_API JulianDay jdSinceSE(const JulianDay jd); // JDE
GLHIMMEL_BASE_API JulianDay jCenturiesSinceSE(const JulianDay jd); // T

} // namespace glHimmel
