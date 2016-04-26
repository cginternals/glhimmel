
#pragma once

#include <texturebased/texturebased_api.h>

#include <texturebased/astronomicaltime.h>


namespace glHimmel
{

typedef long double t_julianDay;


TEXTUREBASED_API const t_julianDay jd(t_aTime aTime);

TEXTUREBASED_API const t_julianDay jdUT(const t_aTime &aTime);
TEXTUREBASED_API const t_julianDay jd0UT(t_aTime aTime);

TEXTUREBASED_API const t_julianDay mjd(t_aTime aTime);


TEXTUREBASED_API const t_aTime makeTime(
    t_julianDay julianDate
,   const short GMTOffset = 0);

TEXTUREBASED_API const t_aTime makeUT(const t_aTime &aTime);


TEXTUREBASED_API const t_julianDay standardEquinox();
    
TEXTUREBASED_API const t_julianDay j2050();
TEXTUREBASED_API const t_julianDay j2000();
TEXTUREBASED_API const t_julianDay b1900();
TEXTUREBASED_API const t_julianDay b1950();

TEXTUREBASED_API const t_julianDay jdSinceSE(const t_julianDay jd); // JDE
TEXTUREBASED_API const t_julianDay jCenturiesSinceSE(const t_julianDay jd); // T

} // namespace glHimmel
