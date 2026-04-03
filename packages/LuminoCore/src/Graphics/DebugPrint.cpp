#include <LuminoCore/Graphics/DebugPrint.hpp>
#include <LuminoCore/Graphics/GraphicsContext.hpp>
#include <LuminoCore/Graphics/Renderer.hpp>
#include <LuminoCore/Graphics/TextureLoader.hpp>
#include <LuminoCore/Graphics/Transform.hpp>

// Embedded bitmap font PNG: 16x8 ASCII glyph grid, 16px wide × 20px tall per glyph.
static const unsigned char s_fontPngData[] = {
#include "text_16.png.inl"
};

namespace ln {

// Font atlas layout (must match the embedded PNG)
static constexpr float kGlyphW    = 16.0f;
static constexpr float kGlyphH    = 20.0f;
static constexpr int   kAtlasCols = 16;
static constexpr float kPadLeft   = 8.0f;
static constexpr float kPadTop    = 8.0f;

Result<Ref<DebugPrint>> DebugPrint::create(GraphicsContext* ctx) {
    auto dp = Ref<DebugPrint>::adopt(new DebugPrint());

    // Load font texture from embedded PNG.
    auto texResult = TextureLoader::loadFromMemory(
        ctx->device(), s_fontPngData, sizeof(s_fontPngData));
    if (!texResult) {
        return tl::make_unexpected(texResult.error());
    }
    dp->m_fontTexture = std::move(*texResult);

    // Unlit material: alpha blending on, depth off (overlay).
    auto matResult = MaterialFactory::createUnlit(ctx);
    if (!matResult) {
        return tl::make_unexpected(matResult.error());
    }
    dp->m_material = std::move(*matResult);
    dp->m_material->setTexture(dp->m_fontTexture.get());
    dp->m_material->setBlendEnabled(true);
    dp->m_material->setDepthTestEnabled(false);
    dp->m_material->setDepthWriteEnabled(false);

    // Dynamic mesh: worst-case kMaxChars glyphs × 6 verts each.
    auto meshResult = Mesh::createDynamic(
        ctx->device(), kMaxChars * kVertsPerChar, kMaxChars * kVertsPerChar);
    if (!meshResult) {
        return tl::make_unexpected(meshResult.error());
    }
    dp->m_mesh = std::move(*meshResult);
    dp->m_mesh->materials().push_back(dp->m_material);

    return dp;
}

void DebugPrint::print(std::string_view text) {
    for (char c : text) {
        if (m_textBufferUsed >= kMaxChars - 1) {
            break;
        }
        m_textBuffer[m_textBufferUsed++] = c;
    }
    if (m_textBufferUsed < kMaxChars) {
        m_textBuffer[m_textBufferUsed++] = '\n';
    }
}

Result<void> DebugPrint::render(GraphicsContext* ctx) {
    if (m_textBufferUsed == 0) return {};

    const float screenWidth = static_cast<float>(ctx->width());
    const float screenHeight = static_cast<float>(ctx->height());
    const float textureWidth = static_cast<float>(m_fontTexture->width());
    const float textureHeight = static_cast<float>(m_fontTexture->height());

    // Convert screen-space pixels to NDC. Origin is top-left.
    const auto ndcX = [&](float x) { return (x / screenWidth) * 2.0f - 1.0f; };
    const auto ndcY = [&](float y) { return 1.0f - (y / screenHeight) * 2.0f; };
    const auto makeVertex = [](float x, float y, float u, float v) {
        Vertex vtx;
        vtx.position = {x, y, 0.0f};
        vtx.normal = {0.0f, 0.0f, 1.0f};
        vtx.uv = {u, v};
        vtx.color = Color::white();
        vtx.tangent = {1.0f, 0.0f, 0.0f, 1.0f};
        return vtx;
    };

    std::vector<Vertex> verts;
    std::vector<u32> idxs;
    verts.reserve(m_textBufferUsed * kVertsPerChar);
    idxs.reserve(m_textBufferUsed * kVertsPerChar);

    float textX = 0.0f, textY = 0.0f;
    for (u32 i = 0; i < m_textBufferUsed; ++i) {
        char cp = m_textBuffer[i];
        if (cp == '\n') {
            textX = 0.0f;
            textY += kGlyphH;
            continue;
        }
        if (cp < 0 || cp > 127) {
            cp = '?';
        }

        // Screen-space pixel corners.
        const float sx0 = kPadLeft + textX;
        const float sy0 = kPadTop + textY;
        const float sx1 = sx0 + kGlyphW;
        const float sy1 = sy0 + kGlyphH;

        // UV in the font atlas.
        const float u0 = (cp % kAtlasCols) * kGlyphW / textureWidth;
        const float v0 = (cp / kAtlasCols) * kGlyphH / textureHeight;
        const float u1 = u0 + kGlyphW / textureWidth;
        const float v1 = v0 + kGlyphH / textureHeight;

        // NDC corners.
        const float nx0 = ndcX(sx0), ny0 = ndcY(sy0);
        const float nx1 = ndcX(sx1), ny1 = ndcY(sy1);

        u32 base = static_cast<u32>(verts.size());
        verts.push_back(makeVertex(nx0, ny0, u0, v0)); // top-left
        verts.push_back(makeVertex(nx0, ny1, u0, v1)); // bottom-left
        verts.push_back(makeVertex(nx1, ny0, u1, v0)); // top-right
        verts.push_back(makeVertex(nx1, ny1, u1, v1)); // bottom-right
        idxs.insert(idxs.end(), {base+0, base+1, base+2, base+2, base+1, base+3});

        textX += kGlyphW;
    }
    m_textBufferUsed = 0;

    if (verts.empty()) {
        return {};
    }

    // Upload geometry.
    auto r1 = m_mesh->updateVertices(0, verts.data(), static_cast<u32>(verts.size()));
    if (!r1) {
        return r1;
    }
    auto r2 = m_mesh->updateIndices(0, idxs.data(), static_cast<u32>(idxs.size()));
    if (!r2) {
        return r2;
    }
    m_mesh->setSubmeshes({{0, static_cast<u32>(idxs.size()), 0}});

    // Render as overlay (LoadOp::Load — preserves the scene).
    Renderer* renderer = ctx->renderer();
    renderer->beginOverlayRenderPass(ctx->m_currentColorTarget);
    Transform identity;
    auto dr = renderer->drawMesh(m_mesh.get(), identity, m_material.get());
    renderer->endRenderPass();
    return dr;
}

} // namespace ln
