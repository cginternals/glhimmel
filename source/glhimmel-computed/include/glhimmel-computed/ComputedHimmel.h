
#pragma once

#include <glhimmel-computed/glhimmel-computed_api.h>
#include <glhimmel-base/AbstractHimmel.h>
#include <glhimmel-base/AstronomicalTime.h>

#include <glhimmel-computed/AbstractAstronomy.h>
#include <glhimmel-computed/AtmosphereDrawable.h>
#include "moongeode.h"
#include "moonglaregeode.h"
#include "starsgeode.h"
#include "starmapgeode.h"
#include "highcloudlayergeode.h"
#include "dubecloudlayergeode.h"


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
    static ComputedHimmel *createWithClouds();

public:

    ComputedHimmel(
        StarMapGeode *milkyWay
    ,   MoonGeode *moon
    ,   StarsGeode *stars
    ,   AtmosphereDrawable *atmosphere
    ,   HighCloudLayerGeode *highLayer
    ,   DubeCloudLayerGeode *dubeLayer
    ,   AbstractAstronomy *astronomy);

    virtual ~ComputedHimmel();

    virtual void update();

    const float setLatitude(const float latitude);
    const float getLatitude() const;

    const float setLongitude(const float longitude);
    const float getLongitude() const;

    const float setAltitude(const float altitude);
    const float getAltitude() const;
    static const float defaultAltitude();
    
    inline AtmosphereDrawable *atmosphere() const 
    {
        return m_atmosphere;
    }

    inline MoonGeode *moon() const
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
    }


    const glm::vec3 getSunPosition() const;
    const glm::vec3 getSunPosition(const astronomicalTime &aTime) const;

protected:

    void updateSeed();

    // Workaround avoiding culling by osgs' automated near far 
    // retrieval. This fixes culling of stars and moon when no 
    // further scene geometry is available.
    // Six black quads will be drawn. This adds a black 
    // background that is used if no starmap is given. This is
    // required since other geodes are rendered with blending 
    // enabled.
    osg::Geode *addAntiCull();

protected:

    osg::ref_ptr<osg::Uniform> u_sun;
    osg::ref_ptr<osg::Uniform> u_sunr; // refraction corrected
    osg::ref_ptr<osg::Uniform> u_time;
    // height above mean sea level in km, mean earth radius in km
    osg::ref_ptr<osg::Uniform> u_common; 

    AbstractAstronomy *m_astronomy;

    osg::ref_ptr<AtmosphereDrawable>     m_atmosphere;
    osg::ref_ptr<MoonGeode>           m_moon;
    osg::ref_ptr<MoonGlareGeode>      m_moonGlare;
    osg::ref_ptr<StarsGeode>          m_stars;
    osg::ref_ptr<StarMapGeode>        m_starmap;
    osg::ref_ptr<HighCloudLayerGeode> m_highLayer;
    osg::ref_ptr<DubeCloudLayerGeode> m_dubeLayer;
};

} // namespace glHimmel
