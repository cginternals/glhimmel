
#include <texturebased/julianday.h>

#include <glm/common.hpp>

#include <cassert>


namespace glHimmel
{
t_julianDay jd(t_aTime aTime)
{
    assert(aTime.month > 0);

    if(aTime.month < 3)
    {
        // "If the date is in January or February, it is concidered to
        // be 13th or 14th month of the preceding year."

        --aTime.year;
        aTime.month += 12;
    }

    // TODO: from "A Physically-Based Night Sky Model" - 2001 - Wann Jensen et al.
    // "Local time is GMT with a zone correction. Terrestrial Time (TT)
    // is essentially the time kept by atomic clocks. As it is not corrected
    // for the slowing of the Earth’s rotation, it gains on GMT by about
    // a second per year. The current difference T is about 64 sec in 2000.
    // It should be added to s in all equations for precise computation..."

    unsigned int s = aTime.second;

    const long double h(toDays(aTime.hour, aTime.minute, s));

    signed int b = 0;

    const int i = aTime.year * 10000 + aTime.month * 100 + aTime.day;
    if(i >= 15821015)
    {
        // gregorian calendar
        const signed int a = static_cast<int>(aTime.year * 0.01);
        b = 2 - a + static_cast<int>(a * 0.25);
    }
    else if(i > 15821004)
        return 0.0; // The gregorian calender follows with 15. on the 4.
                    // julian oktober in 1582 - poor people who missed a birthday these days :P

    return static_cast<int>(365.25 * (aTime.year + 4716))
        + static_cast<int>(30.600001 * (aTime.month + 1)) + aTime.day + h + b - 1524.5;
}

t_julianDay jdUT(const t_aTime &aTime)
{
    return jd(0 == aTime.utcOffset ? aTime : makeUT(aTime));
}

t_julianDay jd0UT(t_aTime aTime)
{
    aTime.second = 0;
    aTime.minute = 0;
    aTime.hour   = 0;

    return jd(0 == aTime.utcOffset ? aTime : makeUT(aTime));
}


// Modified Julian Date.
t_julianDay mjd(t_aTime aTime)
{
    return jd(aTime) - 2400000.5;
}

t_aTime makeTime(
    t_julianDay julianDate
,   const short GMTOffset)
{
    assert(julianDate >= 0.0);

    julianDate += 0.5;

    const int z = static_cast<int>(julianDate);
    const long double f = glm::fract(julianDate);

    long int a(z); // julian
    if(z >= 2299161) // gregorian
    {
        const int g = static_cast<int>((z - 1867216.25) / 36524.25);
        a = z + 1 + g - static_cast<int>(g / 4);
    }

    const int b = static_cast<int>(a + 1524);
    const int c = static_cast<int>((b - 122.1) / 365.25);
    const int d = static_cast<int>(365.25 * c);
    const int e = static_cast<int>((b - d) / 30.600001);


    const short day    = static_cast<short>(b - d - static_cast<int>(30.600001 * e));
    const short month  = static_cast<short>(e < 14 ? e - 1 : e - 13);
    const short year   = static_cast<short>(month > 2 ? c - 4716 : c - 4715);

    const long double h = f * 24.0;
    const long double m = glm::fract(h) * 60.0;
    const long double s = glm::fract(m) * 60.0001;

    const short hour   = static_cast<short>(h);
    const short minute = static_cast<short>(m);
    const short second = static_cast<short>(s);

    return t_aTime(year, month, day, hour, minute, second, GMTOffset);
}

t_aTime makeUT(const t_aTime &aTime)
{
    return makeTime(jd(aTime) - aTime.utcOffset / 3600.0 / 24.0, 0);
}


// http://en.wikipedia.org/wiki/Equatorial_coordinate_system
// Over long periods of time, precession and nutation effects alter the Earth's orbit and thus
// the apparent location of the stars. Likewise, proper motion of the stars themselves will affect
// their coordinates as seen from Earth. When considering observations separated by long intervals,
// it is necessary to specify an epoch (frequently J2000.0, for older data B1950.0) when specifying
// coordinates of planets, stars, galaxies, etc.
t_julianDay standardEquinox()
{
    return j2000();
}

t_julianDay j2000()
{
    return 2451545.0; // 2000 January 1 noon TT
    // 2451543.5 was used by http://www.stjarnhimlen.se/comp/tutorial.html
}

t_julianDay j2050()
{
    return 2469807.5; // 2050 January 1 noon TT
}

t_julianDay b1900()
{
    return 2415020.3135; // 1900 January 0.8135 TT
}

t_julianDay b1950()
{
    return 2433282.4235; // 1950 January 0.9235 TT
}

t_julianDay jdSinceSE(const t_julianDay jd)
{
    return jd - standardEquinox();
}

t_julianDay jCenturiesSinceSE(const t_julianDay jd)   // T
{
    // Even if google says that 1 century has currently 36 524.2199 days,
    // the julian century is explicitly defined as 36525 days...

    // Used in http://en.wikipedia.org/wiki/Julian_year_(astronomy) and
    // "Astronomical Algorithms"
    return jdSinceSE(jd) / 36525.0;
}

} // namespace glHimmel
