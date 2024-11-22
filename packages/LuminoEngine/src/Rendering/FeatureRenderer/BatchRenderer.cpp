#include <LuminoEngine/Rendering/Kanata/KBatchProxy.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchProxyCollector.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchList.hpp>
#include <LuminoEngine/Rendering/Kanata/RenderFeature/KSpriteRenderFeature.hpp>
#include <LuminoEngine/Rendering/FeatureRenderer/BatchRenderer.hpp>
#include <LuminoEngine/Rendering/FeatureRenderer/BatchInstructionDispatcher.hpp>
#include <LuminoEngine/Rendering/CommandList.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>

namespace ln {
namespace detail {
class DrawSpriteSFBatchProxy : public kanata::SingleFrameBatchProxy {
public:
    Material* material;
    const SpriteData* entries;
    int32_t count;
    void getBatch(kanata::BatchCollector* collector) override {
        auto* r = detail::RenderingManager::instance()->spriteRenderFeature().get();
        r->drawSpritesDirect(collector, material, entries, count);
    }
};


class InstructionBatchProxy : public kanata::SingleFrameBatchProxy {
public:
    Material* material;
    BatchInstruction* first;
    BatchInstruction* last;
    void getBatch(kanata::BatchCollector* collector) override {
        BatchInstructionDispatcher* dispatcher = collector->manager()->batchInstructionDispatcher();
        dispatcher->dispatchList(collector, material, first);
    }
};


class SpriteMeshGenerater : public MeshGenerater {
public:
    SpriteData sprite;

    int vertexCount() const override { return 4; }
    int indexCount() const override { return 6; }
    PrimitiveTopology primitiveType() const override { return PrimitiveTopology::TriangleList; }
    void onGenerate(MeshGeneraterBuffer* buf) override {
        Vector2 center(sprite.size.x * sprite.anchorRatio.x, sprite.size.y * sprite.anchorRatio.y);
        Vector3 normal = Vector3::UnitZ;
        Vertex vertices[4];

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
            normal = Vector3(0, 0, -1);
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
        }

        for (int i = 0; i < 4; i++) {
            vertices[i].setNormal(normal);
        }

        //const Vector3& worldPoint = sprite.transform.position();

        //Matrix actualTransform;
        //{
        //    // ビルボード
        //    if (sprite.billboardType == BillboardType::ToCameraPoint) {
        //        Vector3 f = Vector3::normalize(m_currentCollector->viewPoint()->viewPosition - worldPoint);
        //        Vector3 r = Vector3::normalize(Vector3::cross(Vector3::UnitY, f));
        //        Vector3 u = Vector3::cross(f, r);
        //        actualTransform =
        //            Matrix(r.x, r.y, r.z, 0.0f, u.x, u.y, u.z, 0.0f, f.x, f.y, f.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
        //    }
        //    else if (sprite.billboardType == BillboardType::ToScreen) {
        //        // ↑がカメラ位置を基準にするのに対し、こちらはビュー平面に垂直に交差する点を基準とする。

        //        // ビュー平面との距離
        //        float d = Vector3::dot(
        //            worldPoint - m_currentCollector->viewPoint()->viewPosition,
        //            m_currentCollector->viewPoint()->viewDirection);

        //        // left-hand coord
        //        Vector3 f = Vector3::normalize(m_currentCollector->viewPoint()->viewDirection * d);
        //        Vector3 r = Vector3::normalize(Vector3::cross(Vector3::UnitY, f));
        //        Vector3 u = Vector3::cross(f, r);
        //        actualTransform =
        //            Matrix(r.x, r.y, r.z, 0.0f, u.x, u.y, u.z, 0.0f, f.x, f.y, f.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
        //    }
        //    // ビルボード・Y 軸のみに適用
        //    else if (sprite.billboardType == BillboardType::RotY) {
        //        auto posDiff = Vector3(worldPoint - m_currentCollector->viewPoint()->viewPosition);
        //        auto dir = m_currentCollector->viewPoint()->viewDirection;
        //        posDiff.y = 0.0f;
        //        dir.y = 0.0f;

        //        // ビュー平面との距離
        //        float d = Vector3::dot(posDiff, dir);

        //        // left-hand coord
        //        Vector3 f = Vector3::normalize(dir * d);
        //        Vector3 r = Vector3::normalize(Vector3::cross(Vector3::UnitY, f));
        //        Vector3 u = Vector3::cross(f, r);
        //        actualTransform =
        //            Matrix(r.x, r.y, r.z, 0.0f, u.x, u.y, u.z, 0.0f, f.x, f.y, f.z, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
        //        // LN_NOTIMPLEMENTED();

        //        // if (m_viewDirection.x > 0.0f)
        //        //{
        //        //	actualTransform.rotateY(-atanf(m_viewDirection.z / m_viewDirection.x) + Math::PI / 2);
        //        // }
        //        // else if (m_viewDirection.x == 0.0f)
        //        //{
        //        //	//D3DXMatrixIdentity(&matWorld); // 0除算を防ぐため
        //        // }
        //        // else
        //        //{
        //        //	actualTransform.rotateY(-atanf(m_viewDirection.z / m_viewDirection.x) - Math::PI / 2);
        //        // }
        //    }
        //    // ビルボードではない
        //    else {
        //        actualTransform = sprite.transform.getRotationMatrix();
        //    }

        //    actualTransform.translate(worldPoint);
        //}

        // 座標変換
        //const Matrix& transform = sprite.transform;
        //vertices[0].transformPosition(transform);
        //vertices[1].transformPosition(transform);
        //vertices[2].transformPosition(transform);
        //vertices[3].transformPosition(transform);

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

        
        buf->setV(0, vertices[0]);
        buf->setV(1, vertices[1]);
        buf->setV(2, vertices[2]);
        buf->setV(3, vertices[3]);
        buf->setI(0, 0);
        buf->setI(1, 1);
        buf->setI(2, 2);
        buf->setI(3, 2);
        buf->setI(4, 1);
        buf->setI(5, 3);
    }
    void copyFrom(const SpriteMeshGenerater* other) {
        MeshGenerater::copyFrom(other);
        sprite = other->sprite;
    }
    LN_MESHGENERATOR_CLONE_IMPLEMENT(SpriteMeshGenerater);
};

} // namespace detail

BatchRenderer* BatchRenderer::get() {
    return detail::RenderingManager::instance()->spriteRenderer();
}

BatchRenderer::BatchRenderer()
    : m_commandList()
    , m_material(nullptr)
    , m_entries() {
    m_entries.reserve(MaxBatchSprites);
}

Result<> BatchRenderer::init() {
    if (!Object::init()) return err();
    return ok();
}

void BatchRenderer::begin(CommandList* commandList, Material* material) {
    if (LN_ASSERT(!m_commandList)) return;
    if (LN_ASSERT(commandList)) return;
    if (LN_ASSERT(material)) return;
    m_commandList = commandList;
    m_material = material;


    
    auto& collector = m_commandList->batchProxyCollector();
    m_currentProxy = collector->newSingleFrameBatchProxy<detail::InstructionBatchProxy>();
    m_currentProxy->material = m_material;
    m_currentProxy->first = nullptr;
    m_currentProxy->last = nullptr;
}

void BatchRenderer::end() {
    if (m_entries.size() > 0) {
        flush();
    }
    m_commandList = nullptr;
    m_material = nullptr;
    m_currentProxy = nullptr;
}

void BatchRenderer::drawSprite(const SpriteData& sprite) {
    if (m_entries.size() >= MaxBatchSprites) {
        flush();
    }

    m_entries.push_back(sprite);

    auto& collector = m_commandList->batchProxyCollector();
    detail::SpriteMeshGenerater* instruction = collector->newFrameRawData<detail::SpriteMeshGenerater>();
    instruction->type = detail::BatchInstructionType::StandardMesh;
    instruction->setTransform(sprite.transform);
    instruction->sprite = sprite;
    instruction->next = nullptr;
    if (!m_currentProxy->first) {
        m_currentProxy->first = instruction;
        m_currentProxy->last = instruction;
    }
    else {
        m_currentProxy->last->next = instruction;
        m_currentProxy->last = instruction;
    }
}

void BatchRenderer::drawSprite(
    const Matrix& transform,
    const Size& size,
    const Vector2& anchorRatio,
    const Rect& srcUVRect,
    const Color& color,
    SpriteBaseDirection baseDirection,
    BillboardType billboardType,
    const Flags<SpriteFlipFlags>& flipFlags) {
    drawSprite({
        transform,
        size,
        anchorRatio,
        srcUVRect,
        color,
        baseDirection,
        billboardType,
        flipFlags,
    });
}

void BatchRenderer::flush() {
    LN_DCHECK(m_entries.size() > 0);

    //auto& collector = m_commandList->batchProxyCollector();

    //const size_t size = sizeof(SpriteData) * m_entries.size();
    //auto* entries = collector->dataAllocator()->allocate(size);
    //memcpy(entries, m_entries.data(), size);

    //auto* proxy = collector->newSingleFrameBatchProxy<detail::DrawSpriteSFBatchProxy>();
    //proxy->material = m_material;
    //proxy->entries = static_cast<const SpriteData*>(entries);
    //proxy->count = m_entries.size();

    m_entries.clear();
}

} // namespace ln

