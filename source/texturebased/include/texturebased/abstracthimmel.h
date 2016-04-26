
#pragma once

#include <texturebased/texturebased_api.h>


namespace glHimmel
{
class TimeF;

class TEXTUREBASED_API AbstractHimmel
{
public:
    AbstractHimmel();
    virtual ~AbstractHimmel();

    void assignTime(TimeF);
    void update();

};

} // namespace osgHimmel
