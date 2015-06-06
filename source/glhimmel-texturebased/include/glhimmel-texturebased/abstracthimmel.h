
#pragma once
#ifndef __ABSTRACTHIMMEL_H__
#define __ABSTRACTHIMMEL_H__

#include <glhimmel-texturebased/glhimmel-texturebased_api.h>
#include <glhimmel-texturebased/typedefs.h>

#include <osg/MatrixTransform>
#include <osg/NodeCallback>


namespace glhimmel_texturebased
{

class TimeF;

class GLHIMMEL_TEXTUREBASED_API AbstractHimmel : public osg::MatrixTransform
{
private:

    // This callback calls update.
    class HimmelNodeCallback : public osg::NodeCallback 
    {
    public:
        virtual void operator()(
            osg::Node *node
        ,   osg::NodeVisitor *nv);
    };

public:

    AbstractHimmel();
    virtual ~AbstractHimmel();

    // Called by the HimmelUpdateCallback. Call this first when inherited!
    virtual void update();


    // (Initialize is automatically called at least on first update)
    void initialize();

    // The timef gets updated but takes no ownership.
    void assignTime(
        TimeF *timef
    ,   const bool autoUpdate = false);

    inline TimeF *getTime() const
    {
        return m_timef;
    }

    // Update timef on update.
    void setAutoUpdateTime(const bool autoUpdate);
    inline const bool getAutoUpdateTime() const
    {
        return m_autoUpdateTime;
    }

    void setReferenceBoundingRadius(const float radius);


    // From osg::Group:

    virtual void traverse(osg::NodeVisitor &nv);

    // From osg::Transform:

    // Get the transformation matrix which moves from local coords to world coords.
    virtual bool computeLocalToWorldMatrix(osg::Matrix& matrix, osg::NodeVisitor* nv) const;

    // Get the transformation matrix which moves from world coords to local coords.
    virtual bool computeWorldToLocalMatrix(osg::Matrix& matrix, osg::NodeVisitor* nv) const;


    void dirty(const bool enable = true);
    virtual const bool isDirty() const;

    // TODO: seems ugly - optimize this!

    void setCameraHint(osg::Camera *camera);
    const float getCameraFovHint() const;

    void setViewSizeHint(unsigned int width, unsigned int height);
    const unsigned int getViewSizeWidthHint() const;
    const unsigned int getViewSizeHeightHint() const;

protected:

    void setupNode(osg::StateSet* stateSet);

    virtual void postInitialize() { };


    const float timef() const;

protected:

    bool m_initialized;
    bool m_dirty;

    //

    float m_referenceBoundingRadius;

    TimeF *m_timef;
    bool m_autoUpdateTime;

    // This is used for "is dirty decisions" in update.
    // m_timef can be updated from outsite, so we need check for changes manually.
    t_longf m_lastElapsed;

    // TODO: maybe there is a better, cleaner solution to get this information from osg...
    osg::Camera *m_cameraHint;
    unsigned int m_widthHint;
    unsigned int m_heightHint;
};

} // namespace glhimmel_texturebased

#endif // __ABSTRACTHIMMEL_H__