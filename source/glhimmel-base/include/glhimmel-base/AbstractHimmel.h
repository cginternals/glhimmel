#pragma once

#include <glhimmel-base/glhimmel-base_api.h>
#include <memory>
#include <glm/mat4x4.hpp>


namespace glHimmel
{
class TimeF;

class GLHIMMEL_BASE_API AbstractHimmel
{
public:
    AbstractHimmel();
    virtual ~AbstractHimmel();

    virtual void initialize() = 0;

    void assignTime(std::shared_ptr<TimeF> timef);
    TimeF* getTime() const;

    virtual void draw();

    virtual bool isDirty();
    void dirty(bool dirty = true);

    void setProjection(const glm::mat4 & projection
        , float fov, unsigned int viewportWidth, unsigned int viewportHeight);
    void setView(const glm::mat4 & view);

    const glm::mat4 & getProjection() const;
    const glm::mat4 & getView() const;

    float getCameraFovHint() const;
    unsigned int getViewSizeWidthHint() const;
    unsigned int getViewSizeHeightHint() const;

protected:
    float timef() const;
    virtual void update();

protected:
    std::shared_ptr<TimeF> m_timef;
    bool m_initialized;
    bool m_autoUpdateTime;
    bool m_dirty;
    long double m_lastElapsed;
    glm::mat4 m_projection;
    glm::mat4 m_view;
    float m_fov;
    unsigned int m_viewportWidth
        , m_viewportHeight;

};

} // namespace osgHimmel
