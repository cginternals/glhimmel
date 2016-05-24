#include <glhimmel-base/AbstractHimmel.h>
#include <glhimmel-base/Timef.h>

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


void AbstractHimmel::assignTime(std::shared_ptr<TimeF> timef)
{
    m_timef = timef;
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

void AbstractHimmel::setProjection(const glm::mat4 & projection)
{
    m_projection = projection;
}

void AbstractHimmel::setView(const glm::mat4 & view)
{
    m_view = view;
}

bool AbstractHimmel::isDirty()
{
    return m_dirty;
}


} // namespace glhimmel