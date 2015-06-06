
#pragma once
#ifndef __HORIZONBAND_H__
#define __HORIZONBAND_H__

#include <glhimmel-texturebased/glhimmel-texturebased_api.h>

#include <osg/StateSet>


namespace glhimmel_texturebased
{

// Some texture projections (polar mapping with MM_Half and paraboloid mapping)
// discard the lower hemisphere, resulting in a hard edge to gl clear color. 
// To avoid this, these himmels offer a gradient called HorizonBand. This is 
// given by scale, width, and offset as well as a color and a bottom color. 
// The scale is in `[`0;1`[`, with 1.0 overlapping the skies vertical 180 degrees. 
// Width specifies the vertical region uniformly filled with color. Currently 
// smoothstep blending is used for the gradient, but inv_squared interpolation 
// yields good results too.

class GLHIMMEL_TEXTUREBASED_API HorizonBand
{
public:

    HorizonBand();
    virtual ~HorizonBand();

    void initialize(osg::StateSet *stateSet);

    const float setScale(const float scale);
    const float getScale() const;
    static const float defaultScale();

    const float setWidth(const float thickness);
    const float getWidth() const;
    static const float defaultWidth();

    const float setOffset(const float offset);
    const float getOffset() const;
    static const float defaultOffset();

    const osg::Vec4f setColor(const osg::Vec4f &color);
    const osg::Vec4f getColor() const;
    static const osg::Vec4f defaultColor();

    const osg::Vec4f setBottomColor(const osg::Vec4f &color);
    const osg::Vec4f getBottomColor() const;
    static const osg::Vec4f defaultBottomColor();

protected:
    const osg::Vec3f getParams() const;

protected:

    osg::ref_ptr<osg::Uniform> u_params; // Vec3 { 0: Scale, 1: Width, 2: Offset }

    osg::ref_ptr<osg::Uniform> u_color; // Vec4
    osg::ref_ptr<osg::Uniform> u_bottomColor; // Vec4
};

} // namespace glhimmel_texturebased

#endif // __HORIZONBAND_H__