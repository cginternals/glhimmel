#pragma once

#include <globjects/base/ref_ptr.h>

namespace globjects
{
    class VertexArray;
    class Buffer;
}


class ScreenAlignedQuad
{
public:
    ScreenAlignedQuad();
    ~ScreenAlignedQuad() = default;

    void draw() const;

protected:
    void initialize();

protected:
    globjects::ref_ptr<globjects::VertexArray> m_vao;
    globjects::ref_ptr<globjects::Buffer> m_buffer;
};
