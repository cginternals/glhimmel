
#pragma once

#include <glhimmel-base/interpolate.h>
#include <glhimmel-texturebased/glhimmel-texturebased_api.h>
#include <globjects/Texture.h>

#include <map>
#include <glbinding/gl/types.h>
#include <globjects/base/ref_ptr.h>


namespace glHimmel
{

class GLHIMMEL_TEXTUREBASED_API TwoUnitsChanger
{
public:
    TwoUnitsChanger();
    virtual ~TwoUnitsChanger();

    // Specify the duration in timef for blending between two textures.
    // To avoid faulty blending, this should be smaller than the smallest 
    // time range, since only two textures will be blended at a time.

    float setTransitionDuration(const float duration);
    float getTransitionDuration() const;

    void setInterpolationMethod(const InterpolationMethod method);
    InterpolationMethod getInterpolationMethod() const;

    // Append a time range using the given texture unit. Example: If you
    // add unit 0 at time 0.2 and unit 1 at 0.8, then with a 
    // transitionDuration of 0.1 the following blending will occur:

    // time  0.0  0.1  0.2  0.3  0.4  0.5  0.6  0.7  0.8  0.9  1.0/0.0
    // unit   1    1   1/0   0    0    0    0    0   0/1   1    1

    void push(
        globjects::ref_ptr<globjects::Texture> texture
    ,   const float time = 1.f);


    // Getter (they call update if required).

    globjects::ref_ptr<globjects::Texture> getBack (const float time) const;
    globjects::ref_ptr<globjects::Texture> getSrc  (const float time) const;

    float getSrcAlpha  (const float time) const;

protected:

    void update(const float time) const;

    void updateSmallestRange();
    void checkAndAdjustTransitionDuration();

    // This is required, since the transition duration can change, 
    // and the back units by time change.
    void updateBackUnits() const; 

protected:

    float m_transitionDuration;
    InterpolationMethod m_interpolationMethod;

    typedef std::map<float, globjects::ref_ptr<globjects::Texture>> t_unitsByTime;

    t_unitsByTime m_unitsByTime;
    mutable t_unitsByTime m_backUnitsByTime;

    float m_smallestRange;

    // cache (make mutable to provide const getter)

    mutable bool m_updated; // helper for first update
    mutable float m_lastTime;

    mutable globjects::ref_ptr<globjects::Texture> m_back;
    mutable globjects::ref_ptr<globjects::Texture> m_src;

    mutable float m_srcAlpha;
};

} // namespace glHimmel
