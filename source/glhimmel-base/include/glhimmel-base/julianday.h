
#pragma once

#include <glhimmel-base/glhimmel-base_api.h>

#include <glhimmel-base/AstronomicalTime.h>


namespace glHimmel
{

typedef long double t_julianDay;


GLHIMMEL_BASE_API t_julianDay jd(t_aTime aTime);

GLHIMMEL_BASE_API t_julianDay jdUT(const t_aTime &aTime);
GLHIMMEL_BASE_API t_julianDay jd0UT(t_aTime aTime);

GLHIMMEL_BASE_API t_julianDay mjd(t_aTime aTime);


GLHIMMEL_BASE_API t_aTime makeTime(
    t_julianDay julianDate
,   const short GMTOffset = 0);

GLHIMMEL_BASE_API t_aTime makeUT(const t_aTime &aTime);


GLHIMMEL_BASE_API t_julianDay standardEquinox();

GLHIMMEL_BASE_API t_julianDay j2050();
GLHIMMEL_BASE_API t_julianDay j2000();
GLHIMMEL_BASE_API t_julianDay b1900();
GLHIMMEL_BASE_API t_julianDay b1950();

GLHIMMEL_BASE_API t_julianDay jdSinceSE(const t_julianDay jd); // JDE
GLHIMMEL_BASE_API t_julianDay jCenturiesSinceSE(const t_julianDay jd); // T

} // namespace glHimmel
