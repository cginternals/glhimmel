#pragma once

#include <texturebased/texturebased_api.h>
#include <texturebased/AbstractHimmel.h>
#include <texturebased/ScreenAlignedQuad.h>
#include <texturebased/TwoUnitsChanger.h>
#include <texturebased/Timef.h>

#include <glbinding/gl/gl.h>

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

#include <globjects/Program.h>

using namespace gl;

namespace glHimmel
{
    class HorizonBand;

    class TEXTUREBASED_API AbstractMappedHimmel : public AbstractHimmel
    {
    public:

        enum class RazDirection
        {
            // RD_NorthEastSouthWest and RD_NorthWestSouthEast were used to avoid misinterpretations of CW and CCW.

            NorthWestSouthEast
            , NorthEastSouthWest
        };

    public:

        AbstractMappedHimmel(const bool hBand = false, const bool fakeSun = false);
        virtual ~AbstractMappedHimmel();

        void initialize() override;

        // When assigning textures to an instance of this class, a TimeF has to be
        // specified, that is used for internal keypoints. Keypoints define point
        // in times where one texture gets replaced (abruptly or by smooth fading)
        // by the following one. The transition duration can be specified in
        // floating time and should be smaller than the minimum time distance between
        // two adjacent keypoints.

        // These calls get redirected to a TwoUnitsChanger instance (see comment there).

        void setTransitionDuration(const float duration);
        float getTransitionDuration() const;

        // This call gets redirected to a TwoUnitsChanger instance  (see comment there).
        void pushTextureUnit(
            const globjects::ref_ptr<globjects::Texture> textureUnit
            , const float time = 1.f);


        // Rotation around Zenith (RaZ), if used discreetly, can greatly influence the
        // dynamic of a scene, by slightly rotating the given texture mapped himmel
        // around the up vector. It accesses its own TimeF object and thereby remains
        // independent from the himmels timef.

        void setSecondsPerRAZ(const float secondsPerRAZ); // reasonable values should be around 2000+
        float getSecondsPerRAZ() const;

        void setRazDirection(const RazDirection razDirection);
        RazDirection getRazDirection() const;


        void setSunScale(const float scale);
        float getSunScale() const;

        void setSunCoeffs(const glm::vec4 &coeffs);
        glm::vec4 getSunCoeffs() const;
        static glm::vec4 defaultSunCoeffs();

        // Use HorizonBand for horizon blending.
        HorizonBand* hBand() const;

        // Use this helper to work with pre-configured textures.
        globjects::ref_ptr<globjects::Texture> getOrCreateTexture(const unsigned int textureUnit);

        void draw() override;

    protected:

        // Inherited by AbstractHimmel. Call this first when inherited!
        void update() override;

        void setupProgram();
        void updateUniforms() const;

        virtual std::string fragmentShaderPath() const = 0;
        virtual GLenum textureTarget() const;

    protected:

        ScreenAlignedQuad m_hquad;
        globjects::ref_ptr<globjects::Program> m_program;

        TwoUnitsChanger m_changer;
        globjects::ref_ptr<globjects::Texture> m_activeBackUnit;
        globjects::ref_ptr<globjects::Texture> m_activeSrcUnit;
        float m_srcAlpha;


        TimeF m_razTimef;
        glm::mat4 m_razTransform;
        RazDirection m_razDirection;

        glm::vec3 m_sunCoordinates;
        glm::vec4 m_sunCoeffs;
        float m_sunScale;
        const bool m_fakeSun;

        std::unique_ptr<HorizonBand> m_hBand;

        std::map<GLint, globjects::ref_ptr<globjects::Texture>> m_texturesById;
    };

} // namespace glHimmel
