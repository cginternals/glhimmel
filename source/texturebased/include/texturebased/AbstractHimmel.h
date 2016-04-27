
#pragma once

#include <texturebased/texturebased_api.h>
#include <memory>


namespace glHimmel
{
class TimeF;

class TEXTUREBASED_API AbstractHimmel
{
public:
    AbstractHimmel();
    virtual ~AbstractHimmel();

    void assignTime(std::unique_ptr<TimeF> timef);
    TimeF* getTime() const;

    void update();
    void initialize();

protected:
    float timef() const;
    virtual void postInitialize() = 0;

protected:
    std::unique_ptr<TimeF> m_timef;
    bool m_initialized;
    bool m_autoUpdateTime;
    bool m_dirty;
};

} // namespace osgHimmel
