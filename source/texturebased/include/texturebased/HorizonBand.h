
#pragma once

#include <texturebased/texturebased_api.h>

#include <glm/vec4.hpp>
#include <glm/vec3.hpp>

#include <globjects/Program.h>

namespace glHimmel
{

// Some texture projections (polar mapping with MappingMode::Half and paraboloid mapping)
// discard the lower hemisphere, resulting in a hard edge to gl clear color. 
// To avoid this, these himmels offer a gradient called HorizonBand. This is 
// given by scale, width, and offset as well as a color and a bottom color. 
// The scale is in `[`0;1`[`, with 1.0 overlapping the skies vertical 180 degrees. 
// Width specifies the vertical region uniformly filled with color. Currently 
// smoothstep blending is used for the gradient, but inv_squared interpolation 
// yields good results too.

class TEXTUREBASED_API HorizonBand
{
public:

    HorizonBand();
    virtual ~HorizonBand();

    void updateUniforms(globjects::ref_ptr<globjects::Program>) const;

    void setScale(const float scale);
    float getScale() const;
    static float defaultScale();

    void setWidth(const float width);
    float getWidth() const;
    static float defaultWidth();

    void setOffset(const float offset);
    float getOffset() const;
    static float defaultOffset();

    void setColor(const glm::vec4 &color);
    glm::vec4 getColor() const;
    static glm::vec4 defaultColor();

    void setBottomColor(const glm::vec4 &color);
    glm::vec4 getBottomColor() const;
    static glm::vec4 defaultBottomColor();

protected:

    float m_scale;
    float m_width;
    float m_offset;

    glm::vec4 m_color; 
    glm::vec4 m_bottomColor; 
};

} // namespace glHimmel
