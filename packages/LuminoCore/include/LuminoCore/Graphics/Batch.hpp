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

// ---------------------------------------------------------------------------
// DrawCommand
// ---------------------------------------------------------------------------

enum class DrawCommandType : uint8_t {
    Sprite,
    SubMesh,
};

struct DrawCommand {
    DrawCommand() : type(DrawCommandType::Sprite), zIndex(0), material(nullptr) {
        std::memset(&sprite, 0, sizeof(sprite));
    }
    ~DrawCommand() = default;
    DrawCommand(const DrawCommand& o) { std::memcpy(this, &o, sizeof(*this)); }
    DrawCommand& operator=(const DrawCommand& o) { std::memcpy(this, &o, sizeof(*this)); return *this; }

    DrawCommandType type;
    int32_t zIndex;          // user-specified sort priority
    Material* material;  // sort key & draw material

    // --- Sprite ---
    struct SpriteData {
        Vector3 position;
        Vector2 size;
        Vector2 uvOffset;
        Vector2 uvSize;
        Color   color;
        float     rotation; // Z-axis rotation (rad)
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

// ---------------------------------------------------------------------------
// DrawCommandBuffer
// ---------------------------------------------------------------------------

class DrawCommandBuffer {
public:
    void clear();

    void drawSprite(Material* material, int32_t zIndex,
                    const Vector3& pos, const Vector2& size,
                    const Vector2& uvOffset, const Vector2& uvSize,
                    const Color& color, float rotation = 0.0f);

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

// ---------------------------------------------------------------------------
// BatchProcessor
// ---------------------------------------------------------------------------

class BatchProcessor {
public:
    static Result<std::unique_ptr<BatchProcessor>> create(GraphicsContext* ctx);

    // Sort, batch, and draw all commands via Renderer::drawMesh().
    Result<void> flush(Renderer* renderer, DrawCommandBuffer* commandBuffer);

    // Exposed for testing
    static void sortCommands(std::vector<DrawCommand>& commands);

private:
    BatchProcessor() = default;

    Result<void> flushSpriteGroup(Renderer* renderer, const DrawCommand* begin, uint32_t count);
    Result<void> flushSubMeshGroup(Renderer* renderer, const DrawCommand* begin, uint32_t count);

    // Sprite DynamicMesh (reused across frames, grows as needed)
    Ref<Mesh> m_spriteMesh;
    uint32_t m_spriteMeshCapacity = 0; // current sprite capacity

    // CPU staging buffers
    std::vector<Vertex>  m_vertexStaging;
    std::vector<uint32_t>     m_indexStaging;
    std::vector<SubMesh> m_submeshStaging;

    GraphicsContext* m_ctx = nullptr;
};

} // namespace ln
