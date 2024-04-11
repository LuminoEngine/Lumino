#pragma once
#include "../Common.hpp"

// ある GraphicsObject を複数のコンテキストで使いまわせるようにするための
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
// - GraphicsObjectRegistry
// 　- Object 種類ごとに、GraphicsManager にひとつのインスタンスがあります。
// - RHIGraphicsObjectRegistry
// 　- Object 種類及び GraphicsContext ごとに、ひとつのインスタンスがあります。
//
// GraphicsObjectRegistry はいうなれば「元の」あるいは「最新の」リソースです。
// 描画を行う際に、必要に応じて RHIGraphicsObjectRegistry のリソースへデータを転送します。
// GraphicsObjectRegistry は RHI に依存しません。
// これによりネイティブで共有をサポートしていない gl コンテキストや、複数のバックエンド、
// デバイスロスト時の復帰などのサポートがスムーズになります。
//
//

namespace ln {
namespace detail {

class GraphicsObjectRegistry : public URefObject {
public:
    GraphicsObjectRegistry();
    ~GraphicsObjectRegistry();
    void registerObject(IGraphicsObject* object);
    void unregisterObject(IGraphicsObject* object);

    void subscribe(RHIGraphicsObjectRegistry* rhiRegistry);
    void unsubscribe(RHIGraphicsObjectRegistry* rhiRegistry);

private:
    ln::Array<IGraphicsObject*> m_resourceList;
    std::stack<GraphicsObjectId> m_idStack;
    ln::Array<RHIGraphicsObjectRegistry*> m_rhiRegistries;
};

class RHIGraphicsObjectRegistry : public URefObject {
public:
    RHIGraphicsObjectRegistry(GraphicsObjectRegistry* ownerRegistry);
    ~RHIGraphicsObjectRegistry();
    void registerObject(IGraphicsObject* resource, RHIDeviceObject* rhiObject);
    void unregisterObject(GraphicsObjectId id);
    void unregisterAllObjects();
    RHIDeviceObject* get(IGraphicsObject* object) const;

private:
    GraphicsObjectRegistry* m_ownerRegistry;
    ln::Array<Ref<RHIDeviceObject>> m_rhiObjectList; // strong reference.
};

} // namespace detail
} // namespace ln
