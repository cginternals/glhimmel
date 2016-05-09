
#include <texturebased/HorizonBand.h>


namespace glHimmel
{

HorizonBand::HorizonBand()
{
    m_scale = defaultScale();
    m_width = defaultWidth();
    m_offset = defaultOffset();

    m_color = defaultColor();
    m_bottomColor = defaultBottomColor();
}

HorizonBand::~HorizonBand()
{
}

void HorizonBand::updateUniforms(globjects::ref_ptr<globjects::Program> program) const
{
    program->setUniform("hbandScale", m_scale);
    program->setUniform("hbandWidth", m_width);
    program->setUniform("hbandOffset", m_offset);
    program->setUniform("hbandColor", m_color);
    program->setUniform("hbandBottomColor", m_bottomColor);
}


void HorizonBand::setScale(const float scale)
{
    m_scale = scale;
}

float HorizonBand::getScale() const
{
    return m_scale;
}

float HorizonBand::defaultScale()
{
    return 0.1f;
}

void HorizonBand::setWidth(const float width)
{
    m_width = width;
}

float HorizonBand::getWidth() const
{
    return m_width;
}

float HorizonBand::defaultWidth()
{
    return 0.0f;
}

void HorizonBand::setOffset(const float offset)
{
    m_offset = offset;
}

float HorizonBand::getOffset() const
{
    return m_offset;
}

float HorizonBand::defaultOffset()
{
    return 0.f;
}

void HorizonBand::setColor(const glm::vec4 &color)
{
    m_color = color;
}

glm::vec4 HorizonBand::getColor() const
{
    return m_color;
}

glm::vec4 HorizonBand::defaultColor()
{
    return glm::vec4(0.25f, 0.25f, 0.25f, 1.f);
}

void HorizonBand::setBottomColor(const glm::vec4 &color)
{
    m_bottomColor = color;
}

glm::vec4 HorizonBand::getBottomColor() const
{
    return m_bottomColor;
}

glm::vec4 HorizonBand::defaultBottomColor()
{
    return glm::vec4(0.08f, 0.08f, 0.08f, 1.f);
}

} // namespace glHimmel
