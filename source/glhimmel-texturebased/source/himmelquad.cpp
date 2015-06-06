
#include <glhimmel-texturebased/himmelquad.h>

namespace glhimmel_texturebased
{

HimmelQuad::HimmelQuad()
:   osg::Geometry()
{
    // Create quad that will be used screen aligned (via vertex shader).

    osg::Vec3Array *vertices = new osg::Vec3Array();
    vertices->push_back(osg::Vec3f(-1.f, -1.f, 0.f));
    vertices->push_back(osg::Vec3f(-1.f,  1.f, 0.f));
    vertices->push_back(osg::Vec3f( 1.f,  1.f, 0.f));
    vertices->push_back(osg::Vec3f( 1.f, -1.f, 0.f));

    setVertexArray(vertices);   
    addPrimitiveSet(
        new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));
}

} // namespace glhimmel_texturebased