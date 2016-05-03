
#include <texturebased/AbstractMappedHimmel.h>

#include <texturebased/ScreenAlignedQuad.h>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <texturebased/coordinates.h>

#include <globjects/Shader.h>
#include <globjects/Texture.h>

//#include "shaderfragment/common.h"


//#include <osg/Geode>
//#include <osg/MatrixTransform>

#include <cassert>
#include <limits>
#include <glbinding/gl/types.h>


#pragma NOTE("detach sun stuff to sun composite")

namespace
{
    const gl::GLint BACK_TEXTURE_INDEX(0);
    const gl::GLint SRC_TEXTURE_INDEX(1);
}

namespace glHimmel
{

    AbstractMappedHimmel::AbstractMappedHimmel(
        const bool fakeSun)
        : AbstractHimmel()

        , m_hquad()

        , m_program(nullptr)
        , m_razTimef()
        , m_activeBackUnit(std::numeric_limits<GLint>::max())
        , m_activeSrcUnit(std::numeric_limits<GLint>::max())

        , m_razTransform()
        , m_razDirection(RazDirection::NorthWestSouthEast)
        , m_sunCoordinates(glm::vec3(1.0, 0.0, 1.0))
        , m_sunCoeffs(glm::vec4(0.63, 0.58, 0.49, 1.0))
        , m_sunScale(1.0)
        , m_fakeSun(fakeSun)
    {
        setupProgram();
        setupUniforms();

        m_razTimef.start();

        // Encapsulate hQuad into MatrixTransform.

        /*osg::Geode *geode = new osg::Geode();
        geode->addDrawable(m_hquad);

        m_razTransform->addChild(geode);
        addChild(m_razTransform);*/
    };


    AbstractMappedHimmel::~AbstractMappedHimmel()
    {
    };


    void AbstractMappedHimmel::update()
    {
        AbstractHimmel::update();

        // Update rotation around zenith.

        const float razd(m_razDirection == RazDirection::NorthWestSouthEast ? 1.f : -1.f);
        m_razTransform = glm::rotate<float>(glm::mat4(), razd * m_razTimef.getf(true) * glm::pi<float>() * 2.f, glm::vec3(0.f, 0.f, 1.f));

#pragma NOTE("interface for FakeSun required")

        const float t(timef());

        if (m_fakeSun)
        {
            EquatorialCoords<long double> equ;
            // TODO: fix this
            equ.declination = glm::degrees(t * glm::pi<float>() / 2.f);
            equ.right_ascension = glm::degrees((2.f * t - 1) * glm::pi<float>());

            m_sunCoordinates = equ.toEuclidean();
        }

        // Update two texture status for arbitrary blending (e.g. normal).

        // update texture change
        m_srcAlpha = m_changer.getSrcAlpha(t);

        // Avoid unnecessary unit switches.

        const GLint backUnit(m_changer.getBackUnit(t));
        if (backUnit != m_activeBackUnit)
        {
            assignBackUnit(backUnit);
            assert(backUnit == m_activeBackUnit);
        }

        const GLint srcUnit(m_changer.getSrcUnit(t));
        if (srcUnit != m_activeSrcUnit)
        {
            assignSrcUnit(srcUnit);
            assert(srcUnit == m_activeSrcUnit);
        }
    }

    void AbstractMappedHimmel::setupProgram()
    {
        m_program = new globjects::Program;
        auto vertexShader = globjects::Shader::fromFile(GL_VERTEX_SHADER, "data/shader/abstractMappedHimmel.vert");
        m_program->attach(vertexShader);
        m_program->attach(getFragmentShader());
    }


    void AbstractMappedHimmel::setupUniforms()
    {
        m_program->setUniform<float>("srcAlpha", m_srcAlpha);
        m_program->setUniform<GLint>("back", m_back);
        m_program->setUniform<GLint>("src", m_src);

        if (m_fakeSun)
        {
            m_program->setUniform<glm::vec3>("sunCoords", m_sunCoordinates);
            m_program->setUniform<glm::vec4>("sunCoeffs", m_sunCoeffs);
            m_program->setUniform<float>("sunScale", m_sunScale);

            m_program->setUniform("razInverse", glm::inverse(m_razTransform));
        }
    }

    void AbstractMappedHimmel::setTransitionDuration(const float duration)
    {
        m_changer.setTransitionDuration(duration);
    }

    float AbstractMappedHimmel::getTransitionDuration() const
    {
        return m_changer.getTransitionDuration();
    }
    
    void AbstractMappedHimmel::pushTextureUnit(
        const GLint textureUnit
        , const float time)
    {
        m_changer.pushUnit(textureUnit, time);
    }
    
    void AbstractMappedHimmel::setSecondsPerRAZ(const float secondsPerRAZ)
    {
        m_razTimef.setSecondsPerCycle(abs(secondsPerRAZ));
    }
    
    float AbstractMappedHimmel::getSecondsPerRAZ() const
    {
        return m_razTimef.getSecondsPerCycle();
    }
    
    void AbstractMappedHimmel::setRazDirection(const RazDirection razDirection)
    {
        m_razDirection = razDirection;
    }
    
    AbstractMappedHimmel::RazDirection AbstractMappedHimmel::getRazDirection() const
    {
        return m_razDirection;
    }
    
    void AbstractMappedHimmel::setSunScale(const float scale)
    {
        m_sunScale = scale;
    }

    float AbstractMappedHimmel::getSunScale() const
    {
        return m_sunScale;
    }

    void AbstractMappedHimmel::setSunCoeffs(const glm::vec4 &coeffs)
    {
        m_sunCoeffs = coeffs;
    }

    glm::vec4 AbstractMappedHimmel::getSunCoeffs() const
    {
        return m_sunCoeffs;
    }
    glm::vec4 AbstractMappedHimmel::defaultSunCoeffs()
    {
        return glm::vec4(0.63, 0.58, 0.49, 1.0);
    }
   

} // namespace glHimmel

