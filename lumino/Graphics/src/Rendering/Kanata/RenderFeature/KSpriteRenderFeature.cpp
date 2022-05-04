#include <LuminoGraphics/Rendering/Kanata/KBatch.hpp>
#include <LuminoGraphics/Rendering/Kanata/KBatchList.hpp>
#include <LuminoGraphics/Rendering/Kanata/RenderFeature/KSpriteRenderFeature.hpp>

namespace ln {
namespace kanata {

SpriteRenderFeature::SpriteRenderFeature(detail::RenderingManager* manager)
    : m_currentCollector(nullptr)
    , m_material(nullptr)
    , m_sprites() {
}

void SpriteRenderFeature::beginBatch(BatchCollector* collector, Material* material) {
    LN_DCHECK(!m_currentCollector);
    LN_DCHECK(collector);
    LN_DCHECK(material);
    m_currentCollector = collector;
    m_material = material;
    m_sprites.clear();
}

void SpriteRenderFeature::endBatch(BatchCollector* collector) {
    LN_DCHECK(m_currentCollector);
    LN_DCHECK(m_currentCollector == collector);
    flush(m_sprites.data(), m_sprites.length());
    m_currentCollector = nullptr;
    m_material = nullptr;
}

void SpriteRenderFeature::flush(const SpriteData* sprites, int32_t spriteCount) {
    int32_t vertexCount = spriteCount * 4;
    int32_t indexCount = spriteCount * 6;
    auto view = m_currentCollector->allocateMeshBuffer(vertexCount, indexCount);

    // Write indices
    {
        auto* ib = view.indexData;
        int32_t idx = 0;
        int32_t i2 = 0;
        for (int32_t i = 0; i < spriteCount; ++i) {
            i2 = i * 6;
            idx = i * 4;
            ib[i2 + 0] = idx + 0;
            ib[i2 + 1] = idx + 1;
            ib[i2 + 2] = idx + 2;
            ib[i2 + 3] = idx + 2;
            ib[i2 + 4] = idx + 1;
            ib[i2 + 5] = idx + 3;
        }
    }

    // Write vertices
    for (int32_t i = 0; i < spriteCount; ++i) {
        putVertices(sprites[i], view.vertexData + (i * 4));
    }

    // Create batch
    {
        Batch* batch = m_currentCollector->newBatch<Batch>(1, m_material);
        batch->elemets2[0].vertexBuffers[0] = view.vertexBuffer;
        batch->elemets2[0].indexBuffer = view.indexBuffer;
        batch->elemets2[0].firstIndex = view.firstIndex;
        batch->elemets2[0].firstVertex = view.vertexOffset;
        batch->elemets2[0].primitiveCount = indexCount / 3;
        batch->vertexLayout = m_currentCollector->standardVertexDeclaration();
        batch->primitiveTopology = PrimitiveTopology::TriangleList;
    }
}

// srcRect は UV 座標系上の値を設定する。 (通常0～1)
// 以前は 2D メインな Sprite なのでピクセル単位で指定していたが、
// 考え方として他の RenderFeature と同様に「最終的な描画に使うメッシュを作る」方針で統一したい。
void SpriteRenderFeature::drawSprite(const SpriteData& sprite) {
    m_sprites.push(sprite);
}

void SpriteRenderFeature::drawSpritesDirect(BatchCollector* collector, Material* material, const SpriteData* sprite, int32_t count) {
    beginBatch(collector, material);
    flush(sprite, count);
    m_currentCollector = nullptr;
    m_material = nullptr;
}

void SpriteRenderFeature::putVertices(const SpriteData& sprite, Vertex* vertices) const {
    Vector2 center(sprite.size.x * sprite.anchorRatio.x, sprite.size.y * sprite.anchorRatio.y);
    Vector3 normal = Vector3::UnitZ;

    // 3D の場合の頂点座標
    if (sprite.baseDirection != SpriteBaseDirection::Basic2D) {
        // Vector3 origin(-center);
        float l, t, r, b;
#if 1 // 原点左下、povot.y+ が↑
        r = sprite.size.x;
        b = 0;
        l = 0;
        t = sprite.size.y;

        l -= center.x;
        r -= center.x;
        t -= center.y;
        b -= center.y;
#else
        // 原点中央 povot.y+ が↓
        Vector2 harf_size(size * 0.5f);
        r = harf_size.x;
        b = -harf_size.y;
        l = -r;
        t = -b;

        l -= center.x;
        r -= center.x;
        t -= center.y;
        b -= center.y;
#endif

#define LN_WRITE_V3(x_, y_, z_) x_, y_, z_

        switch (sprite.baseDirection) {
#ifdef LN_COORD_RH
            case SpriteBaseDirection::XPlus:
                vertices[0].setPosition(LN_WRITE_V3(0, t, l)); // 左上
                vertices[1].setPosition(LN_WRITE_V3(0, b, l)); // 左下
                vertices[2].setPosition(LN_WRITE_V3(0, t, r)); // 右上
                vertices[3].setPosition(LN_WRITE_V3(0, b, r)); // 右下
                normal = Vector3(1, 0, 0);
                break;
            case SpriteBaseDirection::YPlus:
                vertices[0].setPosition(LN_WRITE_V3(l, 0, t));
                vertices[1].setPosition(LN_WRITE_V3(l, 0, b));
                vertices[2].setPosition(LN_WRITE_V3(r, 0, t));
                vertices[3].setPosition(LN_WRITE_V3(r, 0, b));
                normal = Vector3(0, 1, 0);
                break;
            case SpriteBaseDirection::ZPlus:
                vertices[0].setPosition(LN_WRITE_V3(l, t, 0));
                vertices[1].setPosition(LN_WRITE_V3(l, b, 0));
                vertices[2].setPosition(LN_WRITE_V3(r, t, 0));
                vertices[3].setPosition(LN_WRITE_V3(r, b, 0));
                normal = Vector3(0, 0, 1);
                break;
            case SpriteBaseDirection::XMinus:
                vertices[0].setPosition(LN_WRITE_V3(0, t, r));
                vertices[1].setPosition(LN_WRITE_V3(0, b, r));
                vertices[2].setPosition(LN_WRITE_V3(0, t, l));
                vertices[3].setPosition(LN_WRITE_V3(0, b, l));
                normal = Vector3(-1, 0, 0);
                break;
            case SpriteBaseDirection::YMinus:
                vertices[0].setPosition(LN_WRITE_V3(r, 0, t));
                vertices[1].setPosition(LN_WRITE_V3(r, 0, b));
                vertices[2].setPosition(LN_WRITE_V3(l, 0, t));
                vertices[3].setPosition(LN_WRITE_V3(l, 0, b));
                normal = Vector3(0, -1, 0);
                break;
            case SpriteBaseDirection::ZMinus:
                vertices[0].setPosition(LN_WRITE_V3(l, t, 0));
                vertices[1].setPosition(LN_WRITE_V3(l, b, 0));
                vertices[2].setPosition(LN_WRITE_V3(r, t, 0));
                vertices[3].setPosition(LN_WRITE_V3(r, b, 0));
                normal = Vector3(0, 0, -1);
                break;
        }
#else
            case SpriteBaseDirection::XPlus:
                vertices[0].setPosition(LN_WRITE_V3(0, t, l)); // 左上
                vertices[1].setPosition(LN_WRITE_V3(0, t, r)); // 右上
                vertices[2].setPosition(LN_WRITE_V3(0, b, l)); // 左下
                vertices[3].setPosition(LN_WRITE_V3(0, b, r)); // 右下
                normal = Vector3(1, 0, 0);
                break;
            case SpriteBaseDirection::YPlus:
                vertices[0].setPosition(LN_WRITE_V3(l, 0, t));
                vertices[1].setPosition(LN_WRITE_V3(r, 0, t));
                vertices[2].setPosition(LN_WRITE_V3(l, 0, b));
                vertices[3].setPosition(LN_WRITE_V3(r, 0, b));
                normal = Vector3(0, 1, 0);
                break;
            case SpriteBaseDirection::ZPlus:
                vertices[0].setPosition(LN_WRITE_V3(r, t, 0));
                vertices[1].setPosition(LN_WRITE_V3(l, t, 0));
                vertices[2].setPosition(LN_WRITE_V3(r, b, 0));
                vertices[3].setPosition(LN_WRITE_V3(l, b, 0));
                normal = Vector3(0, 0, 1);
                break;
            case SpriteBaseDirection::XMinus:
                vertices[0].setPosition(LN_WRITE_V3(0, t, r));
                vertices[1].setPosition(LN_WRITE_V3(0, t, l));
                vertices[2].setPosition(LN_WRITE_V3(0, b, r));
                vertices[3].setPosition(LN_WRITE_V3(0, b, l));
                normal = Vector3(-1, 0, 0);
                break;
            case SpriteBaseDirection::YMinus:
                vertices[0].setPosition(LN_WRITE_V3(r, 0, t));
                vertices[1].setPosition(LN_WRITE_V3(l, 0, t));
                vertices[2].setPosition(LN_WRITE_V3(r, 0, b));
                vertices[3].setPosition(LN_WRITE_V3(l, 0, b));
                normal = Vector3(0, -1, 0);
                break;
            case SpriteBaseDirection::ZMinus:
                vertices[0].setPosition(LN_WRITE_V3(l, t, 0));
                vertices[1].setPosition(LN_WRITE_V3(r, t, 0));
                vertices[2].setPosition(LN_WRITE_V3(l, b, 0));
                vertices[3].setPosition(LN_WRITE_V3(r, b, 0));
                normal = Vector3(0, 0, -1);
                break;
        }
#endif
#undef LN_WRITE_V3
    }
    // 2D の場合の頂点座標
    else {
#ifdef LN_COORD_RH
        Vector2 origin(-center);
        vertices[0].setPosition(origin.x, origin.y, 0);
        vertices[1].setPosition(origin.x, origin.y + sprite.size.y, 0);
        vertices[2].setPosition(origin.x + sprite.size.x, origin.y, 0);
        vertices[3].setPosition(origin.x + sprite.size.x, origin.y + sprite.size.y, 0);
#else
        Vector2 origin(-center);
        vertices[0].setPosition(origin.x, origin.y, 0);
        vertices[1].setPosition(origin.x + size.x, origin.y, 0);
        vertices[2].setPosition(origin.x, origin.y + size.y, 0);
        vertices[3].setPosition(origin.x + size.x, origin.y + size.y, 0);
#endif
        normal = Vector3(0, 0, -1);
    }

    for (int i = 0; i < 4; i++)
        vertices[i].setNormal(normal);

    const Vector3& worldPoint = sprite.transform.position();

    Matrix actualTransform;
    {
        // ビルボード
        if (sprite.billboardType == BillboardType::ToCameraPoint) {
            Vector3 f = Vector3::normalize(m_currentCollector->viewPoint()->viewPosition - worldPoint);
            Vector3 r = Vector3::normalize(Vector3::cross(Vector3::UnitY, f));
            Vector3 u = Vector3::cross(f, r);
            actualTransform = Matrix(
                r.x, r.y, r.z, 0.0f, u.x, u.y, u.z, 0.0f, f.x, f.y, f.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
        }
        else if (sprite.billboardType == BillboardType::ToScreen) {
            // ↑がカメラ位置を基準にするのに対し、こちらはビュー平面に垂直に交差する点を基準とする。

            // ビュー平面との距離
            float d = Vector3::dot(worldPoint - m_currentCollector->viewPoint()->viewPosition, m_currentCollector->viewPoint()->viewDirection);

            // left-hand coord
            Vector3 f = Vector3::normalize(m_currentCollector->viewPoint()->viewDirection * d);
            Vector3 r = Vector3::normalize(Vector3::cross(Vector3::UnitY, f));
            Vector3 u = Vector3::cross(f, r);
            actualTransform = Matrix(
                r.x, r.y, r.z, 0.0f, u.x, u.y, u.z, 0.0f, f.x, f.y, f.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
        }
        // ビルボード・Y 軸のみに適用
        else if (sprite.billboardType == BillboardType::RotY) {
            auto posDiff = Vector3(worldPoint - m_currentCollector->viewPoint()->viewPosition);
            auto dir = m_currentCollector->viewPoint()->viewDirection;
            posDiff.y = 0.0f;
            dir.y = 0.0f;

            // ビュー平面との距離
            float d = Vector3::dot(posDiff, dir);

            // left-hand coord
            Vector3 f = Vector3::normalize(dir * d);
            Vector3 r = Vector3::normalize(Vector3::cross(Vector3::UnitY, f));
            Vector3 u = Vector3::cross(f, r);
            actualTransform = Matrix(
                r.x, r.y, r.z, 0.0f, u.x, u.y, u.z, 0.0f, f.x, f.y, f.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
            // LN_NOTIMPLEMENTED();

            // if (m_viewDirection.x > 0.0f)
            //{
            //	actualTransform.rotateY(-atanf(m_viewDirection.z / m_viewDirection.x) + Math::PI / 2);
            // }
            // else if (m_viewDirection.x == 0.0f)
            //{
            //	//D3DXMatrixIdentity(&matWorld); // 0除算を防ぐため
            // }
            // else
            //{
            //	actualTransform.rotateY(-atanf(m_viewDirection.z / m_viewDirection.x) - Math::PI / 2);
            // }
        }
        // ビルボードではない
        else {
            actualTransform = sprite.transform.getRotationMatrix();
        }

        actualTransform.translate(worldPoint);
    }

    // 座標変換
    vertices[0].transformPosition(actualTransform);
    vertices[1].transformPosition(actualTransform);
    vertices[2].transformPosition(actualTransform);
    vertices[3].transformPosition(actualTransform);

    // 色
    vertices[0].color = sprite.color;
    vertices[1].color = sprite.color;
    vertices[2].color = sprite.color;
    vertices[3].color = sprite.color;

    // テクスチャ
    {
        float l = sprite.srcUVRect.x;
        float t = sprite.srcUVRect.y;
        float r = (sprite.srcUVRect.x + sprite.srcUVRect.width);
        float b = (sprite.srcUVRect.y + sprite.srcUVRect.height);

        if (testFlag(sprite.flipFlags, SpriteFlipFlags::FlipX)) {
            std::swap(l, r);
        }
        if (testFlag(sprite.flipFlags, SpriteFlipFlags::FlipY)) {
            std::swap(t, b);
        }

#ifdef LN_COORD_RH
        vertices[0].uv.x = l;
        vertices[0].uv.y = t;
        vertices[1].uv.x = l;
        vertices[1].uv.y = b;
        vertices[2].uv.x = r;
        vertices[2].uv.y = t;
        vertices[3].uv.x = r;
        vertices[3].uv.y = b;
#else
        vertices[0].uv.x = l;
        vertices[0].uv.y = t;
        vertices[1].uv.x = r;
        vertices[1].uv.y = t;
        vertices[2].uv.x = l;
        vertices[2].uv.y = b;
        vertices[3].uv.x = r;
        vertices[3].uv.y = b;
#endif
    }
}

} // namespace kanata
} // namespace ln

