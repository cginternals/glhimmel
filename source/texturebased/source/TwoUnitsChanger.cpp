
#include <texturebased/TwoUnitsChanger.h>

#include <osg/Notify>


namespace glHimmel
{

TwoUnitsChanger::TwoUnitsChanger()
:   m_transitionDuration(1.f)
,   m_smallestRange(1.f)
,   m_updated(false)
,   m_lastTime(0.f)
,   m_back(0)
,   m_src(0)
,   m_srcAlpha(0.f)

,   m_interpolationMethod(IM_Linear)
{
}


TwoUnitsChanger::~TwoUnitsChanger()
{

}


const float TwoUnitsChanger::setTransitionDuration(const float duration)
{
    m_transitionDuration = std::min<float>(std::max<float>(duration, 0.0), 1.0);
    checkAndAdjustTransitionDuration();

    return getTransitionDuration();
}


void TwoUnitsChanger::pushUnit(
    const GLint unit
,   const float time)
{
    const float t = std::min<float>(std::max<float>(time, 0.0), 1.0);

    // check if time is valid
    if(m_unitsByTime.lower_bound(t) != m_unitsByTime.end())
    {
        osg::notify(osg::WARN) << "Attempt to push unit before the last unit was ignored." << std::endl;
        return;
    }

    m_unitsByTime[t] = unit;
    m_backUnitsByTime.clear();

    updateSmallestRange();
}


void TwoUnitsChanger::updateSmallestRange()
{
    m_smallestRange = 1.f;

    if(m_unitsByTime.size() < 2)
        return;

    const t_unitsByTime::const_iterator ie(m_unitsByTime.end());
    
    t_unitsByTime::const_iterator i(m_unitsByTime.begin());
    t_unitsByTime::const_iterator i0(i), i1(i);

    // Iterate over all time keys, and retrieve the smallest range.
    for(; i != ie; ++i)
    {
        ++i1;

        // The last key has to be compared with the first.
        if(i1 == ie)
            i1 = i0;

        const float range = _abs(i1->first - i->first);

        if(range < m_smallestRange)
            m_smallestRange = range;
    }
    checkAndAdjustTransitionDuration();
}


void TwoUnitsChanger::checkAndAdjustTransitionDuration()
{
    if(m_transitionDuration > m_smallestRange)
    {
        m_transitionDuration = m_smallestRange;
        osg::notify(osg::WARN) << "Transition duration was bigger than smallest time range. Set to smallest range." << std::endl;
    }
}


void TwoUnitsChanger::updateBackUnits() const
{
    t_unitsByTime::const_iterator i(m_unitsByTime.begin());
    const t_unitsByTime::const_iterator ie(m_unitsByTime.end());

    for(; i != ie; ++i)
    {
        float t(i->first + m_transitionDuration * 0.5f);

        if(t >= 1.f)
            t -= 1.f;

        m_backUnitsByTime[t] = i->second;
    }
}


void TwoUnitsChanger::update(const float time) const
{
    if(time == m_lastTime && m_updated)
        return;

    if(!m_updated)
        m_updated = true;

    m_lastTime = time;

    // If no or only one unit is assigned do nothing.
    if(m_unitsByTime.size() < 2)
    {
        m_back = 0;
        m_src  = 0;
        m_srcAlpha = 0.f;

        return;
    }

    // Update back units if required.
    if(m_unitsByTime.size() != m_backUnitsByTime.size())
        updateBackUnits();

    const t_unitsByTime::const_iterator ie(m_backUnitsByTime.end());
    const t_unitsByTime::const_iterator ib(m_backUnitsByTime.begin());
    t_unitsByTime::const_iterator il = ie; --il; // last

    t_unitsByTime::const_iterator isrc(m_backUnitsByTime.upper_bound(time));
    t_unitsByTime::const_iterator iback;

    if(isrc == ie || isrc == ib)
    {
        // If time is after last or before first key-point, src 
        // is at first key-point, and back is at last key-point.
        isrc  = ib;
        iback = il;
    }
    else
    {
        iback = isrc;
        std::advance(iback, -1);
    }

    if(iback->second == isrc->second)
        m_srcAlpha = 0.f;
    else
    {
        const float tsrc(isrc->first);
        const float t = tsrc < time ? time - 1.f : time;

        if(m_transitionDuration != 0.f)
        {
            float a = 1.f - (tsrc - t) / m_transitionDuration;
            a = std::min<float>(std::max<float>(a, 0.f), 1.f);

            m_srcAlpha = interpolate<float>(a, m_interpolationMethod);
        }
        else
            m_srcAlpha = 0.f;
    }

    // NOTE: This could also be optimized for 8 or less textures by not 
    // switching their texture unit and adapting the shader: "EightUnitsChanger" :P

    m_back = iback->second;
    m_src  = isrc->second;
}


const GLint TwoUnitsChanger::getBackUnit(const float time) const
{
    update(time);
    return m_back;
}


const GLint TwoUnitsChanger::getSrcUnit(const float time) const
{
    update(time);
    return m_src;
}


const float TwoUnitsChanger::getSrcAlpha(const float time) const
{
    update(time);
    return m_srcAlpha;
}

} // namespace glHimmel