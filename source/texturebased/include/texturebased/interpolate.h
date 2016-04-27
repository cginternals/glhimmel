
#pragma once

#include <cmath>
#include <glm/gtc/constants.hpp>


namespace glHimmel
{

    template<typename T>
    T linear(T t)
    {
        return t;
    }

    template<typename T>
    T smoothstep(T t)
    {
        return t * t * (3 - 2 * t);
    }

    template<typename T>
    T smoothstep2(T t)
    {
        return smoothstep(smoothstep(t));
    }

    template<typename T>
    T smoothstep3(T t)
    {
        return smoothstep(smoothstep2(t));
    }

    template<typename T>
    T smootherstep(T t)
    {
        return t * t * t * (t * (6 * t - 15) + 10);
    }

    template<typename T>
    T squared(T t)
    {
        return t * t;
    }

    template<typename T>
    T invsquared(T t)
    {
        return 1 - (1 - t) * (1 - t);
    }

    template<typename T>
    T cubed(T t)
    {
        return t * t * t;
    }

    template<typename T>
    T invcubed(T t)
    {
        return 1 - (1 - t) * (1 - t) * (1 - t);
    }

    template<typename T>
    T sin(T t)
    {
        return std::sin(t * glm::pi<T>() / 2);
    }

    template<typename T>
    T invsin(T t)
    {
        return 1 - std::sin((1 - t) * glm::pi<T>() / 2);
    }

    template<typename T>
    T smoothstepExt(T t, T l, T r)
    {
        return t < l ? 0 : r < t ? 1 : smoothstep((t - l) / (r - l));
    }

// Several interpolation methods in action: http://sol.gfxile.net/interpolation/

enum class InterpolationMethod
{
    Linear
,   SmoothStep
,   SmoothStep2
,   SmoothStep3
,   SmootherStep // Ken Perlin
,   Squared
,   InvSquared
,   Cubed
,   InvCubed
,   Sin          // strong in, soft out
,   InvSin       // soft in, strong out
};


template<typename T>
inline const T interpolate(
    const T t
,   const InterpolationMethod function = InterpolationMethod::Linear)
{
    switch(function)
    {
    case InterpolationMethod::SmoothStep:
        return smoothstep(t);
    case InterpolationMethod::SmoothStep2:
        return smoothstep2(t);
    case InterpolationMethod::SmoothStep3:
        return smoothstep3(t);
    case InterpolationMethod::SmootherStep:
        return smootherstep(t);
    case InterpolationMethod::Squared:
        return squared(t);
    case InterpolationMethod::InvSquared:
        return invsquared(t);
    case InterpolationMethod::Cubed:
        return cubed(t);
    case InterpolationMethod::InvCubed:
        return invcubed(t);
    case InterpolationMethod::Sin:
        return static_cast<T>(sin(t));
    case InterpolationMethod::InvSin:
        return static_cast<T>(invsin(t));
    default:
    case InterpolationMethod::Linear:
        return linear(t);
    }
}

} // namespace glHimmel
