
#include <glhimmel-computed/ComputedHimmel.h>

#include <glhimmel-computed/Earth.h>
#include <glhimmel-base/Debugging.h>
#include <glhimmel-base/Timef.h>
#include <glhimmel-computed/Astronomy.h>

#include <glhimmel-computed/AbstractAstronomy.h>
#include <glhimmel-computed/AtmosphereDrawable.h>
#include <glhimmel-computed/MoonGeode.h>
#include <glhimmel-computed/MoonGlareGeode.h>
#include <glhimmel-computed/StarMapGeode.h>
#include <glhimmel-computed/StarsGeode.h>
#include <glhimmel-computed/HighCloudLayerGeode.h>
#include <glhimmel-computed/DubeCloudLayerGeode.h>

/*#include "moonglaregeode.h"
#include "starsgeode.h"
#include "starmapgeode.h"*/

#include <stdlib.h>
#include <assert.h>


namespace glHimmel
{

ComputedHimmel *ComputedHimmel::createWithoutClouds(const char *brightStarsFilePath)
{
    return new ComputedHimmel(
        new StarMapGeode()
    ,   new MoonGeode()
        ,   new StarsGeode(brightStarsFilePath)
    ,   new AtmosphereDrawable()
    ,   NULL
    ,   NULL
    ,   new Astronomy()
    );
}

ComputedHimmel *ComputedHimmel::createWithClouds(const char *brightStarsFilePath)
{
    GLint frameBuffer;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &frameBuffer);
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    auto himmel = new ComputedHimmel(
        new StarMapGeode()
    ,   new MoonGeode()
        ,   new StarsGeode(brightStarsFilePath)
    ,   new AtmosphereDrawable()
    ,   new HighCloudLayerGeode()
    ,   new DubeCloudLayerGeode()
    ,   new Astronomy()
    );

    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

    return himmel;
}


ComputedHimmel::ComputedHimmel(
    StarMapGeode *milkyWay
,   MoonGeode *moon
,   StarsGeode *stars
,   AtmosphereDrawable *atmosphere
,   HighCloudLayerGeode *highLayer
,   DubeCloudLayerGeode *dubeLayer
,   AbstractAstronomy *astronomy)
:   AbstractHimmel()
,   m_altitude(0.2f)
,   m_sunPosition(glm::vec3(0.0, 0.0, 0.0))
,   m_sunRefractedPosition(glm::vec3(0.0, 0.0, 0.0))
,   m_time(0.f)
,   m_starmap(milkyWay)
,   m_moon(moon)
,   m_moonGlare(moon ? new MoonGlareGeode : nullptr)
,   m_stars(stars)
,   m_astronomy(astronomy)
,   m_atmosphere(atmosphere)
,   m_highLayer(highLayer)
,   m_dubeLayer(dubeLayer)
{
    assert(m_astronomy);
}

ComputedHimmel::~ComputedHimmel()
{
}

void ComputedHimmel::update()
{
    AbstractHimmel::update();

    updateSeed();

    GLint frameBuffer;
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &frameBuffer);
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    if(isDirty())
    {
        const auto atime = astronomicalTime::fromTimeF(*getTime());
        m_astronomy->update(atime);
        m_sunPosition = m_astronomy->getSunPosition(false);
        m_sunRefractedPosition = m_astronomy->getSunPosition(true);
        m_time = static_cast<float>(getTime()->getf());

        if (m_starmap)
                m_starmap->update(*this);

        if (m_moon)
        {
          m_moon->update(*this);
            m_moonGlare->update(*this);
        }

        if (m_stars)
            m_stars->update(*this);

        if (m_atmosphere)
            m_atmosphere->update(*this);

        if (m_highLayer)
            m_highLayer->update(*this);

        if (m_dubeLayer)
            m_dubeLayer->update(*this);

        dirty(false);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
    glViewport(viewport[0], viewport[1], viewport[2], viewport[3]);

    if (m_starmap)
        m_starmap->draw();

    if(m_moon)
    {
        m_moon->draw();
        m_moonGlare->draw();
    }

    if (m_stars)
        m_stars->draw();

    if (m_atmosphere)
        m_atmosphere->draw();

    if (m_highLayer)
        m_highLayer->draw();

    if (m_dubeLayer)
        m_dubeLayer->draw();

    logGLError("ComputedHimmel::update end");
}

void ComputedHimmel::updateSeed()
{
    m_seed = static_cast<float>(rand());
}

AbstractAstronomy * ComputedHimmel::astronomy() const
{
    return m_astronomy.get();
}

AtmosphereDrawable * ComputedHimmel::atmosphere() const
{
    return m_atmosphere.get();
}

MoonGeode *ComputedHimmel::moon()
{
    return m_moon.get();
}

MoonGlareGeode *ComputedHimmel::moonGlare()
{
    return m_moonGlare.get();
}

StarsGeode *ComputedHimmel::stars()
{
    return m_stars.get();
}

StarMapGeode *ComputedHimmel::starMap()
{
    return m_starmap.get();
}

HighCloudLayerGeode * ComputedHimmel::highLayer()
{
    return m_highLayer.get();
}

DubeCloudLayerGeode * ComputedHimmel::dubeLayer()
{
    return m_dubeLayer.get();
}

glm::vec3 ComputedHimmel::getSunPosition() const
{
    return m_sunPosition;
}

glm::vec3 ComputedHimmel::getSunRefractedPosition() const
{
    return m_sunRefractedPosition;
}

glm::vec3 ComputedHimmel::getSunPosition(const astronomicalTime &aTime) const
{
    return m_astronomy->getSunPosition(aTime, m_astronomy->getLatitude(), m_astronomy->getLongitude(), false);
}

float ComputedHimmel::setLatitude(const float latitude)
{
    assert(m_astronomy);

    dirty();
    return m_astronomy->setLatitude(latitude);
}

float ComputedHimmel::getLatitude() const
{
    assert(m_astronomy);
    return m_astronomy->getLatitude();
}

float ComputedHimmel::setLongitude(const float longitude)
{
    assert(m_astronomy);

    dirty();
    return m_astronomy->setLongitude(longitude);
}

float ComputedHimmel::getLongitude() const
{
    assert(m_astronomy);
    return m_astronomy->getLongitude();
}


void ComputedHimmel::setAltitude(const float altitude)
{
    m_altitude = altitude;
}

float ComputedHimmel::getAltitude() const
{
    return m_altitude;
}

float ComputedHimmel::defaultAltitude()
{
    return 0.2f;
}

float ComputedHimmel::getSeed() const
{
    return m_seed;
}

void ComputedHimmel::initialize()
{
}

} // namespace glHimmel
