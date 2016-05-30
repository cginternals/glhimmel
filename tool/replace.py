import sys
enc='iso-8859-15'
with open(sys.argv[1], 'r', encoding=enc) as f:
    content = f.read()
    replacements = [
        ('osg::Vec3f', 'glm::vec3'),
        ('osg::Vec4f', 'glm::vec4'),
        ('osgHimmel', 'glHimmel'),
        ('"declspec.h"', '<glhimmel-computed/glhimmel-computed_api.h>'),
        ('t_julianDay', 'JulianDay'),
        ('t_aTime', 'AstronomicalTime'),
        ('OSGH_API', 'GLHIMMEL_TEXTUREBASED_API'),
        ('t_longf', 'long double'),
        ('osg::Matrixf::rotate', 'glm::rotate'),
        ('osg::Matrixf', 'glm::mat4'),
        ('sin(', 'std::sin('),
        ('cos(', 'std::cos('),
        ('tan(', 'std::tan('),
        ('asin(', 'std::asin('),
        ('atan2(', 'std::atan2('),
        ('acos(', 'std::acos('),
        ('_rad(', 'glm::radians('),
        ('_deg(', 'glm::degrees('),
        ('osg::Program *', 'globjects::ref_ptr<globjects::Program> '),
    ]
    for k, v in replacements:
        content = content.replace(k, v)

    new_content = ""
    include = False
    if sys.argv[1].endswith('.cpp'):
        for n in content.splitlines():
            if n.startswith('#include'):
                include = True
            if include:
                new_content += n + '\n'
    elif sys.argv[1].endswith('.h'):
        for n in content.splitlines():
            if n.startswith('#pragma'):
                pragma = True
            if (n.startswith('#ifndef __') or
                    n.startswith('#define __') or
                    n.startswith('#endif // __')):
                continue
            if pragma:
                new_content += n + '\n'
    content = new_content


with open(sys.argv[1], 'w') as f:
    f.write(content)
