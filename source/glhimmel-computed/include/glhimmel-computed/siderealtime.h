#pragma once

#include <glhimmel-computed/glhimmel-computed_api.h>
#include <glhimmel-base/AstronomicalTime.h>


namespace glHimmel
{

// The mean sideral time, Greenwich hour angle of the mean vernal point.
GLHIMMEL_COMPUTED_API long double siderealTime(const AstronomicalTime &aTime);
GLHIMMEL_COMPUTED_API long double siderealTime2(const AstronomicalTime &aTime);

} // namespace glHimmel

