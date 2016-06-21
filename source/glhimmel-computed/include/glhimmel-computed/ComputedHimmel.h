
#pragma once

#include <glhimmel-computed/glhimmel-computed_api.h>
#include <glhimmel-base/AbstractHimmel.h>
#include <glhimmel-base/AstronomicalTime.h>

#include <glhimmel-computed/AbstractAstronomy.h>
#include <glhimmel-computed/AtmosphereDrawable.h>
//#include "moongeode.h"
//#include "moonglaregeode.h"
//#include "starsgeode.h"
//#include "starmapgeode.h"
//#include "highcloudlayergeode.h"
//#include "dubecloudlayergeode.h"


namespace osgHimmel
{
    class AbstractAstronomy;
    class AtmosphereGeode;
    class MoonGeode;
    class MoonGlareGeode;
    class StarsGeode;
    class StarMapGeode;
    class HighCloudLayerGeode;
    class DubeCloudLayerGeode;
} // namespace osgHimmel

namespace glHimmel
{

class GLHIMMEL_BASE_API ComputedHimmel : public AbstractHimmel
{
public:
    static ComputedHimmel *createWithoutClouds();
    //static ComputedHimmel *createWithClouds();

public:

    ComputedHimmel(
        /*StarMapGeode *milkyWay
    ,   MoonGeode *moon
    ,   StarsGeode *stars*/
       AtmosphereDrawable *atmosphere
    /*,   HighCloudLayerGeode *highLayer
    ,   DubeCloudLayerGeode *dubeLayer*/
    ,   AbstractAstronomy *astronomy);

    virtual ~ComputedHimmel();

    void update() override;

    float setLatitude(const float latitude);
    float getLatitude() const;

    float setLongitude(const float longitude);
    float getLongitude() const;

    void setAltitude(const float altitude);
    float getAltitude() const;
    static float defaultAltitude();

    float getSeed() const;

    void initialize() override;

   /* inline MoonGeode *moon() const
    {
        return m_moon;
    }

    inline MoonGlareGeode *moonGlare() const
    {
        return m_moonGlare;
    }

    inline StarsGeode *stars() const
    {
        return m_stars;
    }

    inline StarMapGeode *starmap() const
    {
        return m_starmap;
    }

    inline AbstractAstronomy *astro() const
    {
        return m_astronomy;
    }

    inline HighCloudLayerGeode *highLayer() const
    {
        return m_highLayer;
    }

    inline DubeCloudLayerGeode *dubeLayer() const
    {
        return m_dubeLayer;
    }*/

    AtmosphereDrawable *atmosphere() const;
    AbstractAstronomy* astronomy() const;
    glm::vec3 getSunPosition() const;
    glm::vec3 getSunPosition(const astronomicalTime &aTime) const;

protected:

    void updateSeed();

protected:

    float m_altitude;
    float m_seed = 0.0f;
    glm::vec3 m_sunPosition;
    glm::vec3 m_sunRefractedPosition; // refraction corrected
    float m_time;
    // height above mean sea level in km, mean earth radius in km

    std::unique_ptr<AbstractAstronomy> m_astronomy;

    std::unique_ptr<AtmosphereDrawable> m_atmosphere;
   /* osg::ref_ptr<MoonGeode>           m_moon;
    osg::ref_ptr<MoonGlareGeode>      m_moonGlare;
    osg::ref_ptr<StarsGeode>          m_stars;
    osg::ref_ptr<StarMapGeode>        m_starmap;
    osg::ref_ptr<HighCloudLayerGeode> m_highLayer;
    osg::ref_ptr<DubeCloudLayerGeode> m_dubeLayer;*/
};

} // namespace glHimmel
