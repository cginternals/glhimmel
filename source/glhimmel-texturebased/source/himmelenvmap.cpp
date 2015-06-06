
#include <glhimmel-texturebased/himmelenvmap.h>

#include <osg/TextureCubeMap>


namespace glhimmel_texturebased
{

class EnvMapCallback : public osg::NodeCallback
{
public:
    EnvMapCallback(HimmelEnvMap::t_cameras &cameras)
    :   m_cameras(cameras)
    {
    }

    virtual void operator()(osg::Node *node, osg::NodeVisitor *nv)
    {
        traverse(node, nv);

        const osg::Vec3f position = node->getBound().center();

        typedef std::pair<osg::Vec3f, osg::Vec3f> t_planeBase;
        const t_planeBase pb[6] =
        {
            t_planeBase(osg::Vec3f( 1, 0, 0), osg::Vec3f( 0,-1, 0))
        ,   t_planeBase(osg::Vec3f(-1, 0, 0), osg::Vec3f( 0,-1, 0))
        ,   t_planeBase(osg::Vec3f( 0, 1, 0), osg::Vec3f( 0, 0, 1))
        ,   t_planeBase(osg::Vec3f( 0,-1, 0), osg::Vec3f( 0, 0,-1))
        ,   t_planeBase(osg::Vec3f( 0, 0, 1), osg::Vec3f( 0,-1, 0))
        ,   t_planeBase(osg::Vec3f( 0, 0,-1), osg::Vec3f( 0,-1, 0)) 
        };

        for(unsigned int i = 0;  i < 6 && i < m_cameras.size(); ++i)
        {
            osg::Matrixf localOffset;
            localOffset.makeLookAt(position, position + pb[i].first, pb[i].second);

            osg::Matrixf viewMatrix = node->getWorldMatrices().front() * localOffset;

            m_cameras[i]->setReferenceFrame(osg::Camera::ABSOLUTE_RF);
            m_cameras[i]->setProjectionMatrixAsFrustum(-1.0, 1.0, -1.0, 1.0, 1.0, node->getBound().radius());
            m_cameras[i]->setViewMatrix(viewMatrix);
        }
    }

protected:
    virtual ~EnvMapCallback() 
    { 
    }

protected:
    HimmelEnvMap::t_cameras m_cameras;
};




HimmelEnvMap::HimmelEnvMap(const unsigned int texSize)
:   osg::Group()
,   m_group(new osg::Group())
{
    setupCubeMap(texSize);
    setupCameras();

    setUpdateCallback(new EnvMapCallback(m_cameras));
}


HimmelEnvMap::~HimmelEnvMap()
{
}


void HimmelEnvMap::setupCubeMap(const unsigned int texSize)
{
    m_cubeMap = new osg::TextureCubeMap;
    m_cubeMap->setTextureSize(texSize, texSize);

    m_cubeMap->setInternalFormat(GL_RGB);

    m_cubeMap->setWrap(osg::Texture::WRAP_S, osg::Texture::CLAMP_TO_EDGE);
    m_cubeMap->setWrap(osg::Texture::WRAP_T, osg::Texture::CLAMP_TO_EDGE);
    m_cubeMap->setWrap(osg::Texture::WRAP_R, osg::Texture::CLAMP_TO_EDGE);

    m_cubeMap->setFilter(osg::TextureCubeMap::MIN_FILTER, osg::TextureCubeMap::LINEAR);
    m_cubeMap->setFilter(osg::TextureCubeMap::MAG_FILTER, osg::TextureCubeMap::LINEAR);
}


void HimmelEnvMap::setupCameras()
{
    for(unsigned int i = 0; i < 6; ++i)
    {
        osg::Camera *camera = new osg::Camera;

        camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera->setViewport(0, 0, m_cubeMap->getTextureWidth(), m_cubeMap->getTextureHeight());
        camera->setRenderOrder(osg::Camera::PRE_RENDER);

        // Tell the camera to use OpenGL frame buffer object where supported.
        camera->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER_OBJECT);

        // Attach the texture and use it as the color buffer.
        camera->attach(osg::Camera::COLOR_BUFFER, m_cubeMap, 0, i);

        camera->addChild(m_group.get());
        osg::Group::addChild(camera);

        m_cameras.push_back(camera);
    }
}


// osg::Group child manipulation wrapper:

bool HimmelEnvMap::addChild(osg::Node *child)
{
    osg::Group::addChild(child);
    return m_group->addChild(child);
}


bool HimmelEnvMap::insertChild(unsigned int index, Node *child)
{
    osg::Group::insertChild(index + 6, child);
    return m_group->insertChild(index, child);
}


bool HimmelEnvMap::removeChild(Node *child)
{
    osg::Group::removeChild(child);
    return m_group->removeChild(child);
}


bool HimmelEnvMap::removeChild(unsigned int pos, unsigned int numChildrenToRemove)
{
    osg::Group::removeChild(pos + 6, numChildrenToRemove);
    return m_group->removeChild(pos, numChildrenToRemove);
}


bool HimmelEnvMap::removeChildren(unsigned int pos, unsigned int numChildrenToRemove)
{
    osg::Group::removeChildren(pos + 6, numChildrenToRemove);
    return m_group->removeChildren(pos, numChildrenToRemove);
}


bool HimmelEnvMap::replaceChild(Node *origChild, Node *newChild)
{
    osg::Group::replaceChild(origChild, newChild);
    return m_group->replaceChild(origChild, newChild);
}


unsigned int HimmelEnvMap::getNumChildren() const
{
    return m_group->getNumChildren();
}


bool HimmelEnvMap::setChild(unsigned int i, Node *node)
{
    osg::Group::setChild(i + 6, node);
    return m_group->setChild(i, node);
}


osg::Node *HimmelEnvMap::getChild(unsigned int i)
{
    return m_group->getChild(i);
}


const osg::Node *HimmelEnvMap::getChild(unsigned int i) const
{
    return m_group->getChild(i);
}


bool HimmelEnvMap::containsNode(const Node *node) const
{
    return m_group->containsNode(node);
}


unsigned int HimmelEnvMap::getChildIndex(const Node *node) const
{
    return m_group->getChildIndex(node);
}

} // namespace glhimmel_texturebased