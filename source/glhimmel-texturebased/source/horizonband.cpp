
#include <glhimmel-texturebased/horizonband.h>


namespace glhimmel_texturebased
{

HorizonBand::HorizonBand()
:   u_params(NULL)
,   u_color(NULL)
,   u_bottomColor(NULL)
{
}


HorizonBand::~HorizonBand()
{
};


void HorizonBand::initialize(osg::StateSet *stateSet)
{
    if(!stateSet)
        return;

    u_params = new osg::Uniform("hbandParams", 
        osg::Vec3f(defaultScale(), defaultWidth(), defaultOffset()));

    u_color       = new osg::Uniform("hbandColor", defaultColor());
    u_bottomColor = new osg::Uniform("hbandBackground", defaultBottomColor());

    stateSet->addUniform(u_params);
    stateSet->addUniform(u_color);
    stateSet->addUniform(u_bottomColor);
};


const osg::Vec3f HorizonBand::getParams() const
{
    osg::Vec3f params;
    u_params->get(params);

    return params;
}


const float HorizonBand::setScale(const float scale)
{
    osg::Vec3f params = getParams();
    params[0] = scale;

    u_params->set(params);
    return getScale();
}

const float HorizonBand::getScale() const
{
    osg::Vec3f params = getParams();
    return params[0];
}
const float HorizonBand::defaultScale()
{
    return 0.1f;
}


const float HorizonBand::setWidth(const float thickness)
{
    osg::Vec3f params = getParams();
    params[1] = thickness;

    u_params->set(params);
    return getWidth();
}

const float HorizonBand::getWidth() const
{
    osg::Vec3f params = getParams();
    return params[1];
}
const float HorizonBand::defaultWidth()
{
    return 0.0f;
}


const float HorizonBand::setOffset(const float offset)
{
    osg::Vec3f params = getParams();
    params[2] = offset;

    u_params->set(params);
    return getOffset();
}

const float HorizonBand::getOffset() const
{
    osg::Vec3f params = getParams();
    return params[2];
}
const float HorizonBand::defaultOffset()
{
    return 0.f;
}


const osg::Vec4f HorizonBand::setColor(const osg::Vec4f &color)
{
    u_color->set(color);
    return getColor();
}

const osg::Vec4f HorizonBand::getColor() const
{
    osg::Vec4f color;
    u_color->get(color);

    return color;
}
const osg::Vec4f HorizonBand::defaultColor()
{
    return osg::Vec4f(0.25f, 0.25f, 0.25f, 1.f);
}


const osg::Vec4f HorizonBand::setBottomColor(const osg::Vec4f &color)
{
    u_bottomColor->set(color);
    return getBottomColor();
}

const osg::Vec4f HorizonBand::getBottomColor() const
{
    osg::Vec4f bottomColor;
    u_bottomColor->get(bottomColor);

    return bottomColor;
}
const osg::Vec4f HorizonBand::defaultBottomColor()
{
    return osg::Vec4f(0.08f, 0.08f, 0.08f, 1.f);
}

} // namespace glhimmel_texturebased