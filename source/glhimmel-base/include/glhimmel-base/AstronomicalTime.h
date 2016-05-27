#pragma once

#include <glhimmel-base/glhimmel-base_api.h>
#include <ctime>

namespace glHimmel
{

class TimeF;


typedef struct GLHIMMEL_BASE_API AstronomicalTime
{
    AstronomicalTime();

    AstronomicalTime(
        const short year
    ,   const short month
    ,   const short day
    ,   const short hour
    ,   const short minute
    ,   const short second
    ,   const short utcOffset = 0);


    AstronomicalTime(
        const short year
    ,   const short month
    ,   const long double day
    ,   const short utcOffset = 0);

    static AstronomicalTime fromTimeT(
        const time_t &localTime
    ,   const time_t &utcOffset);

    static AstronomicalTime fromTimeF(const TimeF &t);

    time_t toTime_t() const;

    long double dayf() const;

    bool operator==(const AstronomicalTime &other) const
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

} astronomicalTime;

float toDays(int hour, int minute, int second);

} // namespace glHimmel
