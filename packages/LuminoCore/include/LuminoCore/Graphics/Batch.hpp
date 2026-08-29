#pragma once
#include <vector>
#include <memory>
#include <cstring>
#include <LuminoBase/Result.hpp>
#include <LuminoBase/math/Math.hpp>
#include <LuminoCore/Graphics/Vertex.hpp>
#include <LuminoCore/Graphics/Transform.hpp>
#include <LuminoCore/Graphics/Mesh.hpp>

namespace ln {

class GraphicsContext;
class Renderer;
class Material;

//------------------------------------------------------------------------------
// DrawCommand
//------------------------------------------------------------------------------

enum class DrawCommandType : uint8_t {
    Sprite,
    SubMesh,
};

// 同一 zIndex 内の二次ソート方法。zIndex 自体は常に主キー (エンジンが指定するレイヤ/
// レンダーキュー番号) であり、本モードはその中での並びだけを決める。
//   Stable      : 投入順 (描画順 = 呼び出し順)。ポリシー中立の既定値。
//   FrontToBack : ビュー平面からの距離が小さい順 (手前→奥)。不透明の overdraw 削減向け。
//   BackToFront : ビュー平面からの距離が大きい順 (奥→手前)。半透明の正しい合成向け。
//
// 距離はカメラ位置からのユークリッド距離ではなくビュー空間 Z (ビュー平面からの距離) を
// 用いるため、Perspective / Orthographic の両方で正しく機能する。
enum class SortMode : uint8_t {
    Stable,
    FrontToBack,
    BackToFront,
};

struct DrawCommand {
    DrawCommand()
        : type(DrawCommandType::Sprite), zIndex(0), material(nullptr), sequence(0),
          viewDepth(0.0f) {
        std::memset(&sprite, 0, sizeof(sprite));
    }
    ~DrawCommand() = default;
    DrawCommand(const DrawCommand& o) { std::memcpy(static_cast<void*>(this), &o, sizeof(*this)); }
    DrawCommand& operator=(const DrawCommand& o) { std::memcpy(static_cast<void*>(this), &o, sizeof(*this)); return *this; }

    DrawCommandType type;
    int32_t zIndex;          // ユーザー指定のソート優先度
    Material* material;  // ソートキー兼描画マテリアル
    uint32_t sequence;   // レンダーパス内での投入順 (ソートの同順位を決める)
    float viewDepth;     // ビュー平面からの距離。SortMode::FrontToBack/BackToFront 時に
                         // flush() が viewMatrix から算出する (Stable 時は未使用)。

    // --- Sprite ---
    struct SpriteData {
        Vector2 offset;     // ノードローカル空間でのスプライト位置 (transform 適用前に加算)
        Vector2 size;
        Vector2 pivot;      // 矩形上の基準点 (0,0)=左上, (0.5,0.5)=中央, (1,1)=右下。ローカル矩形の原点。
        Vector2 uvOffset;
        Vector2 uvSize;
        Color   color;
        Matrix4x4 transform; // ワールド変換 (position/rotation/scale を畳み込んだ行列)
    };

    // --- SubMesh ---
    struct SubMeshData {
        Mesh*     mesh;
        uint32_t       submeshIndex;
        Transform transform;
    };

    union {
        SpriteData  sprite;
        SubMeshData submesh;
    };

    // 64 ビット整数にパックしたソートキー
    uint64_t sortKey() const;
};

//------------------------------------------------------------------------------
// DrawCommandBuffer
//------------------------------------------------------------------------------

class DrawCommandBuffer {
public:
    void clear();

    // ローカル矩形 (size + pivot) を offset だけずらし、transform でワールド空間へ配置する。
    // world = transform * (localCorner + offset)。
    // position / rotation / scale は transform に畳み込む。offset はノード内の
    // 多数スプライト (Tilemap 等) で transform を共有しつつ位置を変えるための軽量オフセット。
    void drawSprite(Material* material, int32_t zIndex,
                    const Matrix4x4& transform, const Vector2& offset,
                    const Vector2& size, const Vector2& pivot,
                    const Vector2& uvOffset, const Vector2& uvSize,
                    const Color& color);

    // 主 API: サブメッシュ粒度 (1 コマンド = 1 マテリアル)
    void drawSubMesh(Mesh* mesh, uint32_t submeshIndex, Material* material,
                     const Transform& transform, int32_t zIndex);

    // 簡易 API: 全サブメッシュを個別のコマンドに展開する
    void drawMesh(Mesh* mesh, const Transform& transform, int32_t zIndex);

    const std::vector<DrawCommand>& commands() const { return m_commands; }
    std::vector<DrawCommand>& commands() { return m_commands; }

private:
    std::vector<DrawCommand> m_commands;
};

//------------------------------------------------------------------------------
// SpriteMeshPool
//------------------------------------------------------------------------------

// バンプアロケータとして使う、フレームごとのスプライト DynamicMesh のプール。
//
// 描画コマンドは単一のコマンドエンコーダに記録され、submit 時にまとめて実行される。
// そのため、フレーム内の全スプライトフラッシュで 1 つの頂点/インデックスバッファを共有すると、
// 最後のフラッシュ以外が壊れる: 各 writeBuffer は、先に記録済みの描画がまだ参照している
// バッファを上書きするので、submit 時にはすべてのスプライト描画が最後のフラッシュの
// データを読むことになる。
//
// これを避けるため、フレーム内の各フラッシュには *別々の* スロット (つまり別のバッファ) を
// 渡す。resetFrame() がカーソルを巻き戻すので、スロットは次のフレームで再利用される。
// フレームをまたぐ再利用が安全なのは、キューのタイムライン上で writeBuffer が前フレームの
// submit の後に順序付けられるためである。
//
// ここでの管理は単体テストできるよう意図的に GPU を使わない。メッシュの作成は、
// ここが報告する状態をもとに呼び出し側 (BatchProcessor) が行う。
class SpriteMeshPool {
public:
    struct Slot {
        Ref<Mesh> mesh;
        uint32_t  capacity = 0; // 容量 (スプライト数)
    };

    // フレームごとのカーソルを巻き戻す。確保済みのスロットは再利用のため保持する。
    void resetFrame() { m_cursor = 0; }

    // フラッシュ用に次のスロットを予約し、カーソルを進める。返されるスロットインデックスは
    // 現在のフレーム内で一意であり、フラッシュ同士がバッファを共有しないことを保証する。
    // スロットは遅延生成されるため、メッシュが null のままだったり容量が不足していたりする。
    // 呼び出し側は slotAt() を調べ、capacity() < count ならメッシュを (再) 確保する。
    uint32_t acquireSlot();

    // 容量 `current` のスロットが `count` 個のスプライトを保持できるようにするために
    // 確保すべき容量 (スプライト数)。256 スプライトを基準に倍々に増やす。呼び出し側での
    // 再利用と直接の単体テストのための純粋なヘルパー。
    static uint32_t growCapacity(uint32_t current, uint32_t count);

    Slot& slotAt(uint32_t index) { return m_slots[index]; }
    uint32_t slotCount() const { return static_cast<uint32_t>(m_slots.size()); }
    uint32_t frameCursor() const { return m_cursor; }

private:
    std::vector<Slot> m_slots;
    uint32_t m_cursor = 0;
};

//------------------------------------------------------------------------------
// BatchProcessor
//------------------------------------------------------------------------------

class BatchProcessor {
public:
    static Result<std::unique_ptr<BatchProcessor>> create(GraphicsContext* ctx);

    // 全コマンドをソートし、バッチ化して、Renderer::drawMesh() で描画する。
    // viewMatrix は SortMode::FrontToBack/BackToFront 時にビュー空間 Z を算出するために使う
    // (Stable 時は無視される)。
    Result<void> flush(Renderer* renderer, DrawCommandBuffer* commandBuffer,
                       const Matrix4x4& viewMatrix, SortMode sortMode);

    // フレームごとのスプライトメッシュプールを巻き戻す。フラッシュごとのバッファを
    // 再利用できるよう、各フレームの開始時に 1 回呼び出すこと。
    void resetFrame() { m_spritePool.resetFrame(); }

    // テスト用に公開。mode に応じて並べ替える。FrontToBack/BackToFront は各コマンドの
    // viewDepth を読むため、呼び出し前に viewDepth が設定済みである必要がある (flush が設定する)。
    static void sortCommands(std::vector<DrawCommand>& commands,
                             SortMode mode = SortMode::Stable);

private:
    BatchProcessor() = default;

    Result<void> flushSpriteGroup(Renderer* renderer, const DrawCommand* begin, uint32_t count);
    Result<void> flushSubMeshGroup(Renderer* renderer, const DrawCommand* begin, uint32_t count);

    // フレームごとのスプライト DynamicMesh のバンプアロケータ (フラッシュごとに 1 スロット)。
    SpriteMeshPool m_spritePool;

    // CPU 側のステージングバッファ
    std::vector<Vertex>  m_vertexStaging;
    std::vector<uint32_t>     m_indexStaging;
    std::vector<SubMesh> m_submeshStaging;

    GraphicsContext* m_ctx = nullptr;
};

} // namespace ln
