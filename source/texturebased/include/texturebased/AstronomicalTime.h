
#pragma once

#include <texturebased/texturebased_api.h>
#include <ctime>

namespace glHimmel
{

class TimeF;


typedef struct TEXTUREBASED_API s_AstronomicalTime
{
    s_AstronomicalTime();

    s_AstronomicalTime(
        const short year
    ,   const short month
    ,   const short day
    ,   const short hour
    ,   const short minute
    ,   const short second
    ,   const short utcOffset = 0);


    s_AstronomicalTime(
        const short year
    ,   const short month
    ,   const long double day
    ,   const short utcOffset = 0);

    static const s_AstronomicalTime fromTimeT(
        const time_t &localTime
    ,   const time_t &utcOffset);

    static const s_AstronomicalTime fromTimeF(const TimeF &t);

    const time_t toTime_t() const;

    const long double dayf() const;

    const bool operator==(const s_AstronomicalTime &other) const
    {
        return other.year   == year
            && other.month  == month
            && other.day    == day
            && other.hour   == hour
            && other.minute == minute
            && other.second == second;
    }

public:

    short year;
    short month;
    short day;
    short hour;
    short minute;
    short second;

    short utcOffset; // In seconds.

} t_aTime;

float toDays(int hour, int minute, int second);

} // namespace glHimmel
