
#pragma once
#ifndef __TWOUNITSCHANGER_H__
#define __TWOUNITSCHANGER_H__

#include <glhimmel-texturebased/glhimmel-texturebased_api.h>
#include <glhimmel-texturebased/interpolate.h>

#include <osg/GL>

#include <map>


namespace glhimmel_texturebased
{

class GLHIMMEL_TEXTUREBASED_API TwoUnitsChanger
{
public:
    TwoUnitsChanger();
    virtual ~TwoUnitsChanger();

    // Specify the duration in timef for blending between two textures.
    // To avoid faulty blending, this should be smaller than the smallest 
    // time range, since only two textures will be blended at a time.

    const float setTransitionDuration(const float duration);
    inline const float getTransitionDuration() const
    {
        return m_transitionDuration;
    }


    void setInterpolationMethod(const e_InterpolationMethod method);
    inline const e_InterpolationMethod getInterpolationMethod() const
    {
        return m_interpolationMethod;
    }


    // Append a time range using the given texture unit. Example: If you
    // add unit 0 at time 0.2 and unit 1 at 0.8, then with a 
    // transitionDuration of 0.1 the following blending will occur:

    // time  0.0  0.1  0.2  0.3  0.4  0.5  0.6  0.7  0.8  0.9  1.0/0.0
    // unit   1    1   1/0   0    0    0    0    0   0/1   1    1

    void pushUnit(
        const GLint unit
    ,   const float time = 1.f);


    // Getter (they call update if required).

    const GLint getBackUnit (const float time) const;
    const GLint getSrcUnit  (const float time) const;

    const float getSrcAlpha  (const float time) const;

    const bool hasBackChanged(const float time) const;
    const bool hasSrcChanged (const float time) const;

protected:

    void update(const float time) const;

    void updateSmallestRange();
    void checkAndAdjustTransitionDuration();

    // This is required, since the transition duration can change, 
    // and the back units by time change.
    void updateBackUnits() const; 

protected:

    float m_transitionDuration;
    e_InterpolationMethod m_interpolationMethod;

    typedef std::map<float, GLint> t_unitsByTime;

    t_unitsByTime m_unitsByTime;
    mutable t_unitsByTime m_backUnitsByTime;

    float m_smallestRange;

    // cache (make mutable to provide const getter)

    mutable bool m_updated; // helper for first update
    mutable float m_lastTime;

    mutable GLint m_back;
    mutable GLint m_src;

    mutable float m_srcAlpha;
};

} // namespace glhimmel_texturebased

#endif // __TWOUNITSCHANGER_H__