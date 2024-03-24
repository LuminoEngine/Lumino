#pragma once
#include "../Common.hpp"

// ある GraphicsResource を複数のコンテキストで使いまわせるようにするための
// マッピングをサポートするためのクラス群です。
//
// 考え方は PIXI.js と同じような感じです。
// ひとつの Lumino のインスタンスの中で複数のバックエンドをサポートできるようにもなりますが、
// それよりもグラフィックスコンテキストの共有ができない複数のビューへの描画をサポートすることが目的です。
// 特に、 WebGL が該当します。
//
// これは Lyric プロジェクトのエディタ側で複数の Canvas に描画することを想定したものです。
// もし単一のコンテキストで描画する場合は gl の仮想化 (ひとつの canvas へ描いた結果を、他の canvas へ転送する) か、
// あるいは WebGPU を使うことになるかもしれません。
// 前者の gl 仮想化は、そもそも gl コンテキストは多くても 16 個程度までしか作れないことが動機のひとつです。
//
// - GraphicsResourceRegistry
// 　- Resource 種類ごとに、GraphicsManager にひとつのインスタンスがあります。
// - RHIGraphicsResourceRegistry
// 　- Resource 種類及び GraphicsContext ごとに、ひとつのインスタンスがあります。
//
// GraphicsResourceRegistry はいうなれば「元の」あるいは「最新の」リソースです。
// 描画を行う際に、必要に応じて RHIGraphicsResourceRegistry のリソースへデータを転送します。
// GraphicsResourceRegistry は RHI に依存しません。
// これによりネイティブで共有をサポートしていない gl コンテキストや、複数のバックエンド、
// デバイスロスト時の復帰などのサポートがスムーズになります。
//
//

namespace ln {
namespace detail {

class GraphicsResourceRegistry : public URefObject {
public:
    GraphicsResourceRegistry();
    ~GraphicsResourceRegistry();
    void registerResource(IGraphicsResource* resource);
    void unregisterResource(GraphicsResourceId id);

    void subscribe(RHIGraphicsResourceRegistry* rhiRegistry);
    void unsubscribe(RHIGraphicsResourceRegistry* rhiRegistry);

private:
    ln::Array<IGraphicsResource*> m_resourceList;
    std::stack<GraphicsResourceId> m_idStack;
    ln::Array<RHIGraphicsResourceRegistry*> m_rhiRegistries;
};

class RHIGraphicsResourceRegistry : public URefObject {
public:
    RHIGraphicsResourceRegistry(GraphicsResourceRegistry* ownerRegistry);
    ~RHIGraphicsResourceRegistry();
    void registerResource(IGraphicsResource* resource, RHIResource* rhiResource);
    void unregisterResource(RHIResource* rhiResource);

private:
    GraphicsResourceRegistry* m_ownerRegistry;
    ln::Array<Ref<RHIResource>> m_rhiResourceList; // strong reference.
};

} // namespace detail
} // namespace ln
