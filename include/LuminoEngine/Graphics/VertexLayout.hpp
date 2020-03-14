// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once
#include "Common.hpp"
#include "GraphicsResource.hpp"

namespace ln {
namespace detail {
class IVertexDeclaration;
}

/** 頂点バッファのレイアウトを記述するためのクラスです。 */
class VertexLayout
    : public GraphicsResource
{
public:
    /** VertexLayout を作成します。 */
    static Ref<VertexLayout> create();

    /** レイアウトに新しい要素を追加します。 */
    void addElement(int streamIndex, VertexElementType type, VertexElementUsage usage, int usageIndex, VertexInputRate rate = VertexInputRate::Vertex);

protected:
    virtual void onDispose(bool explicitDisposing) override;
    virtual void onChangeDevice(detail::IGraphicsDevice* device) override;

LN_CONSTRUCT_ACCESS:
    VertexLayout();
    virtual ~VertexLayout();

    /** @copydoc create() */
    void init();

    /** @copydoc create(const VertexElement*, int) */
    void init(const VertexElement* elements, int count);

private:
    detail::IVertexDeclaration* resolveRHIObject(GraphicsContext* context, bool* outModified);

    Ref<detail::IVertexDeclaration> m_deviceObj;
    List<VertexElement> m_vertexElements;
    bool m_modified;

    friend class detail::GraphicsResourceInternal;
};

} // namespace ln
