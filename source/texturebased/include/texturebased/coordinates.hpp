#pragma once 
#include <texturebased/coordinates.h>
#include <cmath>
#include <glm/common.hpp>

namespace
{
    float degreesToHours(long double degrees)
    {
        return std::fmod(degrees / 15.l, 24.l);
    }
}


namespace glHimmel {


template<typename T>
EquatorialCoords<T>::EquatorialCoords()
    : right_ascension(0.0)
    , declination(0.0)
    , r(1.0)
{
}


template<typename T>
EclipticalCoords<T>::EclipticalCoords()
    : longitude(0.0)
    , latitude(0.0)
{
}


template<typename T>
HorizontalCoords<T>::HorizontalCoords()
    : azimuth(0.0)
    , altitude(0.0)
{
}


//s_GalacticCoords::s_GalacticCoords()
//:   longitude(0.0)
//,   latitude(0.0)
//{
//}



// To remove 180° ambiquity, atan2 is applied ("Astronomical Algorithms")

template<typename T>
const EclipticalCoords<T> EquatorialCoords<T>::toEcliptical(const T obliquity) const
{
    EclipticalCoords<T> ecl;

    const T cose(std::cos(glm::radians(obliquity)));
    const T sine(std::sin(glm::radians(obliquity)));
    const T sina(std::sin(glm::radians(right_ascension)));

    ecl.latitude = glm::degrees(std::atan2(
        sina * cose + std::tan(glm::radians(declination)) * sine, std::cos(glm::radians(right_ascension))));

    ecl.longitude = glm::degrees(std::asin(
        std::sin(glm::radians(declination)) * cose - std::cos(glm::radians(declination)) * sine * sina));

    return ecl;
}


// (AA.12.5) and (AA.12.6)

template<typename T>
const HorizontalCoords<T> EquatorialCoords<T>::toHorizontal(
    const t_julianDay siderealTime /* θ_0 */
    , const T observersLatitude      /* Φ   */
    , const T observersLongitude     /* L   */) const
{
    HorizontalCoords<T> hor;

    // local hour angle: H = θ - α (AA.p88)
    const T H = glm::radians(siderealTime + observersLongitude - right_ascension);

    const T cosh(std::cos(H));
    const T sinr(std::sin(glm::radians(observersLatitude)));
    const T cosr(std::cos(glm::radians(observersLatitude)));

    hor.altitude = glm::degrees(std::asin(
        sinr * std::sin(glm::radians(declination)) + cosr * std::cos(glm::radians(declination)) * cosh));

    hor.azimuth = glm::degrees(std::atan2(static_cast<T>(
        sin(H)), static_cast<T>(cosh * sinr - std::tan(glm::radians(declination)) * cosr)));

    return hor;
}


template<typename T>
const glm::vec3 EquatorialCoords<T>::toEuclidean() const
{
    const T cosd(std::cos(glm::radians(declination)));

    const T x(r * std::sin(glm::radians(right_ascension)) * cosd);
    const T y(r * std::cos(glm::radians(right_ascension)) * cosd);
    const T z(r * std::sin(glm::radians(declination)));

    return glm::vec3(x, y, z);
}


template<typename T>
const EquatorialCoords<T> EclipticalCoords<T>::toEquatorial(const T obliquity) const
{
    EquatorialCoords<T> equ;

    const T cose(std::cos(glm::radians(obliquity)));
    const T sine(std::sin(glm::radians(obliquity)));

    const T sinl(std::sin(glm::radians(longitude)));

    equ.right_ascension = glm::degrees(std::atan2(
        sinl * cose - std::tan(glm::radians(latitude)) * sine, std::cos(glm::radians(longitude))));

    equ.declination = glm::degrees(std::asin(std::sin(glm::radians(latitude)) * cose + cos(glm::radians(latitude)) * sine * sinl));

    return equ;
}


template<typename T>
const EquatorialCoords<T> HorizontalCoords<T>::toEquatorial(
    const t_julianDay siderealTime /* θ_0 */
    , const T observersLatitude      /* Φ   */
    , const T observersLongitude     /* L   */) const
{
    EquatorialCoords<T> equ;

    const T cosa(std::cos(glm::radians(altitude)));

    const T sinr(std::sin(glm::radians(observersLatitude)));
    const T cosr(std::cos(glm::radians(observersLatitude)));

    const T H = glm::degrees(std::atan2(
        std::sin(glm::radians(altitude)), cosa * sinr + std::tan(glm::radians(azimuth)) * cosr));

    equ.right_ascension = degreesToHours(siderealTime) - observersLongitude - H;

    equ.declination = glm::degrees(std::asin(
        sinr * std::sin(glm::radians(azimuth)) - cosr * std::cos(glm::radians(azimuth)) * cosa));

    return equ;
}


template<typename T>
const glm::vec3 HorizontalCoords<T>::toEuclidean() const
{
    const T cosa(std::cos(glm::radians(altitude)));

    const T x(std::sin(glm::radians(azimuth)) * cosa);
    const T y(std::cos(glm::radians(azimuth)) * cosa);
    const T z(std::sin(glm::radians(altitude)));

    return glm::vec3(x, y, z);
}

} // namespace glHimmel

