
#include <texturebased/SphereMappedHimmel.h>

#include "shaderfragment/common.h"
#include "shaderfragment/blend_normal.h"
#include "shaderfragment/fakesun.h"

#include <globjects/Shader.h>
#include <globjects/Texture.h>


namespace glHimmel
{

SphereMappedHimmel::SphereMappedHimmel(const bool fakeSun)
:   AbstractMappedHimmel(fakeSun)
{
};


SphereMappedHimmel::~SphereMappedHimmel()
{
};



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

} // namespace osgHimmel