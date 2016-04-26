#pragma once

#include <globjects/base/ref_ptr.h>

namespace globjects
{
    class VertexArray;
}


class ScreenAlignedQuad 
{
public:
    ScreenAlignedQuad();
    ~ScreenAlignedQuad();
    
    void draw();

protected:
    void initialize();

protected:
    globjects::ref_ptr<globjects::VertexArray> m_vao;
};
