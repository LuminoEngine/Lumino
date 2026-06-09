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

struct DrawCommand {
    DrawCommand() : type(DrawCommandType::Sprite), zIndex(0), material(nullptr), sequence(0) {
        std::memset(&sprite, 0, sizeof(sprite));
    }
    ~DrawCommand() = default;
    DrawCommand(const DrawCommand& o) { std::memcpy(static_cast<void*>(this), &o, sizeof(*this)); }
    DrawCommand& operator=(const DrawCommand& o) { std::memcpy(static_cast<void*>(this), &o, sizeof(*this)); return *this; }

    DrawCommandType type;
    int32_t zIndex;          // user-specified sort priority
    Material* material;  // sort key & draw material
    uint32_t sequence;   // submission order within the render pass (SubMesh sort tiebreaker)

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

    // Sort key packed into 64-bit integer
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

    // Primary API: submesh granularity (1 command = 1 material)
    void drawSubMesh(Mesh* mesh, uint32_t submeshIndex, Material* material,
                     const Transform& transform, int32_t zIndex);

    // Convenience API: expands all submeshes into individual commands
    void drawMesh(Mesh* mesh, const Transform& transform, int32_t zIndex);

    const std::vector<DrawCommand>& commands() const { return m_commands; }
    std::vector<DrawCommand>& commands() { return m_commands; }

private:
    std::vector<DrawCommand> m_commands;
};

//------------------------------------------------------------------------------
// SpriteMeshPool
//------------------------------------------------------------------------------

// Per-frame pool of sprite DynamicMeshes used as a bump allocator.
//
// Draw commands are recorded into a single command encoder and only executed at
// submit time. Reusing one shared vertex/index buffer for every sprite flush in
// a frame therefore corrupts all but the last flush: each writeBuffer overwrites
// the buffer that earlier (already-recorded) draws still reference, so at submit
// time every sprite draw reads the final flush's data.
//
// To avoid this, each flush within a frame is handed a *distinct* slot (and thus
// a distinct buffer). resetFrame() rewinds the cursor so slots are recycled on
// the next frame; cross-frame reuse is safe because writeBuffer is ordered after
// the previous frame's submit on the queue timeline.
//
// The bookkeeping here is intentionally GPU-free so it can be unit tested: mesh
// creation is performed by the caller (BatchProcessor) using the reported state.
class SpriteMeshPool {
public:
    struct Slot {
        Ref<Mesh> mesh;
        uint32_t  capacity = 0; // capacity in sprites
    };

    // Rewind the per-frame cursor. Allocated slots are kept for reuse.
    void resetFrame() { m_cursor = 0; }

    // Reserve the next slot for a flush and advance the cursor. The returned slot
    // index is unique within the current frame, guaranteeing flushes never share
    // a buffer. The slot is created lazily, so its mesh may still be null / its
    // capacity insufficient; the caller inspects slotAt() and (re)allocates the
    // mesh when capacity() < count.
    uint32_t acquireSlot();

    // Capacity (in sprites) to allocate so that a slot with `current` capacity can
    // hold `count` sprites. Doubles from a 256-sprite base. Pure helper for reuse
    // by the caller and for direct unit testing.
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

    // Sort, batch, and draw all commands via Renderer::drawMesh().
    Result<void> flush(Renderer* renderer, DrawCommandBuffer* commandBuffer);

    // Rewind the per-frame sprite mesh pool. Call once at the start of each frame
    // so the per-flush buffers are recycled.
    void resetFrame() { m_spritePool.resetFrame(); }

    // Exposed for testing
    static void sortCommands(std::vector<DrawCommand>& commands);

private:
    BatchProcessor() = default;

    Result<void> flushSpriteGroup(Renderer* renderer, const DrawCommand* begin, uint32_t count);
    Result<void> flushSubMeshGroup(Renderer* renderer, const DrawCommand* begin, uint32_t count);

    // Per-frame bump allocator of sprite DynamicMeshes (one slot per flush).
    SpriteMeshPool m_spritePool;

    // CPU staging buffers
    std::vector<Vertex>  m_vertexStaging;
    std::vector<uint32_t>     m_indexStaging;
    std::vector<SubMesh> m_submeshStaging;

    GraphicsContext* m_ctx = nullptr;
};

} // namespace ln
