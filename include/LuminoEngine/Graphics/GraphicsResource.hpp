// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once
#include "Common.hpp"

namespace ln {
namespace detail {
class GraphicsResourceInternal;
}

/** Graphics 機能に関係するリソースのベースクラスです。*/
class IGraphicsResource
{
protected:
    virtual void onManagerFinalizing() = 0;
    virtual void onChangeDevice(detail::IGraphicsDevice* device) = 0;

private:
    friend class detail::GraphicsManager;
    friend class detail::GraphicsResourceInternal;
};

namespace detail {

class GraphicsResourceInternal
{
public:
    static IndexBufferFormat selectIndexBufferFormat(int vertexCount) { return (vertexCount > 0xFFFF) ? IndexBufferFormat::UInt32 : IndexBufferFormat::UInt16; }

    template<class T>
    static detail::GraphicsManager* manager(T* obj) { return obj->m_manager; }

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


    template<class T>
    static void initializeHelper_GraphicsResource(T* obj, GraphicsManager** manager)
    {
        *manager = detail::EngineDomain::graphicsManager();
        (*manager)->addGraphicsResource(obj);
    }

    template<class T>
    static void finalizeHelper_GraphicsResource(T* obj, GraphicsManager** manager)
    {
        if (*manager) {
            (*manager)->removeGraphicsResource(obj);
            *manager = nullptr;
        }
    }
};

} // namespace detail
} // namespace ln
