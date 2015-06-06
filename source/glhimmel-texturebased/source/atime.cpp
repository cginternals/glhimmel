
#include <glhimmel-texturebased/atime.h>

#include <glhimmel-texturebased/mathmacros.h>
#include <glhimmel-texturebased/timef.h>

#include <assert.h>


namespace glhimmel_texturebased
{

s_AstronomicalTime::s_AstronomicalTime()
:   year(0)
,   month(1)
,   day(1)
,   hour(0)
,   minute(0)
,   second(0)
,   utcOffset(0)
{
}


s_AstronomicalTime::s_AstronomicalTime(
    const short year
,   const short month
,   const short day
,   const short hour
,   const short minute
,   const short second
,   const short utcOffset)
:   year(year)
,   month(month)
,   day(day)
,   hour(hour)
,   minute(minute)
,   second(second)
,   utcOffset(utcOffset)
{
}


s_AstronomicalTime::s_AstronomicalTime(
    const short year
,   const short month
,   const t_longf day
,   const short utcOffset)
:   year(year)
,   month(month)
,   day(static_cast<unsigned short>(day))
,   utcOffset(utcOffset)
{
    const t_longf h = _frac(day) * 24.0;
    const t_longf m = _frac(h) * 60.0;
    const t_longf s = _frac(m) * 60.001;

    this->hour   = _short(h);
    this->minute = _short(m);
    this->second = _short(s);
}


const s_AstronomicalTime s_AstronomicalTime::fromTimeT(
    const time_t &time
,   const time_t &utcOffset)
{
    // Daylight saving time should not be concidered here -> julian time functions ignore this.

#ifdef __GNUC__
    struct tm lcl(*localtime(&time));
#else // __GNUC__
    struct tm lcl;
    localtime_s(&lcl, &time);
#endif // __GNUC__

    time_t mt = mktime(&lcl);

    if(mt == -1)
        return s_AstronomicalTime();

    return s_AstronomicalTime(
        static_cast<short>(lcl.tm_year + 1900)
    ,   static_cast<short>(lcl.tm_mon + 1)
    ,   static_cast<short>(lcl.tm_mday)
    ,   static_cast<short>(lcl.tm_hour)
    ,   static_cast<short>(lcl.tm_min)
    ,   static_cast<short>(lcl.tm_sec)
    ,   static_cast<short>(utcOffset));
}


const s_AstronomicalTime s_AstronomicalTime::fromTimeF(const TimeF &t)
{
    return fromTimeT(t.gett(), t.getUtcOffset());
}


const time_t s_AstronomicalTime::toTime_t() const
{
    time_t t = 0;

#ifdef __GNUC__
    struct tm lcl(*localtime(&t));
#else // __GNUC__
    struct tm lcl;
    localtime_s(&lcl, &t);
#endif // __GNUC__

    lcl.tm_year = year - 1900;
    lcl.tm_mon  = month - 1;
    lcl.tm_mday = day;

    lcl.tm_hour = hour;
    lcl.tm_min  = minute;
    lcl.tm_sec  = second;

    lcl.tm_isdst = 0;

    t = mktime(&lcl);

    return t;
}


const t_longf s_AstronomicalTime::dayf() const
{
    return day + _day(hour, minute, second);
}

} // namespace glhimmel_texturebased