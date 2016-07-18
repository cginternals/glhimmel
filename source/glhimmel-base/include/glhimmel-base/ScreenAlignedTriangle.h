#pragma once

#include <globjects/base/ref_ptr.h>
#include <glhimmel-base/glhimmel-base_api.h>
#include <globjects/VertexArray.h>
#include <globjects/Buffer.h>

class GLHIMMEL_BASE_API ScreenAlignedTriangle 
{
public:
    ScreenAlignedTriangle();
    ~ScreenAlignedTriangle() = default;

    void draw() const;

protected:
    void initialize();

    globjects::ref_ptr<globjects::VertexArray> m_vao;
    globjects::ref_ptr<globjects::Buffer> m_buffer;
};
