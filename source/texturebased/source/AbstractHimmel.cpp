#include <texturebased/AbstractHimmel.h>
#include <texturebased/Timef.h>

namespace glHimmel {
AbstractHimmel::AbstractHimmel()
:   m_timef(nullptr)
,   m_initialized(false)
,   m_autoUpdateTime(false)
,   m_dirty(false)
{
}

AbstractHimmel::~AbstractHimmel()
{
}

void AbstractHimmel::initialize()
{
    if (m_initialized)
        return;

    postInitialize();

    m_initialized = true;
}

void AbstractHimmel::assignTime(std::unique_ptr<TimeF> timef)
{
    m_timef = std::move(timef);
}

TimeF* AbstractHimmel::getTime() const
{
    return m_timef.get();
}

void AbstractHimmel::update()
{
    if (!m_initialized)
        initialize();

    if (m_autoUpdateTime && m_timef)
    {
        m_timef->update();
        dirty();
    }


    if (!m_timef)
        return;

    const long double elapsed(m_timef->getNonModf());
    if (elapsed != m_lastElapsed)
    {
        dirty();
        m_lastElapsed = elapsed;
    }
}

float AbstractHimmel::timef() const
{
    if (m_timef)
        return m_timef->getf();

    return 0.f;
}

} // namespace glhimmel