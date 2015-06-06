
#include <glhimmel-texturebased/spheremappedhimmel.h>

#include "shaderfragment/common.h"
#include "shaderfragment/blend_normal.h"
#include "shaderfragment/fakesun.h"

#include <osg/Texture2D>


namespace glhimmel_texturebased
{

SphereMappedHimmel::SphereMappedHimmel(
    const osg::Vec3f &center
,   const bool fakeSun)
:   AbstractMappedHimmel(fakeSun)
,   u_transform(NULL)
{
    setName("SphereMappedHimmel");

    osg::Vec3f c(center);
    c.normalize();

    u_transform = new osg::Uniform("transform", osg::Matrixf::rotate(osg::Vec3f(0.0, 0.0, 1.0),  c));
    getOrCreateStateSet()->addUniform(u_transform);
};


SphereMappedHimmel::~SphereMappedHimmel()
{
};


osg::Texture2D* SphereMappedHimmel::getOrCreateTexture2D(const GLint textureUnit)
{
    // Retrieve an existing texture.

    const t_tex2DById::iterator existingTex2D(m_tex2DsById.find(textureUnit));
    if(existingTex2D != m_tex2DsById.end())
        return existingTex2D->second;


    // Create and configure new texture object.

    osg::ref_ptr<osg::Texture2D> newTex2D(new osg::Texture2D);

    newTex2D->setUnRefImageDataAfterApply(true);

    newTex2D->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
    newTex2D->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);

    newTex2D->setFilter(osg::Texture::MIN_FILTER, osg::Texture::LINEAR);
    newTex2D->setFilter(osg::Texture::MAG_FILTER, osg::Texture::LINEAR);

    m_tex2DsById[textureUnit] = newTex2D;

    // Assign some textures if there are none.

    if(m_tex2DsById.size() == 1)
        assignBackUnit(textureUnit);
    if(m_tex2DsById.size() == 2)
        assignSrcUnit(textureUnit);

    return newTex2D;
}


osg::StateAttribute *SphereMappedHimmel::getTextureAttribute(const GLint textureUnit) const
{
    const t_tex2DById::const_iterator tex2Di(m_tex2DsById.find(textureUnit));
    return tex2Di == m_tex2DsById.end() ? NULL : tex2Di->second;
}



const std::string SphereMappedHimmel::getVertexShaderSource()
{
    return glsl_version_150()
    
    +   glsl_quadRetrieveRay()
    +   glsl_quadTransform()

    +   PRAGMA_ONCE(main,

        "uniform mat4 razInverse;\n"
        "uniform mat4 transform;\n"
        "\n"
        "out vec4 m_ray;\n"
        "out vec4 m_razInvariant;\n"
        "\n"
        "void main(void)\n"
        "{\n"
        "    m_ray = transform * quadRetrieveRay();\n"
        "    m_razInvariant = m_ray * razInverse;\n"
        "\n"
        "    quadTransform();\n"
        "}");
}



const std::string SphereMappedHimmel::getFragmentShaderSource()
{
    return glsl_version_150()

//  +   glsl_f_blendNormalExt // using mix

    +   (m_fakeSun ? glsl_fakesun() : "")
    
    +   PRAGMA_ONCE(main,

        "in vec4 m_ray;\n"
        "\n"
        // From AbstractMappedHimmel

        "uniform float srcAlpha;\n"
        "\n"
        "uniform sampler2D back;\n" 
        "uniform sampler2D src;\n"
        "\n"
        // Color Retrieval

        "void main(void)\n"
        "{\n"
        "    vec3 stu = normalize(m_ray.xyz);\n"
        "\n"
        "    float yy = (1.0 - stu.y);\n"
        "    yy *= yy;\n"
        "    float m = 0.5 / sqrt(stu.x * stu.x + stu.z * stu.z + yy);\n"
        "    vec2 uv = vec2(-stu.x, stu.z) * m + 0.5;\n"
    +   "\n"
        "    vec4 fc = mix(\n"
        "        texture2D(back, uv), texture2D(src, uv), srcAlpha);\n"
        "\n"
        "    gl_FragColor = " + (m_fakeSun ? "fc + fakeSun(fc.a)" : "fc") + ";\n"
        "}");
}

} // namespace glhimmel_texturebased