
#include <glhimmel-texturebased/timef.h>

#include <glhimmel-texturebased/mathmacros.h>

#include <osg/Timer>

#include <math.h>


namespace glhimmel_texturebased
{

const long TimeF::utcOffset()
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
    const t_longf time
,   const t_longf secondsPerCycle)
:   m_timer(new osg::Timer())
,   m_secondsPerCycle(secondsPerCycle)
,   m_mode(M_Pausing)
,   m_offset(0.f)
,   m_lastModeChangeTime(0.f)
,   m_utcOffset(0)
{
    initialize();
    setf(time, true);
}


TimeF::TimeF(
    const time_t &time
,   const time_t &utcOffset
,   const t_longf secondsPerCycle)
:   m_timer(new osg::Timer())
,   m_secondsPerCycle(secondsPerCycle)
,   m_mode(M_Pausing)
,   m_offset(0.f)
,   m_lastModeChangeTime(0.f)
,   m_utcOffset(utcOffset)
{
    initialize();
    sett(time, true);
}

void TimeF::initialize()
{
    m_lastModeChangeTime = m_timer->time_s();

    m_timef[0] = 0.f;
    m_timef[1] = 0.f;
    m_timef[2] = 0.f;

    m_time[0]  = 0;
    m_time[1]  = 0;
    m_time[2]  = 0;
}


TimeF::~TimeF()
{
    delete m_timer;
}


void TimeF::update()
{
    const t_longf elapsed(M_Running == m_mode ? m_timer->time_s() : m_lastModeChangeTime);

    const t_longf elapsedTimef(m_secondsPerCycle > 0.f ? elapsed / m_secondsPerCycle : 0.f);

    m_timef[1] = _frac(m_timef[0] + elapsedTimef + m_offset);

    m_time[1] = fToSeconds(elapsedTimef + m_offset) + static_cast<t_longf>(m_time[0]);
}


const t_longf TimeF::getf(const bool updateFirst)
{
    if(updateFirst)
        update();

    return m_timef[1];
}

const t_longf TimeF::setf(
    t_longf timef
,   const bool forceUpdate)
{
    timef = _frac(timef);

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


const t_longf TimeF::getNonModf(const bool updateFirst)
{
    return secondsTof(gett(updateFirst));
}


const time_t TimeF::gett(const bool updateFirst)
{
    if(updateFirst)
        update();

    return m_time[1] + utcOffset();
}


const time_t TimeF::sett(
    const time_t &time
,   const bool forceUpdate)
{
    time_t t = time - utcOffset();

    m_time[0] = t;
    m_time[2] = m_time[0];

    m_timef[0] = _frac(secondsTof(t));
    m_timef[2] = m_timef[0];

    m_offset = 0;

    reset(forceUpdate);

    return gett();
}


const t_longf TimeF::setSecondsPerCycle(const t_longf secondsPerCycle)
{
    // intepret elapsed seconds within new cycle time
    const t_longf elapsed(M_Running == m_mode ? m_timer->time_s() : m_lastModeChangeTime);

    if(m_secondsPerCycle > 0.f)
        m_offset += elapsed / m_secondsPerCycle;

    m_lastModeChangeTime = 0;

    m_secondsPerCycle = secondsPerCycle;
    m_timer->setStartTick();

    return getSecondsPerCycle();
}


inline const t_longf TimeF::secondsTof(const time_t &time)
{
    return static_cast<t_longf>((time) / (60.0 * 60.0 * 24.0));
}


inline const time_t TimeF::fToSeconds(const t_longf time)
{
    return static_cast<time_t>(time * 60.0 * 60.0 * 24.0 + 0.1);
}


const bool TimeF::isRunning() const
{
    return M_Running == m_mode;
}


void TimeF::start(const bool forceUpdate)
{
    if(M_Pausing != m_mode)
        return;

    const t_longf t(m_timer->time_s());

    if(m_secondsPerCycle > 0.f)
        m_offset -= (t - m_lastModeChangeTime) / m_secondsPerCycle;

    m_mode = M_Running;

    if(forceUpdate)
        update();
}


void TimeF::pause(const bool forceUpdate)
{
    if(M_Running != m_mode)
        return;

    m_lastModeChangeTime = m_timer->time_s();

    m_mode = M_Pausing;

    if(forceUpdate)
        update();
}
 

void TimeF::reset(const bool forceUpdate)
{
    m_offset = 0.f;
    m_lastModeChangeTime = 0.f;

    m_timef[0] = m_timef[2];
    m_time[0] = m_time[2];

    delete m_timer;
    m_timer = new osg::Timer();

    if(forceUpdate)
        update();
}


void TimeF::stop(const bool forceUpdate)
{
    pause();
    reset(forceUpdate);
}


const time_t TimeF::getUtcOffset() const
{
    return m_utcOffset;
}

const time_t TimeF::setUtcOffset(const time_t &utcOffset)
{
    m_utcOffset = utcOffset;

    return getUtcOffset();
}

} // namespace glhimmel_texturebased