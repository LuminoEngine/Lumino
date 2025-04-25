#pragma once
#include "../Common.hpp"

namespace ln {

class RendererServer : public URefObject {
public:
    static URef<RendererServer> create();
    BatchRenderer* spriteRenderer() const;

    void reset();
    void activate(BatchRenderer* renderer, CommandList* commandList, Material* material);
    void flush();

private:
    RendererServer();
    MaybeResult init();

    Ref<BatchRenderer> m_spriteRenderer;
    BatchRenderer* m_activeRenderer;
};

class CullingSystem : public URefObject {
    // NOTE: FlustumCulling や OcclusionCulling の実装は、
    //   godot の RendererSceneCull::_scene_cull() が参考になりそう。
    //
    // NOTE: DrawSprite() の中で BatchProxy を作るとすると、BatchProxy に対して Culling するのでは遅くないか？
    //   描画されないかもしれない大量のSpriteDataができてしまう懸念がある。
    //   ただこれについては、動的に作った BatchProxy はやむを得ないかもしれない。
    //   godot もそんな感じに見える。
    //   これを回避するにはやはり SceneNode のようにあらかじめ作られたデータ構造があると有利。
    //   ただこれは SceneGraph に対応しないことにした Lumino のコンセプトと反するが、
    //   例えば SingleFrameBatchProxy と対になる、事前に用意できる BatchProxy を Drawable みたいな名前で公開するのはあり。
    //
    // NOTE: カリングとZソートについて
    //   カリングは出来るだけ早い段階で行った方が処理負荷は少なくなる。
    //   Three.js は Object(SceneNode) に対して行っている。PIXI.js も Sprite が boundBox 持っているようだ。
    //
    //   Lumino としては BatchProxy に対して行うのがよいかもしれない。
    //   まず SceneGraph は Engine から独立させたいので、SceneNode は対象にならない。
    //   Tilemap を例に考えてみるとよいか。
    //   Tilemap は見える範囲の tile を動的に描画するが、この動的な生成処理は BatchProxy の担当。
    //   Batch は、そのカリングを通った Tile の集合となる。
    //
    //   Siv3D のような動的で手続き的なドローイングをする場合、ひとつひとつの図形が Proxy となる。
    //   まず Proxy としておき、 Arc を描くときもすぐには頂点を生成せず、boundBox だけ Proxy が持つ。
    //   ※Proxy 化しなくても、RenderingContext::drawArc() のような関数の中ですぐに弾いてもいいかも。
    //
    //   Tilemap や BillboardSprite(Particle) のように視点に依存して描画結果が変わるものを想定すると、
    //   getBatch() には始点情報を渡す必要がある。（既に BatchCollector::viewPoint() で渡している）
    //
    //   Zソートはやるなら Proxy のリストに対して行う。
    //   なので流れとしては、
    //
    //   1. RenderingContext::drawSprite() などで Proxy を生成する。
    //   2. Batch 化の前に、カリング & Zソートを実施する。
    //
    //   ## 先に始点情報を作るか？
    //   RenderingContext::begin()/end() を設けて、この内側でしか draw 出来ないようにする必要がある。
    //   ただ、 draw 時点でカリングできるので効率は良い。
    //   一方でシャドウマップを作る場合などでは、全く同じ RenderingContext::begin()->draw()->end() を
    //   呼び出してもらう必要がある。特に RenderingContext を公開する場合。
    //   Three.js のように SceneGraph が一級市民なフレームワークでは、 RenderingContext が隠れるので良いだろう。
    //   でも Lumino は SceneGraph が Engine から独立しているので、RenderingContext は公開される。
    //
    //   この Proxy の収取を担当しているのは BatchProxyCollector.
    //   このクラスは RenderingContext が既に持っている。
};

} // namespace ln

