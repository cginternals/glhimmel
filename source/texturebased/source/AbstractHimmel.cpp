#include <texturebased/AbstractHimmel.h>
#include <texturebased/Timef.h>

namespace glHimmel {

    TimeF* AbstractHimmel::getTime() const
    {
        return m_timef.get();
    }


    float AbstractHimmel::timef() const
    {
        if (m_timef)
            return m_timef->getf();

        return 0.f;
    }

}