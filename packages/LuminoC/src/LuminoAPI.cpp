#include "pch.hpp"
#include <LuminoBase.hpp>
#include <LuminoCore/CoreInstance.hpp>
#include <LuminoCore/Object.hpp>
#include <LuminoCore/runtime/ObjectRegistry.hpp>
#include <LuminoCore/graphics/Texture2D.hpp>
#include <LuminoCore/graphics/TextureLoader.hpp>
#include <LuminoCore/graphics/Material.hpp>
#include <LuminoCore/graphics/Mesh.hpp>
#include <LuminoCore/graphics/Renderer.hpp>
#include <LuminoCore/graphics/Camera.hpp>
#include <LuminoCore/graphics/Transform.hpp>
#include <LuminoCore/platform/Window.hpp>
#include <LuminoCore/graphics/GraphicsContext.hpp>
#include <LuminoC/lumino.h>
#include <cstring>

//------------------------------------------------------------------------------
// Internal wrapper classes for value types that need handle management
//------------------------------------------------------------------------------

namespace {

/** Camera は値型なので Object でラップしてハンドル管理に載せる。 */
class CameraObject : public ln::Object {
public:
    ln::Camera camera;
};

} // anonymous namespace

//------------------------------------------------------------------------------
// LNInstance
//------------------------------------------------------------------------------

LNResult LNInstance_Initialize() {
    auto result = ln::CoreInstance::initialize(ln::CoreInstance::Settings{});
    if (!result) return LN_ERROR_UNKNOWN;
    return LN_OK;
}

void LNInstance_Terminate() {
    ln::CoreInstance::terminate();
}

LNResult LNObject_Release(LNHandle handle) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;
    if (handle == LN_NULL_HANDLE) return LN_ERROR_INVALID_HANDLE;

    if (!instance->objectRegistry()->release(handle))
        return LN_ERROR_INVALID_HANDLE;

    return LN_OK;
}

LNResult LNTexture2D_Create(
    uint32_t width,
    uint32_t height,
    uint32_t format,
    LNHandle* outHandle) {
    if (!outHandle) return LN_ERROR_INVALID_ARGUMENT;
    *outHandle = LN_NULL_HANDLE;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto texture = ln::Ref<ln::Texture2D>::adopt(LN_NEW ln::Texture2D(width, height, format));
    LNHandle handle = instance->objectRegistry()->registerObject(std::move(texture));
    if (handle == LN_NULL_HANDLE) return LN_ERROR_UNKNOWN;

    *outHandle = handle;
    return LN_OK;
}

//------------------------------------------------------------------------------
// LNWindow
//------------------------------------------------------------------------------

LNResult LNWindow_Create(
    const char* title,
    uint32_t width,
    uint32_t height,
    LNHandle* outHandle) {
    if (!outHandle) return LN_ERROR_INVALID_ARGUMENT;
    *outHandle = LN_NULL_HANDLE;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    ln::platform::WindowDesc winDesc;
    winDesc.title = title ? title : "Lumino";
    winDesc.width = width;
    winDesc.height = height;

    ln::GraphicsContextDesc gfxDesc;

    auto windowResult = ln::platform::PlatformWindow::create(winDesc, gfxDesc);
    if (!windowResult) return LN_ERROR_UNKNOWN;

    auto obj = std::move(*windowResult);
    LNHandle handle = instance->objectRegistry()->registerObject(std::move(obj));
    if (handle == LN_NULL_HANDLE) return LN_ERROR_UNKNOWN;

    *outHandle = handle;
    return LN_OK;
}

LNResult LNWindow_ProcessEvents(LNHandle handle, int* outContinue) {
    if (!outContinue) return LN_ERROR_INVALID_ARGUMENT;
    *outContinue = 0;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* obj = instance->objectRegistry()->resolve<ln::platform::PlatformWindow>(handle);
    if (!obj) return LN_ERROR_INVALID_HANDLE;

    *outContinue = obj->processEvents() ? 1 : 0;
    return LN_OK;
}

LNResult LNWindow_GetGraphicsContext(LNHandle window, LNHandle* outHandle) {
    if (!outHandle) return LN_ERROR_INVALID_ARGUMENT;
    *outHandle = LN_NULL_HANDLE;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* obj = instance->objectRegistry()->resolve<ln::platform::PlatformWindow>(window);
    if (!obj) return LN_ERROR_INVALID_HANDLE;

    auto* ctx = obj->graphicsContext();
    if (!ctx) return LN_ERROR_UNKNOWN;

    // 既に登録済みの場合はハンドルを再構成して返す
    if (ctx->generation() != 0) {
        *outHandle = ln::ObjectRegistry::makeHandle(ctx->registryIndex(), ctx->generation());
        return LN_OK;
    }

    // 初回登録: window が所有しているので addRef してから登録する
    ctx->addRef();
    LNHandle handle = instance->objectRegistry()->registerObject(
        ln::Ref<ln::GraphicsContext>::adopt(ctx));
    if (handle == LN_NULL_HANDLE) return LN_ERROR_UNKNOWN;

    *outHandle = handle;
    return LN_OK;
}

//------------------------------------------------------------------------------
// LNGraphicsContext
//------------------------------------------------------------------------------

LNResult LNGraphicsContext_BeginFrame(LNHandle graphicsContext) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ctx = instance->objectRegistry()->resolve<ln::GraphicsContext>(graphicsContext);
    if (!ctx) return LN_ERROR_INVALID_HANDLE;

    auto frameResult = ctx->beginFrame();
    if (!frameResult) return LN_ERROR_UNKNOWN;

    ctx->m_currentColorTarget = frameResult->colorTarget;
    ctx->m_currentDepthTarget = frameResult->depthTarget;
    ctx->m_currentCmd = ctx->currentCommandBuffer();
    if (!ctx->m_currentCmd) return LN_ERROR_UNKNOWN;

    return LN_OK;
}

LNResult LNGraphicsContext_BeginRenderPass(
    LNHandle graphicsContext, float r, float g, float b, float a) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ctx = instance->objectRegistry()->resolve<ln::GraphicsContext>(graphicsContext);
    if (!ctx || !ctx->m_currentCmd) return LN_ERROR_INVALID_HANDLE;

    ln::rhi::ColorAttachment colorAttach;
    colorAttach.view = ctx->m_currentColorTarget;
    colorAttach.loadOp = ln::rhi::LoadOp::Clear;
    colorAttach.storeOp = ln::rhi::StoreOp::Store;
    colorAttach.clearColor = {r, g, b, a};

    ln::rhi::DepthStencilAttachment depthAttach;
    depthAttach.view = ctx->m_currentDepthTarget;
    depthAttach.depthLoadOp = ln::rhi::LoadOp::Clear;
    depthAttach.depthStoreOp = ln::rhi::StoreOp::Store;
    depthAttach.clearDepth = 1.0f;

    ln::rhi::RenderPassDesc rpDesc;
    rpDesc.colorAttachments = {colorAttach};
    rpDesc.depthStencilAttachment = &depthAttach;

    ctx->m_currentPass = ctx->m_currentCmd->beginRenderPass(rpDesc);
    if (!ctx->m_currentPass) return LN_ERROR_UNKNOWN;

    return LN_OK;
}

LNResult LNGraphicsContext_EndRenderPass(LNHandle graphicsContext) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ctx = instance->objectRegistry()->resolve<ln::GraphicsContext>(graphicsContext);
    if (!ctx || !ctx->m_currentPass) return LN_ERROR_INVALID_HANDLE;

    ctx->m_currentPass->end();
    ctx->m_currentPass = nullptr;

    return LN_OK;
}

LNResult LNGraphicsContext_EndFrame(LNHandle graphicsContext) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ctx = instance->objectRegistry()->resolve<ln::GraphicsContext>(graphicsContext);
    if (!ctx || !ctx->m_currentCmd) return LN_ERROR_INVALID_HANDLE;

    ctx->m_currentCmd->submit();
    ctx->m_currentCmd = nullptr;

    ctx->endFrame();

    return LN_OK;
}

//------------------------------------------------------------------------------
// LNTexture2D (file loading)
//------------------------------------------------------------------------------

LNResult LNTexture2D_LoadFromFile(
    LNHandle graphicsContext,
    const char* filePath,
    LNHandle* outHandle) {
    if (!outHandle || !filePath) return LN_ERROR_INVALID_ARGUMENT;
    *outHandle = LN_NULL_HANDLE;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ctx = instance->objectRegistry()->resolve<ln::GraphicsContext>(graphicsContext);
    if (!ctx) return LN_ERROR_INVALID_HANDLE;

    auto texResult = ln::TextureLoader::loadFromFile(ctx->device(), filePath);
    if (!texResult) return LN_ERROR_UNKNOWN;

    auto rhiTexture = std::move(*texResult);
    // TODO: extract actual width/height from rhi::Texture if accessor is available
    auto texture = ln::Ref<ln::Texture2D>::adopt(
        LN_NEW ln::Texture2D(std::move(rhiTexture), 0, 0));

    LNHandle handle = instance->objectRegistry()->registerObject(std::move(texture));
    if (handle == LN_NULL_HANDLE) return LN_ERROR_UNKNOWN;

    *outHandle = handle;
    return LN_OK;
}

//------------------------------------------------------------------------------
// LNMaterial
//------------------------------------------------------------------------------

LNResult LNMaterial_CreateUnlit(LNHandle graphicsContext, LNHandle* outHandle) {
    if (!outHandle) return LN_ERROR_INVALID_ARGUMENT;
    *outHandle = LN_NULL_HANDLE;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ctx = instance->objectRegistry()->resolve<ln::GraphicsContext>(graphicsContext);
    if (!ctx) return LN_ERROR_INVALID_HANDLE;

    auto matResult = ln::MaterialFactory::createUnlit(ctx);
    if (!matResult) return LN_ERROR_UNKNOWN;

    auto material = std::move(*matResult);
    LNHandle handle = instance->objectRegistry()->registerObject(std::move(material));
    if (handle == LN_NULL_HANDLE) return LN_ERROR_UNKNOWN;

    *outHandle = handle;
    return LN_OK;
}

LNResult LNMaterial_SetColor(LNHandle material, float r, float g, float b, float a) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* mat = instance->objectRegistry()->resolve<ln::Material>(material);
    if (!mat) return LN_ERROR_INVALID_HANDLE;

    mat->setColor(ln::Color{r, g, b, a});
    return LN_OK;
}

LNResult LNMaterial_SetTexture(LNHandle material, LNHandle texture) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* mat = instance->objectRegistry()->resolve<ln::Material>(material);
    if (!mat) return LN_ERROR_INVALID_HANDLE;

    auto* tex = instance->objectRegistry()->resolve<ln::Texture2D>(texture);
    if (!tex) return LN_ERROR_INVALID_HANDLE;

    mat->setTexture(tex->rhiTexture());
    return LN_OK;
}

//------------------------------------------------------------------------------
// LNMesh
//------------------------------------------------------------------------------

LNResult LNMesh_Create(
    LNHandle graphicsContext,
    const LNVertex* vertices,
    uint32_t vertexCount,
    const uint32_t* indices,
    uint32_t indexCount,
    const LNSubMesh* submeshes,
    uint32_t submeshCount,
    LNHandle* outHandle) {
    if (!outHandle || !vertices || !indices || !submeshes) return LN_ERROR_INVALID_ARGUMENT;
    if (vertexCount == 0 || indexCount == 0 || submeshCount == 0) return LN_ERROR_INVALID_ARGUMENT;
    *outHandle = LN_NULL_HANDLE;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ctx = instance->objectRegistry()->resolve<ln::GraphicsContext>(graphicsContext);
    if (!ctx) return LN_ERROR_INVALID_HANDLE;

    // Convert LNVertex[] → std::vector<ln::Vertex>
    std::vector<ln::Vertex> verts(vertexCount);
    for (uint32_t i = 0; i < vertexCount; i++) {
        const auto& sv = vertices[i];
        auto& dv = verts[i];
        dv.position = {sv.posX, sv.posY, sv.posZ};
        dv.normal   = {sv.normX, sv.normY, sv.normZ};
        dv.uv       = {sv.u, sv.v};
        dv.color    = {sv.colorR, sv.colorG, sv.colorB, sv.colorA};
        dv.tangent  = {sv.tanX, sv.tanY, sv.tanZ, sv.tanW};
    }

    // Convert indices
    std::vector<ln::u32> idx(indices, indices + indexCount);

    // Convert LNSubMesh[] → std::vector<ln::SubMesh>
    std::vector<ln::SubMesh> subs(submeshCount);
    for (uint32_t i = 0; i < submeshCount; i++) {
        subs[i].indexOffset   = submeshes[i].indexOffset;
        subs[i].indexCount    = submeshes[i].indexCount;
        subs[i].materialIndex = submeshes[i].materialIndex;
    }

    auto meshResult = ln::Mesh::create(ctx->device(), verts, idx, subs);
    if (!meshResult) return LN_ERROR_UNKNOWN;

    auto mesh = std::move(*meshResult);
    LNHandle handle = instance->objectRegistry()->registerObject(std::move(mesh));
    if (handle == LN_NULL_HANDLE) return LN_ERROR_UNKNOWN;

    *outHandle = handle;
    return LN_OK;
}

LNResult LNMesh_SetMaterial(LNHandle meshHandle, uint32_t materialIndex, LNHandle materialHandle) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* mesh = instance->objectRegistry()->resolve<ln::Mesh>(meshHandle);
    if (!mesh) return LN_ERROR_INVALID_HANDLE;

    auto* mat = instance->objectRegistry()->resolve<ln::Material>(materialHandle);
    if (!mat) return LN_ERROR_INVALID_HANDLE;

    auto& materials = mesh->materials();
    if (materialIndex >= materials.size()) return LN_ERROR_INVALID_ARGUMENT;

    // addRef because materials() stores Ref<Material>
    mat->addRef();
    materials[materialIndex] = ln::Ref<ln::Material>::adopt(mat);

    return LN_OK;
}

//------------------------------------------------------------------------------
// LNCamera
//------------------------------------------------------------------------------

LNResult LNCamera_Create(LNHandle* outHandle) {
    if (!outHandle) return LN_ERROR_INVALID_ARGUMENT;
    *outHandle = LN_NULL_HANDLE;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto camObj = ln::Ref<CameraObject>::adopt(LN_NEW CameraObject());
    LNHandle handle = instance->objectRegistry()->registerObject(std::move(camObj));
    if (handle == LN_NULL_HANDLE) return LN_ERROR_UNKNOWN;

    *outHandle = handle;
    return LN_OK;
}

LNResult LNCamera_SetPerspective(
    LNHandle camera, float fovY, float aspect, float nearClip, float farClip) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* camObj = instance->objectRegistry()->resolve<CameraObject>(camera);
    if (!camObj) return LN_ERROR_INVALID_HANDLE;

    camObj->camera.setPerspective(fovY, aspect, nearClip, farClip);
    return LN_OK;
}

LNResult LNCamera_SetLookAt(
    LNHandle camera,
    float eyeX, float eyeY, float eyeZ,
    float targetX, float targetY, float targetZ,
    float upX, float upY, float upZ) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* camObj = instance->objectRegistry()->resolve<CameraObject>(camera);
    if (!camObj) return LN_ERROR_INVALID_HANDLE;

    camObj->camera.setLookAt(
        ln::Vector3{eyeX, eyeY, eyeZ},
        ln::Vector3{targetX, targetY, targetZ},
        ln::Vector3{upX, upY, upZ});
    return LN_OK;
}

//------------------------------------------------------------------------------
// LNRenderer
//------------------------------------------------------------------------------

LNResult LNGraphicsContext_GetRenderer(LNHandle graphicsContext, LNHandle* outHandle) {
    if (!outHandle) return LN_ERROR_INVALID_ARGUMENT;
    *outHandle = LN_NULL_HANDLE;

    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ctx = instance->objectRegistry()->resolve<ln::GraphicsContext>(graphicsContext);
    if (!ctx) return LN_ERROR_INVALID_HANDLE;

    auto* renderer = ctx->renderer();
    if (!renderer) return LN_ERROR_UNKNOWN;

    // Renderer is owned by GraphicsContext; add a reference for the C handle.
    renderer->addRef();
    LNHandle handle = instance->objectRegistry()->registerObject(ln::Ref<ln::Object>::adopt(renderer));
    if (handle == LN_NULL_HANDLE) return LN_ERROR_UNKNOWN;

    *outHandle = handle;
    return LN_OK;
}

LNResult LNRenderer_BeginFrame(LNHandle renderer) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* r = instance->objectRegistry()->resolve<ln::Renderer>(renderer);
    if (!r) return LN_ERROR_INVALID_HANDLE;

    r->beginFrame();
    return LN_OK;
}

LNResult LNRenderer_EndFrame(LNHandle renderer) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* r = instance->objectRegistry()->resolve<ln::Renderer>(renderer);
    if (!r) return LN_ERROR_INVALID_HANDLE;

    r->endFrame();
    return LN_OK;
}

LNResult LNRenderer_BeginRenderPass(
    LNHandle renderer, LNHandle graphicsContext,
    LNHandle camera,
    float r, float g, float b, float a) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ren = instance->objectRegistry()->resolve<ln::Renderer>(renderer);
    if (!ren) return LN_ERROR_INVALID_HANDLE;

    auto* ctx = instance->objectRegistry()->resolve<ln::GraphicsContext>(graphicsContext);
    if (!ctx) return LN_ERROR_INVALID_HANDLE;

    if (camera != LN_NULL_HANDLE) {
        auto* camObj = instance->objectRegistry()->resolve<CameraObject>(camera);
        if (!camObj) return LN_ERROR_INVALID_HANDLE;
        ren->beginRenderPass(
            ctx->m_currentColorTarget,
            ctx->m_currentDepthTarget,
            camObj->camera,
            ln::Color{r, g, b, a});
    } else {
        ren->beginRenderPass(
            ctx->m_currentColorTarget,
            ctx->m_currentDepthTarget,
            ln::Color{r, g, b, a});
    }
    return LN_OK;
}

LNResult LNRenderer_EndRenderPass(LNHandle renderer) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* r = instance->objectRegistry()->resolve<ln::Renderer>(renderer);
    if (!r) return LN_ERROR_INVALID_HANDLE;

    r->endRenderPass();
    return LN_OK;
}

LNResult LNRenderer_DrawMesh(
    LNHandle renderer, LNHandle meshHandle, const LNTransform* transform) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ren = instance->objectRegistry()->resolve<ln::Renderer>(renderer);
    if (!ren) return LN_ERROR_INVALID_HANDLE;

    auto* mesh = instance->objectRegistry()->resolve<ln::Mesh>(meshHandle);
    if (!mesh) return LN_ERROR_INVALID_HANDLE;

    ln::Transform xform;
    if (transform) {
        xform.position = {transform->posX, transform->posY, transform->posZ};
        xform.rotation = ln::Quaternion{transform->rotX, transform->rotY, transform->rotZ, transform->rotW};
        xform.scale    = {transform->scaleX, transform->scaleY, transform->scaleZ};
    }

    auto result = ren->drawMesh(mesh, xform);
    if (!result) return LN_ERROR_UNKNOWN;

    return LN_OK;
}

LNResult LNRenderer_DrawMeshWithMaterial(
    LNHandle renderer, LNHandle meshHandle,
    const LNTransform* transform, LNHandle materialHandle) {
    auto* instance = ln::CoreInstance::instance();
    if (!instance) return LN_RUNTIME_UNINITIALIZED;

    auto* ren = instance->objectRegistry()->resolve<ln::Renderer>(renderer);
    if (!ren) return LN_ERROR_INVALID_HANDLE;

    auto* mesh = instance->objectRegistry()->resolve<ln::Mesh>(meshHandle);
    if (!mesh) return LN_ERROR_INVALID_HANDLE;

    auto* mat = instance->objectRegistry()->resolve<ln::Material>(materialHandle);
    if (!mat) return LN_ERROR_INVALID_HANDLE;

    ln::Transform xform;
    if (transform) {
        xform.position = {transform->posX, transform->posY, transform->posZ};
        xform.rotation = ln::Quaternion{transform->rotX, transform->rotY, transform->rotZ, transform->rotW};
        xform.scale    = {transform->scaleX, transform->scaleY, transform->scaleZ};
    }

    auto result = ren->drawMesh(mesh, xform, mat);
    if (!result) return LN_ERROR_UNKNOWN;

    return LN_OK;
}
