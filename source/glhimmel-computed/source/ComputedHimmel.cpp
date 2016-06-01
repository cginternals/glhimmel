
#include <glhimmel-computed/ComputedHimmel.h>

#include <glhimmel-computed/Earth.h>
#include <glhimmel-base/Timef.h>
#include <glhimmel-computed/Astronomy.h>
#include <glhimmel-base/ScreenAlignedTriangle.h>
#include <glhimmel-computed/AtmosphereDrawable.h>
#include "moongeode.h"
#include "moonglaregeode.h"
#include "starsgeode.h"
#include "starmapgeode.h"
#include "highcloudlayergeode.h"
#include "dubecloudlayergeode.h"

#include <osg/ShapeDrawable>

#include <stdlib.h>
#include <assert.h>


namespace glHimmel
{

Himmel *Himmel::createWithoutClouds()
{
    // cubeMapFilePaths should contain a questionmark '?' that is replaced
    // by cubemap extensions '_px', '_nx', '_py', etc. 
    // e.g. "resources/starmap?.png" points to "resources/milkyway_px.png" etc.

    return new Himmel(
        new StarMapGeode("resources/skymap?.png")
    ,   new MoonGeode("resources/moon?.png")
    ,   new StarsGeode("resources/brightstars")
    ,   new AtmosphereDrawable()
    ,   NULL//new HighCloudLayerGeode()
    ,   NULL//new DubeCloudLayerGeode()
    ,   new Astronomy()
    );
}


Himmel *Himmel::createWithClouds()
{
    // cubeMapFilePaths should contain a questionmark '?' that is replaced
    // by cubemap extensions '_px', '_nx', '_py', etc. 
    // e.g. "resources/starmap?.png" points to "resources/milkyway_px.png" etc.

    return new Himmel(
        new StarMapGeode("resources/skymap?.png")
    ,   new MoonGeode("resources/moon?.png")
    ,   new StarsGeode("resources/brightstars")
    ,   new AtmosphereDrawable()
    ,   new HighCloudLayerGeode()
    ,   new DubeCloudLayerGeode()
    ,   new Astronomy()
    );
}


Himmel::Himmel(
    StarMapGeode *milkyWay
,   MoonGeode *moon
,   StarsGeode *stars
,   AtmosphereDrawable *atmosphere
,   HighCloudLayerGeode *highLayer
,   DubeCloudLayerGeode *dubeLayer
,   AbstractAstronomy *astronomy)
:   AbstractHimmel()
,   m_starmap(milkyWay)
,   m_moon(moon)
,   m_moonGlare(NULL)
,   m_stars(stars)
,   m_atmosphere(atmosphere)
,   m_highLayer(highLayer)
,   m_dubeLayer(dubeLayer)
,   m_astronomy(astronomy)

,   u_sun(NULL)
,   u_sunr(NULL)
,   u_time(NULL)
,   u_common(NULL)
{
    assert(m_astronomy);

    u_sun = new osg::Uniform("sun", glm::vec3(0.0, 0.0, 0.0));
    getOrCreateStateSet()->addUniform(u_sun);
    u_sunr = new osg::Uniform("sunr", glm::vec3(0.0, 0.0, 0.0));
    getOrCreateStateSet()->addUniform(u_sunr);


    u_time = new osg::Uniform("time", 0.f);
    getOrCreateStateSet()->addUniform(u_time);

    // 0: altitude in km
    // 1: apparent angular radius (not diameter!)
    // 2: radius up to "end of atm"
    // 3: seed (for randomness of stuff)
    u_common = cmnUniform();
    getOrCreateStateSet()->addUniform(u_common);


    int bin = 0;
    static const std::string binName("RenderBin");

    addAntiCull()->getOrCreateStateSet()->setRenderBinDetails(bin++, binName);

    if(m_starmap)
    {
        addChild(m_starmap);
        m_starmap->getOrCreateStateSet()->setRenderBinDetails(bin++, binName);
    }
    if(m_stars)
    {
        addChild(m_stars);
        m_stars->getOrCreateStateSet()->setRenderBinDetails(bin++, binName);
    }
    if(m_moon)
    {
        addChild(m_moon);
        m_moon->getOrCreateStateSet()->setRenderBinDetails(bin++, binName);

        //m_moonGlare = new MoonGlareGeode;
        //addChild(m_moonGlare);
        //m_moonGlare->getOrCreateStateSet()->setRenderBinDetails(bin++, binName);

        //m_moon->addUniformsToVariousStateSate(m_moonGlare->getOrCreateStateSet());
    }
    if(m_atmosphere)
    {
        addChild(m_atmosphere);
        m_atmosphere->getOrCreateStateSet()->setRenderBinDetails(bin++, binName);
    }



    if(m_highLayer)
    {
        addChild(m_highLayer);
        m_highLayer->getOrCreateStateSet()->setRenderBinDetails(bin++, binName);
    }

    if(m_dubeLayer)
    {
        addChild(m_dubeLayer);
        m_dubeLayer->getOrCreateStateSet()->setRenderBinDetails(bin++, binName);
    }
};


osg::Geode *Himmel::addAntiCull()
{
    // Add a unit cube to this geode, to avoid culling of hquads, stars, 
    // moon, etc. caused by automatic near far retrieval of osg. This geode 
    // should be added first, since all other nodes are drawn with blending.

    osg::ref_ptr<osg::Geode> antiCull = new osg::Geode();
    addChild(antiCull);

    // 2 * 2 ^ 0.5 -> should fit an rotating cube with radius 1
    osg::Box *cube = new osg::Box(glm::vec3(), 2.8284f); 

    osg::ShapeDrawable *cubeDrawable = new osg::ShapeDrawable(cube);
    cubeDrawable->setColor(glm::vec4(0.f, 0.f, 0.f, 1.f));

    antiCull->addDrawable(cubeDrawable);

    return antiCull;
}


osg::Uniform *Himmel::cmnUniform()
{
    return new osg::Uniform("cmn", glm::vec4(defaultAltitude()
        , Earth::meanRadius(), Earth::meanRadius() + Earth::atmosphereThicknessNonUniform(), 0));
}


Himmel::~Himmel()
{
    delete m_astronomy;
};


void Himmel::update()
{
    AbstractHimmel::update();

    updateSeed();

    if(isDirty())
    {
        const astronomicalTime atime = astronomicalTime::fromTimeF(*getTime());
        astro()->update(atime);

        glm::vec3 sunv = astro()->getSunPosition(false);
        u_sun->set(sunv);

        glm::vec3 sunrv = astro()->getSunPosition(true);
        u_sunr->set(sunrv);

        u_time->set(static_cast<float>(getTime()->getf()));

        if(m_starmap)
            m_starmap->update(*this);
        if(m_moon)
        {
            m_moon->update(*this);
            //m_moonGlare->update(*this);
        }
        if(m_stars)
            m_stars->update(*this);
        if(m_atmosphere)
            m_atmosphere->update(*this);
        if(m_highLayer)
            m_highLayer->update(*this);

        if(m_dubeLayer)
            m_dubeLayer->update(*this);

        dirty(false);
    }
}


void Himmel::updateSeed()
{
    glm::vec4 temp; 
    u_common->get(temp);

    temp[3] = rand();
    u_common->set(temp);
}


const glm::vec3 Himmel::getSunPosition() const
{
    glm::vec3 sunv;
    u_sun->get(sunv);

    return sunv;
}


const glm::vec3 Himmel::getSunPosition(const astronomicalTime &aTime) const
{
    return astro()->getSunPosition(aTime, m_astronomy->getLatitude(), m_astronomy->getLongitude(), false);
}


const float Himmel::setLatitude(const float latitude)
{
    assert(m_astronomy);
    
    dirty();
    return m_astronomy->setLatitude(latitude);
}

const float Himmel::getLatitude() const
{
    assert(m_astronomy);
    return m_astronomy->getLatitude();
}


const float Himmel::setLongitude(const float longitude)
{
    assert(m_astronomy);

    dirty();
    return m_astronomy->setLongitude(longitude);
}

const float Himmel::getLongitude() const
{
    assert(m_astronomy);
    return m_astronomy->getLongitude();
}


const float Himmel::setAltitude(const float altitude)
{
    glm::vec4 temp; 
    u_common->get(temp);

    // Clamp altitude into non uniform atmosphere. (min alt is 1m)
    temp[0] = _clamp(0.001f, Earth::atmosphereThicknessNonUniform(), altitude);
    u_common->set(temp);

    return getAltitude();
}

const float Himmel::getAltitude() const
{
    glm::vec4 temp; 
    u_common->get(temp);

    return temp[0];
}

const float Himmel::defaultAltitude()
{
    return 0.2f;
}

} // namespace glHimmel