
#pragma once
#ifndef __JULIANDAY_H__
#define __JULIANDAY_H__

#include <glhimmel-texturebased/glhimmel-texturebased_api.h>
#include <glhimmel-texturebased/typedefs.h>
#include <glhimmel-texturebased/atime.h>


namespace glhimmel_texturebased
{

typedef t_longf t_julianDay;


GLHIMMEL_TEXTUREBASED_API const t_julianDay jd(t_aTime aTime);

GLHIMMEL_TEXTUREBASED_API const t_julianDay jdUT(const t_aTime &aTime);
GLHIMMEL_TEXTUREBASED_API const t_julianDay jd0UT(t_aTime aTime);

GLHIMMEL_TEXTUREBASED_API const t_julianDay mjd(t_aTime aTime);


GLHIMMEL_TEXTUREBASED_API const t_aTime makeTime(
    t_julianDay julianDate
,   const short GMTOffset = 0);

GLHIMMEL_TEXTUREBASED_API const t_aTime makeUT(const t_aTime &aTime);


GLHIMMEL_TEXTUREBASED_API const t_julianDay standardEquinox();
    
GLHIMMEL_TEXTUREBASED_API const t_julianDay j2050();
GLHIMMEL_TEXTUREBASED_API const t_julianDay j2000();
GLHIMMEL_TEXTUREBASED_API const t_julianDay b1900();
GLHIMMEL_TEXTUREBASED_API const t_julianDay b1950();

GLHIMMEL_TEXTUREBASED_API const t_julianDay jdSinceSE(const t_julianDay jd); // JDE
GLHIMMEL_TEXTUREBASED_API const t_julianDay jCenturiesSinceSE(const t_julianDay jd); // T

} // namespace glhimmel_texturebased

#endif // __JULIANDAY_H__