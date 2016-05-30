#include <glhimmel-base/ScreenAlignedTriangle.h>

#include <cassert>
#include <array>

#include <glm/vec2.hpp>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <globjects/base/StaticStringSource.h>

#include <globjects/VertexArray.h>
#include <globjects/VertexAttributeBinding.h>
#include <globjects/Buffer.h>
#include <globjects/base/StringTemplate.h>


using namespace gl;
using namespace globjects;

ScreenAlignedTriangle::ScreenAlignedTriangle()
{
    initialize();
}

void ScreenAlignedTriangle::initialize()
{
    // By default, counterclockwise polygons are taken to be front-facing.
    // http://www.opengl.org/sdk/docs/man/xhtml/glFrontFace.xml

    static const std::array<glm::vec2, 4> raw{ { glm::vec2(+3.f,-1.f), glm::vec2(-1.f,-1.f), glm::vec2(-1.f,+3.f) } };

    m_vao = new VertexArray();

    m_buffer = new Buffer();
    m_buffer->setData(raw, GL_STATIC_DRAW); //needed for some drivers

    auto binding = m_vao->binding(0);
    binding->setAttribute(0);
    binding->setBuffer(m_buffer, 0, sizeof(glm::vec2));
    binding->setFormat(2, GL_FLOAT, GL_FALSE, 0);
    m_vao->enable(0);
}

void ScreenAlignedTriangle::draw() const
{
    m_vao->drawArrays(GL_TRIANGLE_STRIP, 0, 3);
}
