

#include "texturebased/Timef.h"

//#include "mathmacros.h"

#include <cmath>
#include <glm/common.hpp>


namespace glHimmel
{

long TimeF::utcOffset()
{
    long tz;

#ifdef __GNUC__
    tz = timezone;
#else // __GNUC__
    _get_timezone(&tz);
#endif // __GNUC__

    return tz;
}


TimeF::TimeF(
    const long double time
,   const long double secondsPerCycle)
:   m_startTime(std::chrono::steady_clock::now())
,   m_secondsPerCycle(secondsPerCycle)
,   m_mode(Mode::Pausing)
,   m_lastModeChangeTime(0.f)
,   m_utcOffset(0)
,   m_offset(0.f)
{
    initialize();
    setf(time, true);
}


TimeF::TimeF(
    const time_t &time
,   const time_t &utcOffset
,   const long double secondsPerCycle)
:   m_startTime(std::chrono::steady_clock::now())
,   m_secondsPerCycle(secondsPerCycle)
,   m_mode(Mode::Pausing)
,   m_lastModeChangeTime(0.f)
,   m_utcOffset(utcOffset)
,   m_offset(0.f)
{
    initialize();
    sett(time, true);
}

void TimeF::initialize()
{
    m_lastModeChangeTime = elapsedTime();

    m_timef[0] = 0.f;
    m_timef[1] = 0.f;
    m_timef[2] = 0.f;

    m_time[0]  = 0;
    m_time[1]  = 0;
    m_time[2]  = 0;
}

long double TimeF::elapsedTime()
{
    auto now = std::chrono::steady_clock::now();
    auto duration = now - m_startTime;
    return duration.count();
}


TimeF::~TimeF()
{
}


void TimeF::update()
{
    const long double elapsed(Mode::Running == m_mode ? elapsedTime() : m_lastModeChangeTime);

    const long double elapsedTimef(m_secondsPerCycle > 0.f ? elapsed / m_secondsPerCycle : 0.f);

    m_timef[1] = glm::fract(m_timef[0] + elapsedTimef + m_offset);

    m_time[1] = fToSeconds(elapsedTimef + m_offset) + static_cast<long double>(m_time[0]);
}


long double TimeF::getf(const bool updateFirst)
{
    if(updateFirst)
        update();

    return m_timef[1];
}

long double TimeF::setf(
    long double timef
,   const bool forceUpdate)
{
    timef = glm::fract(timef);

    if(1.f == timef)
        timef = 0.f;

    m_timef[0] = timef;
    m_timef[2] = m_timef[0];

    m_offset = 0;

    const time_t seconds(fToSeconds(timef));

#ifdef __GNUC__
    struct tm lcl(*localtime(&m_time[1]));
#else // __GNUC__
    struct tm lcl;
    localtime_s(&lcl, &m_time[1]);
#endif // __GNUC__

    lcl.tm_hour = seconds / 3600;
    lcl.tm_min  = seconds % 3600 / 60;
    lcl.tm_sec  = seconds % 60;

    time_t mt = mktime(&lcl);
    if(mt == -1)
        m_time[0] = m_time[2] = 0;
    else
        m_time[0] = m_time[2] = mktime(&lcl) - utcOffset();

    reset(forceUpdate);

    return getf();
}


long double TimeF::getNonModf(const bool updateFirst)
{
    return secondsTof(gett(updateFirst));
}


time_t TimeF::gett(const bool updateFirst)
{
    if(updateFirst)
        update();

    return m_time[1] + utcOffset();
}


time_t TimeF::sett(
    const time_t &time
,   const bool forceUpdate)
{
    time_t t = time - utcOffset();

    m_time[0] = t;
    m_time[2] = m_time[0];

    m_timef[0] = glm::fract(secondsTof(t));
    m_timef[2] = m_timef[0];

    m_offset = 0;

    reset(forceUpdate);

    return gett();
}

long double TimeF::getSecondsPerCycle() const
{
    return m_secondsPerCycle;
}

long double TimeF::setSecondsPerCycle(const long double secondsPerCycle)
{
    // intepret elapsed seconds within new cycle time
    const long double elapsed(Mode::Running == m_mode ? elapsedTime() : m_lastModeChangeTime);

    if(m_secondsPerCycle > 0.f)
        m_offset += elapsed / m_secondsPerCycle;

    m_lastModeChangeTime = 0;

    m_secondsPerCycle = secondsPerCycle;
    m_startTime = std::chrono::steady_clock::now();

    return getSecondsPerCycle();
}


long double TimeF::secondsTof(const time_t &time)
{
    return static_cast<long double>((time) / (60.0 * 60.0 * 24.0));
}


time_t TimeF::fToSeconds(const long double time)
{
    return static_cast<time_t>(time * 60.0 * 60.0 * 24.0 + 0.1);
}


bool TimeF::isRunning() const
{
    return Mode::Running == m_mode;
}


void TimeF::start(const bool forceUpdate)
{
    if(Mode::Pausing != m_mode)
        return;

    const long double t(elapsedTime());

    if(m_secondsPerCycle > 0.f)
        m_offset -= (t - m_lastModeChangeTime) / m_secondsPerCycle;

    m_mode = Mode::Running;

    if(forceUpdate)
        update();
}


void TimeF::pause(const bool forceUpdate)
{
    if(Mode::Running != m_mode)
        return;

    m_lastModeChangeTime = elapsedTime();

    m_mode = Mode::Pausing;

    if(forceUpdate)
        update();
}


void TimeF::reset(const bool forceUpdate)
{
    m_offset = 0.f;
    m_lastModeChangeTime = 0.f;

    m_timef[0] = m_timef[2];
    m_time[0] = m_time[2];

    m_startTime = std::chrono::steady_clock::now();

    if(forceUpdate)
        update();
}


void TimeF::stop(const bool forceUpdate)
{
    pause();
    reset(forceUpdate);
}


time_t TimeF::getUtcOffset() const
{
    return m_utcOffset;
}

time_t TimeF::setUtcOffset(const time_t &utcOffset)
{
    m_utcOffset = utcOffset;

    return getUtcOffset();
}

} // namespace osgHimmel
