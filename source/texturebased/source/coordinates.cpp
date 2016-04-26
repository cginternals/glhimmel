
#include <texturebased/coordinates.h>

namespace glHimmel {


template<typename T>
s_EquatorialCoords<T>::s_EquatorialCoords()
    : right_ascension(0.0)
    , declination(0.0)
    , r(1.0)
{
}


template<typename T>
s_EclipticalCoords<T>::s_EclipticalCoords()
    : longitude(0.0)
    , latitude(0.0)
{
}


template<typename T>
s_HorizontalCoords<T>::s_HorizontalCoords()
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
const s_EclipticalCoords<T> s_EquatorialCoords<T>::toEcliptical(const T obliquity) const
{
    s_EclipticalCoords<T> ecl;

    const T cose(cos(_rad(obliquity)));
    const T sine(sin(_rad(obliquity)));
    const T sina(sin(_rad(right_ascension)));

    ecl.latitude = _deg(atan2(
        sina * cose + tan(_rad(declination)) * sine, cos(_rad(right_ascension))));

    ecl.longitude = _deg(asin(
        sin(_rad(declination)) * cose - cos(_rad(declination)) * sine * sina));

    return ecl;
}


// (AA.12.5) and (AA.12.6)

template<typename T>
const s_HorizontalCoords<T> s_EquatorialCoords<T>::toHorizontal(
    const t_julianDay siderealTime /* θ_0 */
    , const T observersLatitude      /* Φ   */
    , const T observersLongitude     /* L   */) const
{
    s_HorizontalCoords<T> hor;

    // local hour angle: H = θ - α (AA.p88)
    const T H = _rad(siderealTime + observersLongitude - right_ascension);

    const T cosh(cos(H));
    const T sinr(sin(_rad(observersLatitude)));
    const T cosr(cos(_rad(observersLatitude)));

    hor.altitude = _deg(asin(
        sinr * sin(_rad(declination)) + cosr * cos(_rad(declination)) * cosh));

    hor.azimuth = _deg(atan2(static_cast<T>(
        sin(H)), static_cast<T>(cosh * sinr - tan(_rad(declination)) * cosr)));

    return hor;
}


template<typename T>
const glm::vec3 s_EquatorialCoords<T>::toEuclidean() const
{
    const T cosd(cos(_rad(declination)));

    const T x(r * sin(_rad(right_ascension)) * cosd);
    const T y(r * cos(_rad(right_ascension)) * cosd);
    const T z(r * sin(_rad(declination)));

    return glm::vec3(x, y, z);
}


template<typename T>
const s_EquatorialCoords<T> s_EclipticalCoords<T>::toEquatorial(const T obliquity) const
{
    s_EquatorialCoords<T> equ;

    const T cose(cos(_rad(obliquity)));
    const T sine(sin(_rad(obliquity)));

    const T sinl(sin(_rad(longitude)));

    equ.right_ascension = _deg(atan2(
        sinl * cose - tan(_rad(latitude)) * sine, cos(_rad(longitude))));

    equ.declination = _deg(asin(sin(_rad(latitude)) * cose + cos(_rad(latitude)) * sine * sinl));

    return equ;
}


template<typename T>
const s_EquatorialCoords<T> s_HorizontalCoords<T>::toEquatorial(
    const t_julianDay siderealTime /* θ_0 */
    , const T observersLatitude      /* Φ   */
    , const T observersLongitude     /* L   */) const
{
    s_EquatorialCoords<T> equ;

    const T cosa(cos(_rad(altitude)));

    const T sinr(sin(_rad(observersLatitude)));
    const T cosr(cos(_rad(observersLatitude)));

    const T H = _deg(atan2(
        sin(_rad(altitude)), cosa * sinr + tan(_rad(azimuth)) * cosr));

    equ.right_ascension = _hours(siderealTime) - observersLongitude - H;

    equ.declination = _deg(asin(
        sinr * sin(_rad(azimuth)) - cosr * cos(_rad(azimuth)) * cosa));

    return equ;
}


template<typename T>
const glm::vec3 s_HorizontalCoords<T>::toEuclidean() const
{
    const T cosa(cos(_rad(altitude)));

    const T x(sin(_rad(azimuth)) * cosa);
    const T y(cos(_rad(azimuth)) * cosa);
    const T z(sin(_rad(altitude)));

    return glm::vec3(x, y, z);
}

} // namespace glHimmel

