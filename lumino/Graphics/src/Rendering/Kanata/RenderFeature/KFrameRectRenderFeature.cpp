#include <LuminoGraphics/Rendering/Material.hpp>
#include <LuminoGraphics/Rendering/Kanata/KBatch.hpp>
#include <LuminoGraphics/Rendering/Kanata/KBatchList.hpp>
#include <LuminoGraphics/Rendering/Kanata/RenderFeature/KFrameRectRenderFeature.hpp>
using namespace ln::detail;

namespace ln {
namespace kanata {

FrameRectRenderFeature::FrameRectRenderFeature(detail::RenderingManager* manager)
    : m_currentCollector(nullptr)
    , m_material(nullptr) {
}

void FrameRectRenderFeature::beginBatch(BatchCollector* collector, Material* material) {
    LN_DCHECK(!m_currentCollector);
    LN_DCHECK(collector);
    LN_DCHECK(material);
    m_currentCollector = collector;
    m_material = material;
    m_sprites.clear();

    if (auto* texture = m_material->mainTexture()) {
        m_srcTextureSize.width = texture->width();
        m_srcTextureSize.height = texture->height();
    }
    else {
        m_srcTextureSize.width = 1;
        m_srcTextureSize.height = 1;
    }
}

void FrameRectRenderFeature::endBatch(BatchCollector* collector) {
    LN_DCHECK(m_currentCollector);
    LN_DCHECK(m_currentCollector == collector);

    int32_t spriteCount = m_sprites.length();
    int32_t vertexCount = m_sprites.length() * 4;
    int32_t indexCount = m_sprites.length() * 6;
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
        //putVertices(m_sprites[i], view.vertexData + (i * 4));
        const auto& sprite = m_sprites[i];

        float pos_l = sprite.rect.getLeft();
        float pos_r = sprite.rect.getRight();
        float pos_t = sprite.rect.getTop();
        float pos_b = sprite.rect.getBottom();
        float uv_l = sprite.srcUVRect.getLeft();
        float uv_r = sprite.srcUVRect.getRight();
        float uv_t = sprite.srcUVRect.getTop();
        float uv_b = sprite.srcUVRect.getBottom();

        //uint64_t lockOffset = m_batchData.spriteOffset * 4;
        //uint64_t lockSize = (m_batchData.spriteCount + 1) * 4;
        //m_mappedVertices = static_cast<Vertex*>(m_vertexBuffer->map(MapMode::Write));
        //uint64_t offset = ((batch->data.spriteOffset + batch->data.spriteCount) * 4);
        //m_mappedVertices = static_cast<Vertex*>(m_vertexBuffer->writableData(0, offset * sizeof(Vertex)));
        auto* vertices = view.vertexData + (i * 4);
        vertices[0].set(Vector3(pos_l, pos_t, 0), Vector3::UnitZ, Vector2(uv_l, uv_t), Color::White);   // top-left
        vertices[1].set(Vector3(pos_l, pos_b, 0), Vector3::UnitZ, Vector2(uv_l, uv_b), Color::White);   // bottom-left
        vertices[2].set(Vector3(pos_r, pos_t, 0), Vector3::UnitZ, Vector2(uv_r, uv_t), Color::White);   // top-right
        vertices[3].set(Vector3(pos_r, pos_b, 0), Vector3::UnitZ, Vector2(uv_r, uv_b), Color::White);   // bottom-right

        // transform
        vertices[0].transformPosition(*m_worldTransform);
        vertices[1].transformPosition(*m_worldTransform);
        vertices[2].transformPosition(*m_worldTransform);
        vertices[3].transformPosition(*m_worldTransform);
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

    m_currentCollector = nullptr;
    m_material = nullptr;
}

void FrameRectRenderFeature::draw(
    const Rect& rect,
    const Matrix& worldTransform,
    const Thickness& borderThickness,
    const Rect& srcRect,
    Sprite9DrawMode wrapMode) {

    m_worldTransform = &worldTransform;

    if (wrapMode == Sprite9DrawMode::StretchedSingleImage) {
        Size rec(1.0f / m_srcTextureSize.width, 1.0f / m_srcTextureSize.height);
        Rect uvRect(srcRect.x * rec.width, srcRect.y * rec.height, srcRect.width * rec.width, srcRect.height * rec.height);
        putRectangleStretch(rect, uvRect);
    }
    else if (wrapMode == Sprite9DrawMode::RepeatedSingleImage) {
        Size rec(1.0f / m_srcTextureSize.width, 1.0f / m_srcTextureSize.height);
        Rect uvRect(srcRect.x * rec.width, srcRect.y * rec.height, srcRect.width * rec.width, srcRect.height * rec.height);
        putRectangleTiling(rect, srcRect, uvRect);
    }
    else if (
        wrapMode == Sprite9DrawMode::StretchedBoxFrame ||
        wrapMode == Sprite9DrawMode::RepeatedBoxFrame ||
        wrapMode == Sprite9DrawMode::StretchedBorderFrame ||
        wrapMode == Sprite9DrawMode::RepeatedBorderFrame) {
        // 枠
        {
            // TODO: thickness が left しか対応できていない
            putFrameRectangle(rect, borderThickness, srcRect, wrapMode);
        }

        // Inner
        if (wrapMode == Sprite9DrawMode::RepeatedBoxFrame ||
            wrapMode == Sprite9DrawMode::StretchedBoxFrame) {
            Rect dstRect = rect;
            dstRect.x += borderThickness.left;
            dstRect.y += borderThickness.top;
            dstRect.width -= borderThickness.width();
            dstRect.height -= borderThickness.height();

            Rect innterSrcRect = srcRect;
            innterSrcRect.x += borderThickness.left;
            innterSrcRect.y += borderThickness.top;
            innterSrcRect.width -= borderThickness.width();
            innterSrcRect.height -= borderThickness.height();

            Size texSize(1.0f / m_srcTextureSize.width, 1.0f / m_srcTextureSize.height);
            Rect uvSrcRect(innterSrcRect.x * texSize.width, innterSrcRect.y * texSize.height, innterSrcRect.width * texSize.width, innterSrcRect.height * texSize.height);

            putRectangle(dstRect, innterSrcRect, uvSrcRect, wrapMode);
        }
    }
}

void FrameRectRenderFeature::putRectangleStretch(const Rect& rect, const Rect& srcUVRect) {
    if (rect.isEmpty()) {
        return;
    } // 矩形がつぶれているので書く必要はない

    float pos_l = rect.getLeft();
    float pos_r = rect.getRight();
    float pos_t = rect.getTop();
    float pos_b = rect.getBottom();
    float uv_l = srcUVRect.getLeft();
    float uv_r = srcUVRect.getRight();
    float uv_t = srcUVRect.getTop();
    float uv_b = srcUVRect.getBottom();

    addSprite(rect, srcUVRect);
}

void FrameRectRenderFeature::putRectangleTiling(const Rect& rect, const Rect& srcPixelRect, const Rect& srcUVRect) {
    if (rect.isEmpty()) return; // 矩形がつぶれているので書く必要はない

    int blockCountW = std::ceil(rect.width / srcPixelRect.width);   // 横方向にいくつのタイルを並べられるか (0.5 など、端数も含む)
    int blockCountH = std::ceil(rect.height / srcPixelRect.height); // 縦方向にいくつのタイルを並べられるか (0.5 など、端数も含む)

    float bw = (float)srcPixelRect.width;
    float bh = (float)srcPixelRect.height;

    ln::Vertex v;
    v.color.set(1, 1, 1, 1);
    v.normal.set(0, 0, 1, 0);

    float uv_l = srcUVRect.x;
    float uv_t = srcUVRect.y;

    for (int y = 0; y < blockCountH; y++) {
        float pos_t = bh * y;
        float pos_b = pos_t + bh;
        float uv_b = srcUVRect.getBottom();
        if (pos_b > rect.height) {
            float ratio = (1.0f - (pos_b - rect.height) / bh);
            pos_b = rect.height;
            uv_b = srcUVRect.y + srcUVRect.height * ratio;
        }

        for (int x = 0; x < blockCountW; x++) {
            float pos_l = bw * x;
            float pos_r = pos_l + bw;
            float uv_r = srcUVRect.getRight();
            if (pos_r > rect.width) {
                float ratio = (1.0f - (pos_r - rect.width) / bw);
                pos_r = rect.width;
                uv_r = srcUVRect.x + srcUVRect.width * ratio;
            }

            addSprite(
                Rect(rect.x + pos_l, rect.y + pos_t, pos_r - pos_l, pos_b - pos_t),
                Rect(uv_l, uv_t, uv_r - uv_l, uv_b - uv_t));
        }
    }
}

void FrameRectRenderFeature::putRectangle(const Rect& rect, const Rect& srcPixelRect, const Rect& srcUVRect, Sprite9DrawMode wrapMode) {
    if (wrapMode == Sprite9DrawMode::StretchedBorderFrame || wrapMode == Sprite9DrawMode::StretchedBoxFrame) {
        putRectangleStretch(rect, srcUVRect);
    }
    else if (wrapMode == Sprite9DrawMode::RepeatedBorderFrame || wrapMode == Sprite9DrawMode::RepeatedBoxFrame) {
        putRectangleTiling(rect, srcPixelRect, srcUVRect);
    }
}

void FrameRectRenderFeature::putFrameRectangle(const Rect& rect, const Thickness& borderThickness, Rect srcRect, Sprite9DrawMode wrapMode) {
    if (srcRect.isEmpty()) return;

    if (srcRect.width == INT_MAX) {
        srcRect.width = m_srcTextureSize.width;
    }
    if (srcRect.height == INT_MAX) {
        srcRect.height = m_srcTextureSize.height;
    }

    const Size texSize(1.0f / m_srcTextureSize.width, 1.0f / m_srcTextureSize.height);
    const Rect uvSrcRect(srcRect.x * texSize.width, srcRect.y * texSize.height, srcRect.width * texSize.width, srcRect.height * texSize.height);

    Thickness dstFrame;
    Thickness uvFrame;
    ThicknessI srcFrame;
    {
        Thickness baseThickness = borderThickness;

        // 横幅が小さいため、枠幅も狭めなければならない
        if (rect.width < baseThickness.left + baseThickness.right) {
            baseThickness.left = rect.width / 2;
            baseThickness.right = rect.width / 2;
        }
        // 縦幅が小さいため、枠幅も狭めなければならない
        if (rect.height < baseThickness.top + baseThickness.bottom) {
            baseThickness.top = rect.height / 2;
            baseThickness.bottom = rect.height / 2;
        }

        dstFrame.left = baseThickness.left;
        dstFrame.right = baseThickness.right;
        dstFrame.top = baseThickness.top;
        dstFrame.bottom = baseThickness.bottom;
        uvFrame.left = baseThickness.left * texSize.width;
        uvFrame.right = baseThickness.right * texSize.width;
        uvFrame.top = baseThickness.top * texSize.height;
        uvFrame.bottom = baseThickness.bottom * texSize.height;
        srcFrame.left = (int)baseThickness.left; // 型変換回数を減らすため、あらかじめ int 化しておく TODO: ←必要？floatでよいのでは
        srcFrame.right = (int)baseThickness.right;
        srcFrame.top = (int)baseThickness.top;
        srcFrame.bottom = (int)baseThickness.bottom;
    }

    Rect outerRect = rect;
    Rect innerRect(outerRect.x + dstFrame.left, outerRect.y + dstFrame.top, outerRect.width - (dstFrame.left + dstFrame.right), outerRect.height - (dstFrame.top + dstFrame.bottom));
    Rect outerUVRect = uvSrcRect;
    Rect innerUVRect(outerUVRect.x + uvFrame.left, outerUVRect.y + uvFrame.top, outerUVRect.width - (uvFrame.left + uvFrame.right), outerUVRect.height - (uvFrame.top + uvFrame.bottom));
    Rect outerSrcRect = srcRect;
    Rect innerSrcRect(outerSrcRect.x + srcFrame.left, outerSrcRect.y + srcFrame.top, outerSrcRect.width - (srcFrame.left + srcFrame.right), outerSrcRect.height - (srcFrame.top + srcFrame.bottom));

    // 左上	■□□
    //		□　□
    //		□□□
    putRectangle(
        Rect(outerRect.getLeft(), outerRect.getTop(), dstFrame.left, dstFrame.top),
        Rect(outerSrcRect.getLeft(), outerSrcRect.getTop(), srcFrame.left, srcFrame.top),
        Rect(outerUVRect.getLeft(), outerUVRect.getTop(), uvFrame.left, uvFrame.top),
        wrapMode);

    // 上	□■□
    //		□　□
    //		□□□
    putRectangle(
        Rect(innerRect.getLeft(), outerRect.getTop(), innerRect.width, dstFrame.top),
        Rect(innerSrcRect.getLeft(), outerSrcRect.getTop(), innerSrcRect.width, srcFrame.top),
        Rect(innerUVRect.getLeft(), outerUVRect.getTop(), innerUVRect.width, uvFrame.top),
        wrapMode);

    // 右上	□□■
    //		□　□
    //		□□□
    putRectangle(
        Rect(innerRect.getRight(), outerRect.getTop(), dstFrame.right, dstFrame.top),
        Rect(innerSrcRect.getRight(), outerSrcRect.getTop(), srcFrame.right, srcFrame.top),
        Rect(innerUVRect.getRight(), outerUVRect.getTop(), uvFrame.right, uvFrame.top),
        wrapMode);

    // 右	□□□
    //		□　■
    //		□□□
    putRectangle(
        Rect(innerRect.getRight(), innerRect.getTop(), dstFrame.right, innerRect.height),
        Rect(innerSrcRect.getRight(), innerSrcRect.getTop(), srcFrame.right, innerSrcRect.height),
        Rect(innerUVRect.getRight(), innerUVRect.getTop(), uvFrame.right, innerUVRect.height),
        wrapMode);

    // 右下	□□□
    //		□　□
    //		□□■
    putRectangle(
        Rect(innerRect.getRight(), innerRect.getBottom(), dstFrame.right, dstFrame.bottom),
        Rect(innerSrcRect.getRight(), innerSrcRect.getBottom(), srcFrame.right, srcFrame.bottom),
        Rect(innerUVRect.getRight(), innerUVRect.getBottom(), uvFrame.right, uvFrame.bottom),
        wrapMode);

    // 下	□□□
    //		□　□
    //		□■□
    putRectangle(
        Rect(innerRect.getLeft(), innerRect.getBottom(), innerRect.width, dstFrame.bottom),
        Rect(innerSrcRect.getLeft(), innerSrcRect.getBottom(), innerSrcRect.width, srcFrame.bottom),
        Rect(innerUVRect.getLeft(), innerUVRect.getBottom(), innerUVRect.width, uvFrame.bottom),
        wrapMode);

    // 左下	□□□
    //		□　□
    //		■□□
    putRectangle(
        Rect(outerRect.getLeft(), innerRect.getBottom(), dstFrame.left, dstFrame.bottom),
        Rect(outerSrcRect.getLeft(), innerSrcRect.getBottom(), srcFrame.left, srcFrame.bottom),
        Rect(outerUVRect.getLeft(), innerUVRect.getBottom(), uvFrame.left, uvFrame.bottom),
        wrapMode);

    // 左	□□□
    //		■　□
    //		□□□
    putRectangle(
        Rect(outerRect.getLeft(), innerRect.getTop(), dstFrame.left, innerRect.height),
        Rect(outerSrcRect.getLeft(), innerSrcRect.getTop(), srcFrame.left, innerSrcRect.height),
        Rect(outerUVRect.getLeft(), innerUVRect.getTop(), uvFrame.left, innerUVRect.height),
        wrapMode);
}

void FrameRectRenderFeature::addSprite(const Rect& rect, const Rect& srcUVRect) {
    m_sprites.push({
        rect,
        srcUVRect,
    });
}

} // namespace kanata
} // namespace ln

