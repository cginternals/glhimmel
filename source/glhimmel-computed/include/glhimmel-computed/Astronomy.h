#pragma once

#include <glhimmel-computed/glhimmel-computed_api.h>
#include "abstractastronomy.h"


namespace glHimmel
{

class GLHIMMEL_COMPUTED_API Astronomy : public AbstractAstronomy
{
public:

    Astronomy();

protected:

    glm::vec3 moonPosition(
        const astronomicalTime &aTime
    ,   const float latitude
    ,   const float longitude
    ,   const bool refractionCorrected) const override;

    glm::vec3 sunPosition(
        const astronomicalTime &aTime
    ,   const float latitude
    ,   const float longitude
    ,   const bool refractionCorrected) const override;

    glm::mat4 moonOrientation(
        const astronomicalTime &aTime
    ,   const float latitude
    ,   const float longitude) const override;

    float earthShineIntensity(
        const astronomicalTime &aTime
    ,   const float latitude
    ,   const float longitude) const override;

    glm::mat4 equToHorTransform(
        const astronomicalTime &aTime
    ,   const float latitude
    ,   const float longitude) const override;

    float sunDistance(const JulianDay t) const override;
    float angularSunRadius(const JulianDay t) const override;

    float moonRadius() const override;

    float moonDistance(const JulianDay t) const override;
    float angularMoonRadius(const JulianDay t) const override;
};

} // namespace glHimmel
