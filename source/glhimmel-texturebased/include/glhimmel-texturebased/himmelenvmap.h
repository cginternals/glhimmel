
#pragma once
#ifndef __HIMMELENVMAP_H__
#define __HIMMELENVMAP_H__

#include <glhimmel-texturebased/glhimmel-texturebased_api.h>

#include <osg/Group>
#include <osg/Camera>

namespace osg
{
    class TextureCubeMap;
}


namespace glhimmel_texturebased
{

class GLHIMMEL_TEXTUREBASED_API HimmelEnvMap : public osg::Group
{
public:

    HimmelEnvMap(const unsigned int texSize);
    virtual ~HimmelEnvMap();

    inline osg::TextureCubeMap *cubeMap()
    {
        return m_cubeMap;
    }

    // Redirect all Child Manipulators to this and m_group;

    virtual bool addChild(osg::Node *child);

    // untested...

    virtual bool insertChild(unsigned int index, Node *child);
    virtual bool removeChild(Node *child);
    virtual bool removeChild(unsigned int pos, unsigned int numChildrenToRemove = 1);
    virtual bool removeChildren(unsigned int pos, unsigned int numChildrenToRemove);
    virtual bool replaceChild(Node *origChild, Node *newChild);
    virtual unsigned int getNumChildren() const;
    virtual bool setChild(unsigned int i, Node *node);
    virtual Node *getChild(unsigned int i);
    virtual const Node *getChild(unsigned int i) const;
    virtual bool containsNode(const Node *node) const;
    virtual unsigned int getChildIndex(const Node *node) const;


protected:

    void setupCubeMap(const unsigned int texSize);
    void setupCameras();

public:
    typedef std::vector<osg::ref_ptr<osg::Camera> > t_cameras;

protected:

    osg::ref_ptr<osg::Group> m_group;
    osg::ref_ptr<osg::TextureCubeMap> m_cubeMap;

    t_cameras m_cameras;
};

} // namespace glhimmel_texturebased

#endif // __HIMMELENVMAP_H__