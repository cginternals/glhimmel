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
        return static_cast<float>(m_timef->getf());

    return 0.f;
}

void AbstractHimmel::dirty(bool dirty)
{
    m_dirty = dirty;
}

void AbstractHimmel::setProjection(const glm::mat4 & projection
    , float fov, unsigned int viewportWidth, unsigned int viewportHeight)
{
    m_projection = projection;
    m_fov = fov;
    m_viewportWidth = viewportWidth;
    m_viewportHeight = viewportHeight;
}

void AbstractHimmel::setView(const glm::mat4 & view)
{
    m_view = view;
}

const glm::mat4 & AbstractHimmel::getProjection() const
{
    return m_projection;
}

const glm::mat4 & AbstractHimmel::getView() const
{
    return m_view;
}

float AbstractHimmel::getCameraFovHint() const
{
    return m_fov;
}

unsigned int AbstractHimmel::getViewSizeWidthHint() const
{
    return m_viewportWidth;
}

unsigned int AbstractHimmel::getViewSizeHeightHint() const
{
    return m_viewportHeight;
}

bool AbstractHimmel::isDirty()
{
    return m_dirty;
}


} // namespace glhimmel