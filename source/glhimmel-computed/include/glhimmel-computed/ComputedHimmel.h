
#pragma once

#include <glhimmel-computed/glhimmel-computed_api.h>
#include <glhimmel-base/AbstractHimmel.h>
#include <glhimmel-base/AstronomicalTime.h>
#include <glm/vec3.hpp>


namespace glHimmel
{
class AtmosphereDrawable;
class AbstractAstronomy;
class MoonGeode;
class MoonGlareGeode;
class StarMapGeode;
class StarsGeode;
class HighCloudLayerGeode;
class DubeCloudLayerGeode;

class GLHIMMEL_COMPUTED_API ComputedHimmel : public AbstractHimmel
{
public:
    static ComputedHimmel *createWithoutClouds(const char *brightStarsFilePath);
    static ComputedHimmel *createWithClouds(const char *brightStarsFilePath);

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

    AtmosphereDrawable *atmosphere() const;
    AbstractAstronomy *astronomy() const;
    MoonGeode *moon();
    MoonGlareGeode *moonGlare();
    StarsGeode *stars();
    StarMapGeode *starMap();
    HighCloudLayerGeode *highLayer();
    DubeCloudLayerGeode *dubeLayer();
    glm::vec3 getSunPosition() const;
    glm::vec3 getSunRefractedPosition() const;
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

    std::unique_ptr<MoonGeode> m_moon;
    std::unique_ptr<MoonGlareGeode> m_moonGlare;

    std::unique_ptr<StarsGeode> m_stars;
    std::unique_ptr<StarMapGeode> m_starmap;

    std::unique_ptr<HighCloudLayerGeode> m_highLayer;
    std::unique_ptr<DubeCloudLayerGeode> m_dubeLayer;
};

} // namespace glHimmel
