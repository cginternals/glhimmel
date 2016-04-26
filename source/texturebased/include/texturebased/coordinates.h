
#pragma once

#include "julianday.h"

#include <glm/vec3.hpp>

namespace glHimmel
{

// Explained in "Astronomical Algorithms" and http://en.wikipedia.org/wiki/Celestial_coordinate_system

template<typename T>
struct s_EquatorialCoords;

template<typename T>
struct s_EclipticalCoords;

template<typename T>
struct s_HorizontalCoords;

template<typename T>
struct s_GalacticCoords;


// http://en.wikipedia.org/wiki/Equatorial_coordinate_system

template<typename T>
struct s_EquatorialCoords 
{    
    s_EquatorialCoords();

    const s_EclipticalCoords<T> toEcliptical(const T obliquity) const;

    const s_HorizontalCoords<T> toHorizontal(
        const t_julianDay siderealTime /* θ_0 */
    ,   const T observersLatitude      /* Φ   */
    ,   const T observersLongitude     /* L   */) const;

    const glm::vec3 toEuclidean() const;

// Not required for now...
//#pragma NOTE("const s_GalacticCoords toGalactic() const; not yet implemented")

    // Measures the angle (longitudinal) of an object east of the apparent 
    // location of the center of the Sun at the moment of the vernal equinox.
    T right_ascension; // α

    // Measures the angle (latitudinal) of an object above or below the
    // celestial equator. Positive, if north of the celestial equator.
    T declination;     // δ

    // Radial distance of this point to origin.
    T r;
};

typedef s_EquatorialCoords<long double> t_equd;
typedef s_EquatorialCoords<float> t_equf;


// http://en.wikipedia.org/wiki/Ecliptic_coordinate_system

template<typename T>
struct s_EclipticalCoords 
{    
    s_EclipticalCoords();

    const s_EquatorialCoords<T> toEquatorial(const T obliquity) const;


    // Ecliptical longitude. Measured from the vernal equinox along the ecliptic.
    T longitude; // l

    // Ecliptical latitude. Positive if north of the ecliptic.
    T latitude;  // β
};

typedef s_EclipticalCoords<long double> t_ecld;
typedef s_EclipticalCoords<float> t_eclf;



// http://en.wikipedia.org/wiki/Horizontal_coordinate_system

template<typename T>
struct s_HorizontalCoords 
{    
    s_HorizontalCoords();

    const s_EquatorialCoords<T> toEquatorial(
        const t_julianDay siderealTime /* θ_0 */
    ,   const T observersLatitude      /* Φ   */
    ,   const T observersLongitude     /* L   */) const;

    const glm::vec3 toEuclidean() const;

    // Measured westwards from the south. (longitudinal)
    T azimuth;  // h

    // Positive above, negative below horizon. (latitudinal) 
    T altitude; // A
};

typedef s_HorizontalCoords<long double> t_hord;
typedef s_HorizontalCoords<float> t_horf;


// Not required for now...

//// http://en.wikipedia.org/wiki/Galactic_coordinate_system
//
//typedef struct s_GalacticCoords 
//{    
//    s_GalacticCoords();
//
//#pragma NOTE("const s_EquatorialCoords toEquatorial() const; not yet implemented")
//
//    t_longf longitude; // l
//    t_longf latitude;  // b
//
//} t_galCoords;

} // namespace glHimmel
