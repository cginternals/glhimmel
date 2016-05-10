#pragma once

#include <texturebased/texturebased_api.h>
#include <memory>
#include <glm/mat4x4.hpp>


namespace glHimmel
{
class TimeF;

class TEXTUREBASED_API AbstractHimmel
{
public:
    AbstractHimmel();
    virtual ~AbstractHimmel();

    virtual void initialize() = 0;

    void assignTime(std::unique_ptr<TimeF> timef);
    TimeF* getTime() const;

    virtual void draw();

    virtual bool isDirty();
    void dirty(bool dirty = true);

    void setProjection(glm::mat4 projection);

protected:
    float timef() const;
    virtual void update();

protected:
    std::unique_ptr<TimeF> m_timef;
    bool m_initialized;
    bool m_autoUpdateTime;
    bool m_dirty;
    long double m_lastElapsed;
    glm::mat4 m_projection;
};

} // namespace osgHimmel
