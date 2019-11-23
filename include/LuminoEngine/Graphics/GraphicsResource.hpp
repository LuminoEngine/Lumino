// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once
#include "Common.hpp"

namespace ln {
namespace detail {
class GraphicsResourceInternal;
}

/** Graphics 機能に関係するリソースのベースクラスです。*/
LN_CLASS()
class LN_API GraphicsResource
    : public Object
{
    LN_OBJECT;
protected:
    virtual void onDispose(bool explicitDisposing) override;
    virtual void onChangeDevice(detail::IGraphicsDevice* device) = 0;

LN_CONSTRUCT_ACCESS:
    GraphicsResource();
    virtual ~GraphicsResource();
    void init();

private:
    detail::GraphicsManager* m_manager;

    friend class detail::GraphicsResourceInternal;
};

namespace detail {

class GraphicsResourceInternal
{
public:
    static IndexBufferFormat selectIndexBufferFormat(int vertexCount) { return (vertexCount > 0xFFFF) ? IndexBufferFormat::UInt32 : IndexBufferFormat::UInt16; }

    static detail::GraphicsManager* manager(GraphicsResource* obj) { return obj->m_manager; }

    template<class TReturn, class TObject>
    static TReturn* resolveRHIObject(GraphicsContext* context, const TObject& obj, bool* outModified)
    {
        bool modified = false;
        TReturn* rhi = nullptr;
        if (obj) {
            rhi = obj->resolveRHIObject(context, &modified);
        }
        if (outModified) {
            *outModified = modified;
        }
        return rhi;
    }
};

} // namespace detail
} // namespace ln
