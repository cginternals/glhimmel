
#pragma once
#ifndef __TIMEF_H__
#define __TIMEF_H__

#include <glhimmel-texturebased/glhimmel-texturebased_api.h>
#include <glhimmel-texturebased/typedefs.h>

#include <time.h>

namespace osg
{
    class Timer;
}


namespace glhimmel_texturebased
{

// TimeF manages an osg::Timer and features an interface for floating time
// in the closed interval [0;1] representing a full day and standard c 
// time (time_t) simultaneously. The time updates have to be requested 
// explicitly, thus simplifying usage between multiple recipients.
// The time starts cycling automatically, but can also be paused, stopped, 
// or set to a specific value.

class GLHIMMEL_TEXTUREBASED_API TimeF
{
    enum e_Mode
    {
        M_Running
    ,   M_Pausing
    };

public:
    
    static const long utcOffset();


    TimeF(
        const t_longf time = 0.0
    ,   const t_longf secondsPerCycle = 0.0);

    TimeF(
        const time_t &time
    ,   const time_t &utcOffset // In seconds (UTC+01:00 is m_utcOffset = +3600).
    ,   const t_longf secondsPerCycle = 0.0);

    ~TimeF();

    // Increments time appropriate to secondsPerCycle.
    void update();

    // Cycling manipulation - does not tamper the time.

    void start(const bool forceUpdate = false);
    void pause(const bool forceUpdate = false);
    void reset(const bool forceUpdate = false); // Resets the time to initial value (secondsPerCycle remain unchanged).
    void stop (const bool forceUpdate = false); // Stops and resets the time.

    inline const t_longf getSecondsPerCycle() const
    {
        return m_secondsPerCycle;
    }

    const t_longf setSecondsPerCycle(const t_longf secondsPerCycle);


    // Float time in the intervall [0;1]
    inline const t_longf getf() const
    {
        return m_timef[1];
    }

    const t_longf getf(const bool updateFirst);

    // Sets only time, date remains unchanged.
    const t_longf setf(
        t_longf time
    ,   const bool forceUpdate = false);

    // Elapsed float time from initialized time.
    const t_longf getNonModf(const bool updateFirst = false);

    // Time in seconds from initial time.
    inline const time_t gett() const
    {
        return m_time[1] + utcOffset();
    }

    const time_t gett(const bool updateFirst);
    const time_t sett(
        const time_t &time
    ,   const bool forceUpdate = false);

    const time_t getUtcOffset() const;
    const time_t setUtcOffset(const time_t &utcOffset /* In Seconds. */);

    // 

    const bool isRunning() const;

protected:
    static inline const t_longf secondsTof(const time_t &time);
    static inline const time_t fToSeconds(const t_longf time);

    void initialize();

protected:
    osg::Timer *m_timer;

    time_t m_utcOffset;

    time_t m_time[3];       // [2] is for stop
    t_longf m_timef[3]; // [2] is for stop

    t_longf m_offset;

    e_Mode m_mode;
    t_longf m_lastModeChangeTime;

    t_longf m_secondsPerCycle;
};

} // namespace glhimmel_texturebased

#endif // __TIMEF_H__