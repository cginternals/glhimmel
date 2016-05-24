
#include <glhimmel-base/AstronomicalTime.h>

#include <glhimmel-base/Timef.h>
#include <glm/common.hpp>

namespace glHimmel
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
,   const long double day
,   const short utcOffset)
:   year(year)
,   month(month)
,   day(static_cast<unsigned short>(day))
,   utcOffset(utcOffset)
{
    const long double h = glm::fract(day) * 24.0;
    const long double m = glm::fract(h) * 60.0;
    const long double s = glm::fract(m) * 60.001;

    this->hour   = static_cast<short>(h);
    this->minute = static_cast<short>(m);
    this->second = static_cast<short>(s);
}


s_AstronomicalTime s_AstronomicalTime::fromTimeT(
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


s_AstronomicalTime s_AstronomicalTime::fromTimeF(const TimeF &t)
{
    return fromTimeT(t.gett(), t.getUtcOffset());
}


time_t s_AstronomicalTime::toTime_t() const
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


long double s_AstronomicalTime::dayf() const
{
    return day + toDays(hour, minute, second);
}

float toDays(int hour, int minute, int second)
{
    return (hour + (minute + second / 60.f) / 60.f) / 24.f;
}

} // namespace glHimmel
