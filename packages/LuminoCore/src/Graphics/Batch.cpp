#include <LuminoCore/Graphics/Batch.hpp>
#include <LuminoCore/Graphics/Renderer.hpp>
#include <LuminoCore/Graphics/GraphicsContext.hpp>
#include <algorithm>
#include <cstring>
#include <cmath>

namespace ln {

// ---------------------------------------------------------------------------
// DrawCommand::sortKey
// ---------------------------------------------------------------------------

u64 DrawCommand::sortKey() const {
    // | 63..48 (16bit) | 47..16 (32bit)          | 15..1 (15bit) | 0 (1bit) |
    // | zIndex + 32768 | material ptr hash        | reserved      | type     |
    u64 z = static_cast<u64>(static_cast<u32>(zIndex + 32768)) & 0xFFFF;

    // Material pointer hash — groups same-material commands together.
    // Uses only the pointer value (no dereference), so safe with any pointer.
    u64 matHash = static_cast<u64>(
        (static_cast<u64>(reinterpret_cast<uintptr_t>(material)) * 2654435761ULL) >> 16) & 0xFFFFFFFF;

    u64 t = (type == DrawCommandType::SubMesh) ? 1 : 0;

    return (z << 48) | (matHash << 16) | t;
}

// ---------------------------------------------------------------------------
// DrawCommandBuffer
// ---------------------------------------------------------------------------

void DrawCommandBuffer::clear() {
    m_commands.clear();
}

void DrawCommandBuffer::drawSprite(
    Material* material, i32 zIndex,
    const Vector3& pos, const Vector2& size,
    const Vector2& uvOffset, const Vector2& uvSize,
    const Color& color, f32 rotation) {

    DrawCommand cmd{};
    cmd.type = DrawCommandType::Sprite;
    cmd.zIndex = zIndex;
    cmd.material = material;
    cmd.sprite.position = pos;
    cmd.sprite.size = size;
    cmd.sprite.uvOffset = uvOffset;
    cmd.sprite.uvSize = uvSize;
    cmd.sprite.color = color;
    cmd.sprite.rotation = rotation;
    m_commands.push_back(cmd);
}

void DrawCommandBuffer::drawSubMesh(
    Mesh* mesh, u32 submeshIndex, Material* material,
    const Transform& transform, i32 zIndex) {

    DrawCommand cmd{};
    cmd.type = DrawCommandType::SubMesh;
    cmd.zIndex = zIndex;
    cmd.material = material;
    cmd.submesh.mesh = mesh;
    cmd.submesh.submeshIndex = submeshIndex;
    cmd.submesh.transform = transform;
    m_commands.push_back(cmd);
}

void DrawCommandBuffer::drawMesh(Mesh* mesh, const Transform& transform, i32 zIndex) {
    const auto& submeshes = mesh->submeshes();
    const auto& materials = mesh->materials();
    for (u32 i = 0; i < static_cast<u32>(submeshes.size()); ++i) {
        Material* mat = nullptr;
        if (submeshes[i].materialIndex < materials.size()) {
            mat = materials[submeshes[i].materialIndex].get();
        } else if (!materials.empty()) {
            mat = materials[0].get();
        }
        drawSubMesh(mesh, i, mat, transform, zIndex);
    }
}

// ---------------------------------------------------------------------------
// BatchProcessor
// ---------------------------------------------------------------------------

Result<std::unique_ptr<BatchProcessor>> BatchProcessor::create(GraphicsContext* ctx) {
    auto processor = std::unique_ptr<BatchProcessor>(new BatchProcessor());
    processor->m_ctx = ctx;
    return processor;
}

void BatchProcessor::sortCommands(std::vector<DrawCommand>& commands) {
    std::sort(commands.begin(), commands.end(),
        [](const DrawCommand& a, const DrawCommand& b) {
            return a.sortKey() < b.sortKey();
        });
}

Result<void> BatchProcessor::flush(Renderer* renderer, DrawCommandBuffer* commandBuffer) {
    auto& commands = commandBuffer->commands();
    if (commands.empty()) return {};

    sortCommands(commands);

    u32 groupStart = 0;
    u32 count = static_cast<u32>(commands.size());

    while (groupStart < count) {
        DrawCommandType groupType = commands[groupStart].type;
        u32 groupEnd = groupStart + 1;

        // Find contiguous group of same type + same material
        while (groupEnd < count &&
               commands[groupEnd].type == groupType &&
               commands[groupEnd].material == commands[groupStart].material) {
            ++groupEnd;
        }

        u32 groupCount = groupEnd - groupStart;

        if (groupType == DrawCommandType::Sprite) {
            auto result = flushSpriteGroup(renderer, &commands[groupStart], groupCount);
            if (!result) return result;
        } else {
            auto result = flushSubMeshGroup(renderer, &commands[groupStart], groupCount);
            if (!result) return result;
        }

        groupStart = groupEnd;
    }

    return {};
}

Result<void> BatchProcessor::flushSpriteGroup(
    Renderer* renderer, const DrawCommand* begin, u32 count) {

    // Ensure DynamicMesh has enough capacity
    if (count > m_spriteMeshCapacity) {
        u32 newCapacity = m_spriteMeshCapacity == 0 ? 256 : m_spriteMeshCapacity;
        while (newCapacity < count) newCapacity *= 2;

        auto result = Mesh::createDynamic(m_ctx->device(), newCapacity * 4, newCapacity * 6);
        if (!result) return tl::make_unexpected(result.error());
        m_spriteMesh = std::move(*result);
        m_spriteMeshCapacity = newCapacity;
    }

    u32 totalVertices = count * 4;
    u32 totalIndices = count * 6;
    m_vertexStaging.resize(totalVertices);
    m_indexStaging.resize(totalIndices);
    m_submeshStaging.clear();

    // Track material boundaries for submesh splitting
    Material* currentMaterial = begin[0].material;
    u32 batchStartSprite = 0;

    for (u32 i = 0; i <= count; ++i) {
        // Detect material boundary
        if (i == count || begin[i].material != currentMaterial) {
            SubMesh sub;
            sub.indexOffset = batchStartSprite * 6;
            sub.indexCount = (i - batchStartSprite) * 6;
            sub.materialIndex = static_cast<u32>(m_submeshStaging.size());
            m_submeshStaging.push_back(sub);

            if (i < count) {
                currentMaterial = begin[i].material;
                batchStartSprite = i;
            }
        }

        if (i == count) break;

        const auto& s = begin[i].sprite;
        f32 hw = s.size.x * 0.5f;
        f32 hh = s.size.y * 0.5f;

        // Corner offsets (before rotation)
        f32 cx[4] = { -hw,  hw, -hw,  hw };
        f32 cy[4] = {  hh,  hh, -hh, -hh };

        // Apply rotation if non-zero
        if (s.rotation != 0.0f) {
            f32 cosR = std::cos(s.rotation);
            f32 sinR = std::sin(s.rotation);
            for (int j = 0; j < 4; ++j) {
                f32 rx = cx[j] * cosR - cy[j] * sinR;
                f32 ry = cx[j] * sinR + cy[j] * cosR;
                cx[j] = rx;
                cy[j] = ry;
            }
        }

        f32 u0 = s.uvOffset.x;
        f32 v0 = s.uvOffset.y;
        f32 u1 = s.uvOffset.x + s.uvSize.x;
        f32 v1 = s.uvOffset.y + s.uvSize.y;

        u32 vi = i * 4;
        u32 ii = i * 6;

        // Generate 4 vertices
        Vertex v{};
        v.normal = {0, 0, 1};
        v.tangent = {1, 0, 0, 1};
        v.color = s.color;

        // v0: top-left
        v.position = {s.position.x + cx[0], s.position.y + cy[0], s.position.z};
        v.uv = {u0, v0};
        m_vertexStaging[vi + 0] = v;

        // v1: top-right
        v.position = {s.position.x + cx[1], s.position.y + cy[1], s.position.z};
        v.uv = {u1, v0};
        m_vertexStaging[vi + 1] = v;

        // v2: bottom-left
        v.position = {s.position.x + cx[2], s.position.y + cy[2], s.position.z};
        v.uv = {u0, v1};
        m_vertexStaging[vi + 2] = v;

        // v3: bottom-right
        v.position = {s.position.x + cx[3], s.position.y + cy[3], s.position.z};
        v.uv = {u1, v1};
        m_vertexStaging[vi + 3] = v;

        // Indices: 0,2,1, 1,2,3 (CCW)
        m_indexStaging[ii + 0] = vi + 0;
        m_indexStaging[ii + 1] = vi + 2;
        m_indexStaging[ii + 2] = vi + 1;
        m_indexStaging[ii + 3] = vi + 1;
        m_indexStaging[ii + 4] = vi + 2;
        m_indexStaging[ii + 5] = vi + 3;
    }

    // Upload to GPU
    auto vr = m_spriteMesh->updateVertices(0, m_vertexStaging.data(), totalVertices);
    if (!vr) {
        return vr;
    }
    auto ir = m_spriteMesh->updateIndices(0, m_indexStaging.data(), totalIndices);
    if (!ir) {
        return ir;
    }

    m_spriteMesh->setSubmeshes(m_submeshStaging);

    // Assign materials to mesh slots
    auto& meshMaterials = m_spriteMesh->materials();
    meshMaterials.resize(m_submeshStaging.size());
    currentMaterial = begin[0].material;
    u32 matSlot = 0;
    for (u32 i = 0; i <= count; ++i) {
        if (i == count || (i > 0 && begin[i].material != currentMaterial)) {
            // addRef + adopt: borrow a non-owning raw pointer into a Ref
            currentMaterial->addRef();
            meshMaterials[matSlot] = Ref<Material>::adopt(currentMaterial);
            if (i < count) {
                currentMaterial = begin[i].material;
                ++matSlot;
            }
        }
    }

    // Draw with identity transform (positions are already world-space)
    return renderer->drawMesh(m_spriteMesh.get(), Transform::identity());
}

Result<void> BatchProcessor::flushSubMeshGroup(
    Renderer* renderer, const DrawCommand* begin, u32 count) {

    // Phase 1: draw each submesh individually
    for (u32 i = 0; i < count; ++i) {
        const auto& cmd = begin[i];
        auto result = renderer->drawSingleSubMesh(
            cmd.submesh.mesh, cmd.submesh.submeshIndex,
            cmd.material, cmd.submesh.transform);
        if (!result) return result;
    }

    return {};
}

} // namespace ln
