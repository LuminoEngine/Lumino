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
    // | 63..48 (16bit) | 47 (1bit) | 46..32 (15bit) | 31..0 (32bit) |
    // | zIndex + 32768 | type      | reserved        | sequence       |
    //
    // タイプはビット47（下位フィールドの上位）を占めるため、特定のzレベルにあるすべてのスプライト（タイプ=0）は、
    // 同じレベルにあるすべてのサブメッシュ（タイプ=1）よりも前にソートされます。
    // これにより、ソートされた出力においてすべてのスプライトが連続した状態になり、
    // flushSpriteGroup がグループ全体に対して単一の頂点バッファを構築するために必要な条件が満たされます。
    uint64_t z = static_cast<uint64_t>(static_cast<uint32_t>(zIndex + 32768)) & 0xFFFF;

    uint64_t t = (type == DrawCommandType::SubMesh) ? 1ULL : 0ULL;

    // 下位フィールドは Sprite / SubMesh いずれも投入順 (sequence) を使う。
    //
    // 以前は Sprite のみマテリアルポインタのハッシュでソートし、同一マテリアルを隣接させて
    // draw 数を減らす最適化をしていた。しかしこれは同一 zIndex のスプライトの前後関係を
    // マテリアルのアドレス次第（実質ランダム）で決めてしまい、「後から描いたものが前面に来る」
    // というペインターズアルゴリズムの期待を壊していた（半透明スプライトでは順序の入れ替え自体が
    // 合成結果を変えてしまうため不正）。
    //
    // 投入順を厳密に保持することで同 z の前後関係が描画順どおりになる。連続する同一マテリアルの
    // スプライトは flushSpriteGroup 側で 1 サブメッシュに束ねられるため、よくあるケースでは
    // draw 数は増えない（マテリアルが交互になる時だけサブメッシュが分割される）。
    uint64_t lower = static_cast<uint64_t>(sequence);

    return (z << 48) | (t << 47) | lower;
}

//-----------------------------------------------------------------------------
// DrawCommandBuffer
//-----------------------------------------------------------------------------

void DrawCommandBuffer::clear() {
    m_commands.clear();
}

void DrawCommandBuffer::drawSprite(
    Material* material, int32_t zIndex,
    const Matrix4x4& transform, const Vector2& offset,
    const Vector2& size, const Vector2& pivot,
    const Vector2& uvOffset, const Vector2& uvSize,
    const Color& color) {

    DrawCommand cmd{};
    cmd.type = DrawCommandType::Sprite;
    cmd.zIndex = zIndex;
    cmd.material = material;
    cmd.sequence = static_cast<uint32_t>(m_commands.size());
    cmd.sprite.offset = offset;
    cmd.sprite.size = size;
    cmd.sprite.pivot = pivot;
    cmd.sprite.uvOffset = uvOffset;
    cmd.sprite.uvSize = uvSize;
    cmd.sprite.color = color;
    cmd.sprite.transform = transform;
    m_commands.push_back(cmd);
}

void DrawCommandBuffer::drawSubMesh(
    Mesh* mesh, uint32_t submeshIndex, Material* material,
    const Transform& transform, int32_t zIndex) {

    DrawCommand cmd{};
    cmd.type = DrawCommandType::SubMesh;
    cmd.zIndex = zIndex;
    cmd.material = material;
    cmd.sequence = static_cast<uint32_t>(m_commands.size());
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

void BatchProcessor::sortCommands(std::vector<DrawCommand>& commands, SortMode mode) {
    // std::stable_sort ではなく std::sort を使う。stable_sort は要素数の半分の一時バッファを
    // ヒープに確保するため、毎フレーム 1 回の malloc/free になる。下の 2 つの比較関数は
    // どちらも最後に投入順 (sequence) で決着する全順序なので、等価な要素が存在せず、
    // std::sort でも stable_sort と同じ並びになる。sequence をキーから外す場合は
    // stable_sort に戻すこと。
    if (mode == SortMode::Stable) {
        // zIndex 主 + type + 投入順。sortKey() に畳み込んだ単調キーで比較する。
        std::sort(commands.begin(), commands.end(),
            [](const DrawCommand& a, const DrawCommand& b) {
                return a.sortKey() < b.sortKey();
            });
        return;
    }

    // 深度モード: zIndex を主キーに残しつつ、同一 zIndex 内をビュー平面からの距離で並べる。
    // type ビットは使わない (スプライトとサブメッシュを深度順に混在させる必要があるため)。
    // 距離が等しい場合は投入順 (sequence) で安定化する。
    const bool frontToBack = (mode == SortMode::FrontToBack);
    std::sort(commands.begin(), commands.end(),
        [frontToBack](const DrawCommand& a, const DrawCommand& b) {
            if (a.zIndex != b.zIndex) return a.zIndex < b.zIndex;
            if (a.viewDepth != b.viewDepth) {
                return frontToBack ? (a.viewDepth < b.viewDepth)
                                   : (a.viewDepth > b.viewDepth);
            }
            return a.sequence < b.sequence;
        });
}

Result<void> BatchProcessor::flush(Renderer* renderer, DrawCommandBuffer* commandBuffer,
                                   const Matrix4x4& viewMatrix, SortMode sortMode) {
    auto& commands = commandBuffer->commands();
    if (commands.empty()) return {};

    // 深度モードでは各コマンドのソート基準点をビュー空間へ変換し、ビュー平面からの距離を求める。
    // 基準点: スプライトはアンカー (transform * offset)、サブメッシュは原点 (transform の平行移動)。
    // ビュー空間 Z はカメラ前方が負になる RH 系のため、距離 = -viewZ とする
    // (Perspective/Orthographic いずれでもビュー平面からの距離として正しい)。
    if (sortMode != SortMode::Stable) {
        for (auto& cmd : commands) {
            Vector3 worldPos;
            if (cmd.type == DrawCommandType::Sprite) {
                worldPos = cmd.sprite.transform.transformCoord(
                    Vector3{cmd.sprite.offset.x, cmd.sprite.offset.y, 0.0f});
            } else {
                worldPos = cmd.submesh.transform.matrix().transformCoord(Vector3::zero());
            }
            cmd.viewDepth = -viewMatrix.transformCoord(worldPos).z;
        }
    }

    sortCommands(commands, sortMode);

    uint32_t groupStart = 0;
    uint32_t count = static_cast<uint32_t>(commands.size());

    while (groupStart < count) {
        DrawCommandType groupType = commands[groupStart].type;
        uint32_t groupEnd = groupStart + 1;

        // 同じ type の連続したグループを見つける。
        // マテリアルが異なるスプライトも 1 回の flushSpriteGroup 呼び出しにまとめられ、
        // そこで単一の頂点バッファ内にマテリアルごとの別々のサブメッシュが構築される。
        // ここでマテリアルごとに分割すると、各グループが同じ m_spriteMesh バッファを
        // オフセット 0 から上書きし、先行するドローコールの頂点データを壊してしまう。
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

    // このフラッシュ用に別々のプールスロットを取得する。フレーム内の各フラッシュは
    // 自身のバッファを使わなければならない: 描画は今記録されるが実行は submit 時なので、
    // バッファを共有すると後のフラッシュが先のフラッシュの頂点を上書きしてしまう。
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

    // サブメッシュ分割のためにマテリアルの境界を追跡する
    Material* currentMaterial = begin[0].material;
    uint32_t batchStartSprite = 0;

    for (uint32_t i = 0; i <= count; ++i) {
        // マテリアルの境界を検出する
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

        // 4 隅のオフセット (回転前)
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

        // ローカル矩形の4隅を offset だけずらし、transform でワールド空間へ変換する。
        // world = transform * (localCorner + offset)。
        // position / rotation / scale は transform に畳み込まれている。
        Vector3 world[4];
        for (int j = 0; j < 4; ++j) {
            world[j] = s.transform.transformCoord(
                Vector3{cx[j] + s.offset.x, cy[j] + s.offset.y, 0.0f});
        }

        float u0 = s.uvOffset.x;
        float v0 = s.uvOffset.y;
        float u1 = s.uvOffset.x + s.uvSize.x;
        float v1 = s.uvOffset.y + s.uvSize.y;

        uint32_t vi = i * 4;
        uint32_t ii = i * 6;

        // 4 頂点を生成する
        Vertex v{};
        v.normal = {0, 0, 1};
        v.tangent = {1, 0, 0, 1};
        v.color = s.color;

        // v0: 左上
        v.position = world[0];
        v.uv = {u0, v0};
        m_vertexStaging[vi + 0] = v;

        // v1: 右上
        v.position = world[1];
        v.uv = {u1, v0};
        m_vertexStaging[vi + 1] = v;

        // v2: 左下
        v.position = world[2];
        v.uv = {u0, v1};
        m_vertexStaging[vi + 2] = v;

        // v3: 右下
        v.position = world[3];
        v.uv = {u1, v1};
        m_vertexStaging[vi + 3] = v;

        // インデックス: 0,2,1, 1,2,3 (CCW)
        m_indexStaging[ii + 0] = vi + 0;
        m_indexStaging[ii + 1] = vi + 2;
        m_indexStaging[ii + 2] = vi + 1;
        m_indexStaging[ii + 3] = vi + 1;
        m_indexStaging[ii + 4] = vi + 2;
        m_indexStaging[ii + 5] = vi + 3;
    }

    // GPU へアップロードする
    auto vr = spriteMesh->updateVertices(0, m_vertexStaging.data(), totalVertices);
    if (!vr) {
        return vr;
    }
    auto ir = spriteMesh->updateIndices(0, m_indexStaging.data(), totalIndices);
    if (!ir) {
        return ir;
    }

    spriteMesh->setSubmeshes(m_submeshStaging.data(), static_cast<uint32_t>(m_submeshStaging.size()));

    // メッシュのスロットにマテリアルを割り当てる
    auto& meshMaterials = spriteMesh->materials();
    meshMaterials.resize(m_submeshStaging.size());
    currentMaterial = begin[0].material;
    uint32_t matSlot = 0;
    for (uint32_t i = 0; i <= count; ++i) {
        if (i == count || (i > 0 && begin[i].material != currentMaterial)) {
            // addRef + adopt: 非所有の生ポインタを Ref に取り込む
            currentMaterial->addRef();
            meshMaterials[matSlot] = Ref<Material>::adopt(currentMaterial);
            if (i < count) {
                currentMaterial = begin[i].material;
                ++matSlot;
            }
        }
    }

    // 単位トランスフォームで描画する (位置はすでにワールド空間)
    return renderer->drawMeshImmediate(spriteMesh, Transform::identity());
}

Result<void> BatchProcessor::flushSubMeshGroup(
    Renderer* renderer,
    const DrawCommand* begin,
    uint32_t count) {

    // フェーズ 1: 各サブメッシュを個別に描画する
    for (uint32_t i = 0; i < count; ++i) {
        const auto& cmd = begin[i];
        auto result = renderer->drawSingleSubMesh(
            cmd.submesh.mesh, cmd.submesh.submeshIndex, cmd.material, cmd.submesh.transform);
        if (!result) return result;
    }

    return {};
}

} // namespace ln
