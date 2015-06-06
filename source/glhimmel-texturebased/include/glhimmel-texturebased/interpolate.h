
#pragma once
#ifndef __INTERPOLATE_H__
#define __INTERPOLATE_H__

#include <math.h>


namespace glhimmel_texturebased
{

#define _linear(t)       (t)

#define _smoothstep(t)   ((t) * (t) * (3 - 2 * (t)))
#define _smoothstep2(t)  (_smoothstep(_smoothstep(t)))
#define _smoothstep3(t)  (_smoothstep(_smoothstep2(t)))

#define _smootherstep(t) ((t) * (t) * (t) * ((t) * (6 * (t) - 15) + 10))

#define _squared(t)      ((t) * (t))
#define _invsquared(t)   (1 - (1 - (t)) * (1 - (t)))

#define _cubed(t)        ((t) * (t) * (t))
#define _invcubed(t)     (1 - (1 - (t)) * (1 - (t)) * (1 - (t)))

#define _sin(t)          (sin(t * 1.57079632679489661923))
#define _invsin(t)       (1 - sin((1 - (t)) * 1.57079632679489661923))


#define _smoothstep_ext(t, l, r) \
    ((t) < (l) ? 0 : (r) < (t) ? 1 : _smoothstep(((t) - (l)) / ((r) - (l))))

// Several interpolation methods in action: http://sol.gfxile.net/interpolation/

enum e_InterpolationMethod
{
    IM_Linear
,   IM_SmoothStep
,   IM_SmoothStep2
,   IM_SmoothStep3
,   IM_SmootherStep // Ken Perlin
,   IM_Squared
,   IM_InvSquared
,   IM_Cubed
,   IM_InvCubed
,   IM_Sin          // strong in, soft out
,   IM_InvSin       // soft in, strong out
};


template<typename T>
inline const T interpolate(
    const T t
,   const e_InterpolationMethod function = IM_Linear)
{
    switch(function)
    {
    case IM_SmoothStep:
        return _smoothstep(t);
    case IM_SmoothStep2:
        return _smoothstep2(t);
    case IM_SmoothStep3:
        return _smoothstep3(t);
    case IM_SmootherStep:
        return _smootherstep(t);
    case IM_Squared:
        return _squared(t);
    case IM_InvSquared:
        return _invsquared(t);
    case IM_Cubed:
        return _cubed(t);
    case IM_InvCubed:
        return _invcubed(t);
    case IM_Sin:
        return static_cast<T>(_sin(t));
    case IM_InvSin:
        return static_cast<T>(_invsin(t));
    default:
    case IM_Linear:
        return _linear(t);
    }
}

} // namespace glhimmel_texturebased

#endif // __INTERPOLATE_H__