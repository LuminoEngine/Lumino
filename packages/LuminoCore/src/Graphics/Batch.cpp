#include <LuminoCore/Graphics/Batch.hpp>
#include <LuminoCore/Graphics/Renderer.hpp>
#include <LuminoCore/Graphics/GraphicsContext.hpp>
#include <algorithm>
#include <cstring>
#include <cmath>

namespace ln {

//-----------------------------------------------------------------------------
// DrawCommand
//-----------------------------------------------------------------------------

uint64_t DrawCommand::sortKey() const {
    // | 63..48 (16bit) | 47 (1bit) | 46..32 (15bit) | 31..0 (32bit)   |
    // | zIndex + 32768 | type      | reserved        | material hash   |
    //
    // タイプはビット47（32ビットmatHashの上位）を占めるため、特定のzレベルにあるすべてのスプライト（タイプ=0）は、
    // 同じレベルにあるすべてのサブメッシュ（タイプ=1）よりも前にソートされます。
    // これにより、ソートされた出力においてすべてのスプライトが連続した状態になり、
    // flushSpriteGroup がグループ全体に対して単一の頂点バッファを構築するために必要な条件が満たされます。
    uint64_t z = static_cast<uint64_t>(static_cast<uint32_t>(zIndex + 32768)) & 0xFFFF;

    // Material pointer hash - 同じ種類のコマンドを同じタイプ内でグループ化します。
    uint64_t matHash =
        static_cast<uint64_t>(
            (static_cast<uint64_t>(reinterpret_cast<uintptr_t>(material)) * 2654435761ULL) >> 16) &
        0xFFFFFFFF;

    uint64_t t = (type == DrawCommandType::SubMesh) ? 1ULL : 0ULL;

    return (z << 48) | (t << 47) | matHash;
}

//-----------------------------------------------------------------------------
// DrawCommandBuffer
//-----------------------------------------------------------------------------

void DrawCommandBuffer::clear() {
    m_commands.clear();
}

void DrawCommandBuffer::drawSprite(
    Material* material, int32_t zIndex,
    const Vector3& pos, const Vector2& size,
    const Vector2& pivot,
    const Vector2& uvOffset, const Vector2& uvSize,
    const Color& color, float rotation) {

    DrawCommand cmd{};
    cmd.type = DrawCommandType::Sprite;
    cmd.zIndex = zIndex;
    cmd.material = material;
    cmd.sprite.position = pos;
    cmd.sprite.size = size;
    cmd.sprite.pivot = pivot;
    cmd.sprite.uvOffset = uvOffset;
    cmd.sprite.uvSize = uvSize;
    cmd.sprite.color = color;
    cmd.sprite.rotation = rotation;
    m_commands.push_back(cmd);
}

void DrawCommandBuffer::drawSubMesh(
    Mesh* mesh, uint32_t submeshIndex, Material* material,
    const Transform& transform, int32_t zIndex) {

    DrawCommand cmd{};
    cmd.type = DrawCommandType::SubMesh;
    cmd.zIndex = zIndex;
    cmd.material = material;
    cmd.submesh.mesh = mesh;
    cmd.submesh.submeshIndex = submeshIndex;
    cmd.submesh.transform = transform;
    m_commands.push_back(cmd);
}

void DrawCommandBuffer::drawMesh(Mesh* mesh, const Transform& transform, int32_t zIndex) {
    const auto& submeshes = mesh->submeshes();
    const auto& materials = mesh->materials();
    for (uint32_t i = 0; i < static_cast<uint32_t>(submeshes.size()); ++i) {
        Material* mat = nullptr;
        if (submeshes[i].materialIndex < materials.size()) {
            mat = materials[submeshes[i].materialIndex].get();
        } else if (!materials.empty()) {
            mat = materials[0].get();
        }
        drawSubMesh(mesh, i, mat, transform, zIndex);
    }
}


//-----------------------------------------------------------------------------
// SpriteMeshPool
//-----------------------------------------------------------------------------

uint32_t SpriteMeshPool::acquireSlot() {
    uint32_t index = m_cursor++;
    if (index >= m_slots.size()) {
        m_slots.resize(index + 1);
    }
    return index;
}

uint32_t SpriteMeshPool::growCapacity(uint32_t current, uint32_t count) {
    uint32_t newCapacity = current == 0 ? 256 : current;
    while (newCapacity < count) newCapacity *= 2;
    return newCapacity;
}

//-----------------------------------------------------------------------------
// BatchProcessor
//-----------------------------------------------------------------------------

Result<std::unique_ptr<BatchProcessor>> BatchProcessor::create(GraphicsContext* ctx) {
    auto processor = std::unique_ptr<BatchProcessor>(new BatchProcessor());
    processor->m_ctx = ctx;
    return processor;
}

void BatchProcessor::sortCommands(std::vector<DrawCommand>& commands) {
    std::stable_sort (commands.begin(), commands.end(),
        [](const DrawCommand& a, const DrawCommand& b) {
            return a.sortKey() < b.sortKey();
        });
}

Result<void> BatchProcessor::flush(Renderer* renderer, DrawCommandBuffer* commandBuffer) {
    auto& commands = commandBuffer->commands();
    if (commands.empty()) return {};

    sortCommands(commands);

    uint32_t groupStart = 0;
    uint32_t count = static_cast<uint32_t>(commands.size());

    while (groupStart < count) {
        DrawCommandType groupType = commands[groupStart].type;
        uint32_t groupEnd = groupStart + 1;

        // Find contiguous group of same type.
        // Sprites with different materials are batched together into one flushSpriteGroup call,
        // which builds separate submeshes per material in a single vertex buffer.
        // Splitting by material here would cause each group to overwrite the same m_spriteMesh
        // buffer from offset 0, corrupting vertex data for earlier draw calls.
        while (groupEnd < count &&
               commands[groupEnd].type == groupType) {
            ++groupEnd;
        }

        uint32_t groupCount = groupEnd - groupStart;

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
    Renderer* renderer, const DrawCommand* begin, uint32_t count) {

    // Acquire a distinct pool slot for this flush. Each flush within a frame must
    // use its own buffer: draws are recorded now but executed at submit, so a
    // shared buffer would let later flushes overwrite earlier flushes' vertices.
    uint32_t slotIndex = m_spritePool.acquireSlot();
    auto& slot = m_spritePool.slotAt(slotIndex);
    if (count > slot.capacity) {
        uint32_t newCapacity = SpriteMeshPool::growCapacity(slot.capacity, count);

        auto result = Mesh::createDynamic(m_ctx->device(), newCapacity * 4, newCapacity * 6);
        if (!result) return LN_FORWARD_ERROR(result);
        slot.mesh = std::move(*result);
        slot.capacity = newCapacity;
    }
    Mesh* spriteMesh = slot.mesh.get();

    // 2D カメラの場合、頂点 cy の符号を反転して左上原点 (Y+ 下) のレイアウトに切り替える。
    // Y 反転プロジェクションとの二重反転により NDC ワインディングは CCW のまま保たれる。
    const bool layout2D = renderer->isCurrentCamera2D();

    uint32_t totalVertices = count * 4;
    uint32_t totalIndices = count * 6;
    m_vertexStaging.resize(totalVertices);
    m_indexStaging.resize(totalIndices);
    m_submeshStaging.clear();

    // Track material boundaries for submesh splitting
    Material* currentMaterial = begin[0].material;
    uint32_t batchStartSprite = 0;

    for (uint32_t i = 0; i <= count; ++i) {
        // Detect material boundary
        if (i == count || begin[i].material != currentMaterial) {
            SubMesh sub;
            sub.indexOffset = batchStartSprite * 6;
            sub.indexCount = (i - batchStartSprite) * 6;
            sub.materialIndex = static_cast<uint32_t>(m_submeshStaging.size());
            m_submeshStaging.push_back(sub);

            if (i < count) {
                currentMaterial = begin[i].material;
                batchStartSprite = i;
            }
        }

        if (i == count) break;

        const auto& s = begin[i].sprite;
        float w = s.size.x;
        float h = s.size.y;

        // pivot は矩形上の基準点 (0,0)=視覚的な左上, (0.5,0.5)=中央, (1,1)=右下。
        // pos がこの pivot 位置に一致し、下の rotation もこの点を軸に回転する。
        float left  = -s.pivot.x * w;
        float right = (1.0f - s.pivot.x) * w;

        // Corner offsets (before rotation)
        //   v0=TL(left,top) v1=TR(right,top) v2=BL(left,bottom) v3=BR(right,bottom)
        //   3D (Y+ up):   top は +、bottom は - (pivot.y=0 が視覚的な上端)
        //   2D (Y+ down): top/bottom の符号が反転
        // UV / index winding は両モード共通。2D は ortho Y 反転と組み合わさり NDC で CCW のまま。
        float cx[4] = { left, right, left, right };
        float cy[4];
        if (layout2D) {
            float top    = -s.pivot.y * h;
            float bottom = (1.0f - s.pivot.y) * h;
            cy[0] = top; cy[1] = top; cy[2] = bottom; cy[3] = bottom;
        } else {
            float top    =  s.pivot.y * h;
            float bottom = -(1.0f - s.pivot.y) * h;
            cy[0] = top; cy[1] = top; cy[2] = bottom; cy[3] = bottom;
        }

        // Apply rotation if non-zero
        if (s.rotation != 0.0f) {
            float cosR = std::cos(s.rotation);
            float sinR = std::sin(s.rotation);
            for (int j = 0; j < 4; ++j) {
                float rx = cx[j] * cosR - cy[j] * sinR;
                float ry = cx[j] * sinR + cy[j] * cosR;
                cx[j] = rx;
                cy[j] = ry;
            }
        }

        float u0 = s.uvOffset.x;
        float v0 = s.uvOffset.y;
        float u1 = s.uvOffset.x + s.uvSize.x;
        float v1 = s.uvOffset.y + s.uvSize.y;

        uint32_t vi = i * 4;
        uint32_t ii = i * 6;

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
    auto vr = spriteMesh->updateVertices(0, m_vertexStaging.data(), totalVertices);
    if (!vr) {
        return vr;
    }
    auto ir = spriteMesh->updateIndices(0, m_indexStaging.data(), totalIndices);
    if (!ir) {
        return ir;
    }

    spriteMesh->setSubmeshes(m_submeshStaging);

    // Assign materials to mesh slots
    auto& meshMaterials = spriteMesh->materials();
    meshMaterials.resize(m_submeshStaging.size());
    currentMaterial = begin[0].material;
    uint32_t matSlot = 0;
    for (uint32_t i = 0; i <= count; ++i) {
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
    return renderer->drawMeshImmediate(spriteMesh, Transform::identity());
}

Result<void> BatchProcessor::flushSubMeshGroup(
    Renderer* renderer,
    const DrawCommand* begin,
    uint32_t count) {

    // Phase 1: draw each submesh individually
    for (uint32_t i = 0; i < count; ++i) {
        const auto& cmd = begin[i];
        auto result = renderer->drawSingleSubMesh(
            cmd.submesh.mesh, cmd.submesh.submeshIndex, cmd.material, cmd.submesh.transform);
        if (!result) return result;
    }

    return {};
}

} // namespace ln
