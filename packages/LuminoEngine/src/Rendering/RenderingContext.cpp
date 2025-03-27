#include "Internal.hpp"
#include <LuminoEngine/Graphics/GPU/GraphicsCommandBuffer.hpp>
#include <LuminoEngine/Graphics/GPU/VertexLayout.hpp>
#include <LuminoEngine/Graphics/GPU/VertexBuffer.hpp>
#include <LuminoEngine/Graphics/Font/Font.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Rendering/RenderView.hpp>
#include <LuminoEngine/Rendering/CommandList.hpp>
#include <LuminoEngine/Mesh/MeshPrimitive.hpp>
#include <LuminoEngine/Mesh/SkinnedMeshModel.hpp>
#include <LuminoEngine/Mesh/MeshModeEntity.hpp>
#include <LuminoEngine/Graphics/Font/detail/FontManager.hpp>
#include <LuminoEngine/Mesh/detail/MeshGenerater.hpp>
#include "CommandListServer.hpp"
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>
#include "DrawElementListBuilder.hpp"

namespace ln {

//==============================================================================
// RenderingContext
/*
RenderingContext と RenderView は違うもの？
----------
RenderView は視点の数(=カメラの数) だけ存在する。
RenderingContext は RenderView の描画のたびにリセットがかかるので、1つのインスタンスを使いまわしてOK。

[2022/4/22] リセットのタイミングについて
----------
RenderView でリセットする。
ただし、今のところ World の中に UI を描画するなど、 RenderView のネストを計画している。
その場合、子 RenderView は "Sub-RenderView" のようなフラグを付けておき、これの有無でリセット掛けるかを判断してみたい。

*/

RenderingContext_deprecated::RenderingContext_deprecated()
    : m_manager(detail::RenderingManager::instance())
    , m_pathBegan(false)
    , m_commandList(nullptr)
    , m_listServer(makeRef<detail::CommandListServer>()) {
    m_commandList = m_listServer->acquirePrimaryList(RenderPart::Geometry, nullptr);
}

Result<> RenderingContext_deprecated::init() {
    if (!Object::init()) return err();
    return ok();
}

void RenderingContext_deprecated::resetForBeginRendering(const RenderViewPoint* viewPoint) {
    m_listServer->clearCommandsAndState(viewPoint);
    m_dynamicLightInfoList.clear();
}

void RenderingContext_deprecated::setRenderPass(RenderPass* value) {
    m_commandList->setRenderPass(value);
}

RenderPass* RenderingContext_deprecated::renderPass() const {
    return m_commandList->renderPass();
}

void RenderingContext_deprecated::setViewportRect(const RectI& value) {
    m_commandList->setViewportRect(value);
}

void RenderingContext_deprecated::setScissorRect(const RectI& value) {
    m_commandList->setScissorRect(value);
}

void RenderingContext_deprecated::setTransfrom(const Matrix& value) {
    m_commandList->setTransfrom(value);
}

void RenderingContext_deprecated::setBlendMode(Optional_deprecated<BlendMode> value) {
    m_commandList->setBlendMode(value);
}

void RenderingContext_deprecated::setShadingModel(Optional_deprecated<ShadingModel> value) {
    m_commandList->setShadingModel(value);
}

void RenderingContext_deprecated::setCullingMode(Optional_deprecated<CullMode> value) {
    m_commandList->setCullingMode(value);
}

void RenderingContext_deprecated::setDepthTestEnabled(Optional_deprecated<bool> value) {
    m_commandList->setDepthTestEnabled(value);
}

void RenderingContext_deprecated::setDepthWriteEnabled(Optional_deprecated<bool> value) {
    m_commandList->setDepthWriteEnabled(value);
}

void RenderingContext_deprecated::setOpacity(float value) {
    m_commandList->setOpacity(value);
}

void RenderingContext_deprecated::setMaterial(Material* material) {
    m_commandList->setMaterial(material);
}

void RenderingContext_deprecated::setRenderPhase(RenderPart value) {
    m_commandList->setRenderPhase(value);
}

void RenderingContext_deprecated::setColorScale(const Color& value) {
    m_commandList->setColorScale(value);
}

void RenderingContext_deprecated::setBlendColor(const Color& value) {
    m_commandList->setBlendColor(value);
}

void RenderingContext_deprecated::setTone(const ColorTone& value) {
    m_commandList->setTone(value);
}

void RenderingContext_deprecated::setFont(Font* value) {
    m_commandList->setFont(value);
}

void RenderingContext_deprecated::setTextColor(const Color& value) {
    m_commandList->setTextColor(value);
}

void RenderingContext_deprecated::resetState() {
    m_commandList->resetState();
}

void RenderingContext_deprecated::pushState(bool reset) {
    m_commandList->pushState(reset);
}

void RenderingContext_deprecated::popState() {
    m_commandList->popState();
}

void RenderingContext_deprecated::drawBatchProxy(kanata::BatchProxy* batchProxy) {
    m_commandList->drawBatchProxy(batchProxy);
}

void RenderingContext_deprecated::clear(Flags<ClearFlags> flags, const Color& color, float z, uint8_t stencil) {
    m_commandList->clear(flags, color, z, stencil);
}

void RenderingContext_deprecated::drawLine(const Vector3& from, const Color& fromColor, const Vector3& to, const Color& toColor) {
    m_commandList->drawLine(from, fromColor, to, toColor);
}

//void RenderingContext::drawPlane(Material* material, float width, float depth, const Color& color) {
//    m_commandList->drawPlane(material, width, depth, Vector2::Zero, Vector2::Ones, color);
//}
//
//void RenderingContext::drawPlane(Material* material, float width, float depth, const Vector2& uv1, const Vector2& uv2, const Color& color) {
//    m_commandList->drawPlane(material, width, depth, uv1, uv2, color);
//}
//
//void RenderingContext::drawSphere(Material* material, float radius, int slices, int stacks, const Color& color, const Matrix& localTransform) {
//    m_commandList->drawSphere(material, radius, slices, stacks, color, localTransform);
//}
//
//void RenderingContext::drawBox(const Box& box, const Color& color, const Matrix& localTransform) {
//    m_commandList->drawBox(box, color, localTransform);
//}

void RenderingContext_deprecated::drawRegularPolygonPrimitive(int vertexCount, float radius, const Color& color, bool fill, const Matrix& localTransform) {
    m_commandList->drawRegularPolygonPrimitive(vertexCount, radius, color, fill, localTransform);
}

//void RenderingContext::drawScreenRectangle() {
//    m_commandList->drawScreenRectangle();
//}

void RenderingContext_deprecated::blit(Material* source, RenderTargetTexture* destination/*, RenderPart phase*/) {
    m_commandList->blit(source, destination/*, phase*/);
}

//void RenderingContext::drawSprite(
//    const Matrix& transform,
//    const Size& size,
//    const Vector2& anchor,
//    const Rect& srcRect,
//    const Color& color,
//    SpriteBaseDirection baseDirection,
//    BillboardType billboardType,
//    const Flags<SpriteFlipFlags>& flipFlags,
//    Material* material) {
//    m_commandList->drawSprite(
//        transform,
//        size,
//        anchor,
//        srcRect,
//        color,
//        baseDirection,
//        billboardType,
//        flipFlags,
//        material);
//}

void RenderingContext_deprecated::drawPrimitive(VertexLayout* vertexDeclaration, VertexBuffer* vertexBuffer, PrimitiveTopology topology, int startVertex, int primitiveCount) {
    m_commandList->drawPrimitive(vertexDeclaration, vertexBuffer, topology, startVertex, primitiveCount);
}

// LOD なし。というか直接描画
void RenderingContext_deprecated::drawMesh(MeshResource* meshResource, int sectionIndex) {
    m_commandList->drawMesh(meshResource, sectionIndex);
}

void RenderingContext_deprecated::drawMesh(MeshPrimitive* mesh, int sectionIndex) {
    m_commandList->drawMesh(mesh, sectionIndex);
}

void RenderingContext_deprecated::drawSkinnedMesh(MeshPrimitive* mesh, int sectionIndex, detail::SkeletonInstance* skeleton, detail::MorphInstance* morph) {
    m_commandList->drawSkinnedMesh(mesh, sectionIndex, skeleton, morph);
}

void RenderingContext_deprecated::drawMeshInstanced(Material* material, InstancedMeshList* list) {
    m_commandList->drawMeshInstanced(material, list);
}

void RenderingContext_deprecated::drawTextSprite(const StringView& text, const Color& color, const Vector2& anchor, SpriteBaseDirection baseDirection, detail::FontRequester* font) {
    m_commandList->drawTextSprite(text, color, anchor, baseDirection, font);
}

void RenderingContext_deprecated::drawText(const StringView& text, const Rect& area, TextAlignment alignment /*, TextCrossAlignment crossAlignment*/ /*, const Color& color, Font* font*/) {
    m_commandList->drawText(text, area, alignment);
}

void RenderingContext_deprecated::drawChar(uint32_t codePoint, const Color& color, Font* font, const Matrix& transform) {
    m_commandList->drawChar(codePoint, color, font, transform);
}

void RenderingContext_deprecated::invokeExtensionRendering(INativeGraphicsExtension* extension) {
    m_commandList->invokeExtensionRendering(extension);
}

void RenderingContext_deprecated::addAmbientLight(const Color& color, float intensity) {
    addDynamicLightInfo(detail::DynamicLightInfo::makeAmbientLightInfo(color, intensity));
}

void RenderingContext_deprecated::addHemisphereLight(const Color& skyColor, const Color& groundColor, float intensity) {
    addDynamicLightInfo(detail::DynamicLightInfo::makeHemisphereLightInfo(skyColor, groundColor, intensity));
}

void RenderingContext_deprecated::addEnvironmentLightInfo(const Color& color, const Color& ambientColor, const Color& skyColor, const Color& groundColor, float intensity, const Vector3& direction, bool mainLight, float shadowCameraZFar, float shadowLightZFar) {
    addDynamicLightInfo(detail::DynamicLightInfo::makeEnvironmentLightInfo(color, ambientColor, skyColor, groundColor, intensity, direction, mainLight, shadowCameraZFar, shadowLightZFar));
}

void RenderingContext_deprecated::addDirectionalLight(const Color& color, float intensity, const Vector3& direction, bool mainLight, float shadowCameraZFar, float shadowLightZFar) {
    addDynamicLightInfo(detail::DynamicLightInfo::makeDirectionalLightInfo(color, intensity, direction, mainLight, shadowCameraZFar, shadowLightZFar));
}

void RenderingContext_deprecated::addPointLight(const Color& color, float intensity, const Vector3& position, float range, float attenuation) {
    addDynamicLightInfo(detail::DynamicLightInfo::makePointLightInfo(color, intensity, position, range, attenuation));
}

void RenderingContext_deprecated::addSpotLight(const Color& color, float intensity, const Vector3& position, const Vector3& direction, float range, float attenuation, float spotAngle, float spotPenumbra) {
    addDynamicLightInfo(detail::DynamicLightInfo::makeSpotLightInfo(color, intensity, position, direction, range, attenuation, spotAngle, spotPenumbra));
}

Size RenderingContext_deprecated::measureTextSize(Font* font, const StringView& text) const {
    if (LN_REQUIRE(font)) return Size::Zero;
    if (text.isEmpty()) return Size::Zero;
    return font->measureRenderSize(text, viewPoint()->dpiScale);
}

Size RenderingContext_deprecated::measureTextSize(Font* font, uint32_t codePoint) const {
    if (LN_REQUIRE(font)) return Size::Zero;
    if (codePoint == 0) return Size::Zero;
    return font->measureRenderSize(codePoint, viewPoint()->dpiScale);
}

CommandList* RenderingContext_deprecated::getCommandList(RenderPart index1) {
    return m_listServer->acquirePrimaryList(index1, viewPoint());
}

const RenderViewPoint* RenderingContext_deprecated::viewPoint() const {
    return m_commandList->viewPoint();
}

void RenderingContext_deprecated::setBaseTransfrom(const Optional_deprecated<Matrix>& value) {
    m_commandList->setBaseTransfrom(value);
}

const Matrix& RenderingContext_deprecated::baseTransform() const {
    return m_commandList->baseTransform();
}

void RenderingContext_deprecated::setRenderPriority(int value) {
    m_commandList->setRenderPriority(value);
}

void RenderingContext_deprecated::setBaseBuiltinEffectData(const Optional_deprecated<detail::BuiltinEffectData>& value) {
    m_commandList->setBaseBuiltinEffectData(value);
}

void RenderingContext_deprecated::setAdditionalElementFlags(detail::RenderDrawElementTypeFlags value) {
    m_commandList->setAdditionalElementFlags(value);
}

void RenderingContext_deprecated::setObjectId(int value) {
    m_commandList->setObjectId(value);
}

const Ref<detail::DrawElementListBuilder>& RenderingContext_deprecated::builder() const {
    return m_commandList->builder();
}

} // namespace ln
