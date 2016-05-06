#include <texturebased/AbstractHimmel.h>
#include <texturebased/Timef.h>

namespace glHimmel {
AbstractHimmel::AbstractHimmel()
:   m_timef(nullptr)
,   m_initialized(false)
,   m_autoUpdateTime(false)
,   m_dirty(false)
,   m_lastElapsed(0.0)
{
}

AbstractHimmel::~AbstractHimmel()
{
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

void AbstractHimmel::draw()
{
    update();
}

float AbstractHimmel::timef() const
{
    if (m_timef)
        return m_timef->getf();

    return 0.f;
}

void AbstractHimmel::dirty(bool dirty)
{
    m_dirty = dirty;
}

bool AbstractHimmel::isDirty()
{
    return m_dirty;
}


} // namespace glhimmel