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


    static void initializeHelper_GraphicsResource(IGraphicsResource* obj, GraphicsManager** manager);

    static void finalizeHelper_GraphicsResource(IGraphicsResource* obj, GraphicsManager** manager);
};

} // namespace detail
} // namespace ln
