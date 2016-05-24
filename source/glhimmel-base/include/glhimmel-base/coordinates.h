
#pragma once

#include "julianday.h"

#include <glm/vec3.hpp>

namespace glHimmel
{

// Explained in "Astronomical Algorithms" and http://en.wikipedia.org/wiki/Celestial_coordinate_system

template<typename T>
struct EquatorialCoords;

template<typename T>
struct EclipticalCoords;

template<typename T>
struct HorizontalCoords;

template<typename T>
struct GalacticCoords;


// http://en.wikipedia.org/wiki/Equatorial_coordinate_system

template<typename T>
struct EquatorialCoords 
{    
    EquatorialCoords();

    const EclipticalCoords<T> toEcliptical(const T obliquity) const;

    const HorizontalCoords<T> toHorizontal(
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


// http://en.wikipedia.org/wiki/Ecliptic_coordinate_system

template<typename T>
struct EclipticalCoords 
{    
    EclipticalCoords();

    const EquatorialCoords<T> toEquatorial(const T obliquity) const;


    // Ecliptical longitude. Measured from the vernal equinox along the ecliptic.
    T longitude; // l

    // Ecliptical latitude. Positive if north of the ecliptic.
    T latitude;  // β
};


// http://en.wikipedia.org/wiki/Horizontal_coordinate_system

template<typename T>
struct HorizontalCoords 
{    
    HorizontalCoords();

    const EquatorialCoords<T> toEquatorial(
        const t_julianDay siderealTime /* θ_0 */
    ,   const T observersLatitude      /* Φ   */
    ,   const T observersLongitude     /* L   */) const;

    const glm::vec3 toEuclidean() const;

    // Measured westwards from the south. (longitudinal)
    T azimuth;  // h

    // Positive above, negative below horizon. (latitudinal) 
    T altitude; // A
};


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

#include <glhimmel-base/coordinates.hpp>