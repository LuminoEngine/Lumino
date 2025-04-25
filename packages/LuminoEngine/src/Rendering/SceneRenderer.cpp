#include "Internal.hpp"
#include <LuminoCore/Base/LinearAllocator.hpp>
#include <LuminoEngine/Rendering/FeatureRenderer/RendererServer.hpp>
#include <LuminoEngine/Rendering/FeatureRenderer/BatchRenderer.hpp>
#include <LuminoEngine/Rendering/RenderingManager.hpp>
#include <LuminoEngine/Rendering/RenderViewPoint.hpp>
#include <LuminoEngine/Rendering/SceneRenderer.hpp>

namespace ln {

Ref<SceneRenderer> SceneRenderer::create(detail::RenderingManager* manager) {
    Ref<SceneRenderer> instance(LN_NEW SceneRenderer(manager), false);
    instance->init();
    return instance;
}

SceneRenderer::SceneRenderer(detail::RenderingManager* manager)
    : m_manager(manager)
    , m_dataAllocator(makeRef<detail::LinearAllocator>(manager->stageDataPageManager()))
    , m_rendererServer(nullptr)
    , m_headDrawElement(nullptr)
    , m_tailDrawElement(nullptr)
    , m_visibleDrawElementCount(0)
    , m_currentViewPoint(nullptr)
    , m_zSortDistanceBase(detail::ZSortDistanceBase::CameraScreenDistance) {
    m_rendererServer = RendererServer::create();
}

void SceneRenderer::dispose() {
    destructDrawElementList();
}

RendererServer* SceneRenderer::rendererServer() const {
    return m_rendererServer;
}

void SceneRenderer::destructDrawElementList() {
    DrawElement* p = m_headDrawElement;
    while (p) {
        p->~DrawElement();
        p = p->next;
    }
    m_headDrawElement = nullptr;
    m_tailDrawElement = nullptr;
    m_visibleDrawElementCount = 0;
}

void SceneRenderer::addDrawElement(DrawElement* instance) {
    if (m_tailDrawElement) {
        m_tailDrawElement->next = instance;
    }
    else {
        m_headDrawElement = instance;
    }
    m_tailDrawElement = instance;
}

void SceneRenderer::reset(const RenderViewPoint* currentViewPoint) {
    m_currentViewPoint = currentViewPoint;
    destructDrawElementList();
    m_dataAllocator->cleanup();
}

void SceneRenderer::render(CommandList* commandList) {
    // 可視 DrawElement を配列に集める
    auto** visibleElements = static_cast<DrawElement**>(
        m_dataAllocator->allocate(sizeof(DrawElement*) * m_visibleDrawElementCount));
    {
        int i = 0;
        DrawElement* element = m_headDrawElement;
        while (element) {
            visibleElements[i] = element;
            i++;
            element = element->next;
        }
    }
    
    // ZSort
    //   TODO: Unity に似せるなら、この処理は SceneRenderer の中でパラメータに応じて行うべき。
    //   パラメータは次のようなものがある。
    //   https://docs.unity3d.com/ja/2018.4/ScriptReference/Experimental.Rendering.SortFlags.html
    //   ただ前から奥等典型的なソートを何回も行うのは無駄なので、キャッシュしたいところ。
    {
        // 距離は降順。遠いほうを先に描画する
        // 優先度は昇順。高いほうを手前に描画する (UE4 ESceneDepthPriorityGroup)
        // フェンスID は昇順。高いほうを後に描画する
        std::stable_sort(
            visibleElements,
            visibleElements + m_visibleDrawElementCount,
            [](const DrawElement* lhs, const DrawElement* rhs) {
                return lhs->zDistance > rhs->zDistance;
                //if (lhs->commandFence == rhs->commandFence) {
                //    if (lhs->actualPriority() == rhs->actualPriority()) return lhs->zDistance > rhs->zDistance;
                //    return lhs->actualPriority() < rhs->actualPriority();
                //}
                //else {
                //    return lhs->commandFence < rhs->commandFence;
                //}
            });
    }

    // Render
    {
        m_rendererServer->reset();
        for (int i = 0; i < m_visibleDrawElementCount; i++) {
            DrawElement* element = visibleElements[i];
            element->onRender(m_rendererServer, commandList);
        }
        m_rendererServer->flush();
    }
}

void SceneRenderer::setupElement(DrawElement* instance) {
    // TODO: View Culling
    // instance.visible = ...;
    m_visibleDrawElementCount++;

    // calculate distance for ZSort
    auto& position = instance->worldMatrix().position();
    switch (m_zSortDistanceBase) {
        case detail::ZSortDistanceBase::NodeZ:
            instance->zDistance = position.z;
            break;
        case detail::ZSortDistanceBase::CameraDistance:
            instance->zDistance = (position - m_currentViewPoint->viewPosition).lengthSquared();
            break;
        case detail::ZSortDistanceBase::CameraScreenDistance:
            instance->zDistance = Vector3::dot(
                position - m_currentViewPoint->viewPosition,
                m_currentViewPoint->viewDirection); // 平面と点の距離
                                                    // TODO: ↑第2引数違くない？要確認
            break;
        default:
            LN_UNREACHABLE();
            break;
    }
}

void SceneRenderer::drawSprite(Material* material, const SpriteData& data) {
    struct SpriteDrawElement final : public DrawElement {
        SpriteData data;
        Material* material;
        SpriteDrawElement()
            : DrawElement(DrawElementType::Sprite) {}
        const Matrix& worldMatrix() override { return data.transform; }
        void onRender(RendererServer* rendererServer, CommandList* commandList) override {
            auto* renderer = rendererServer->spriteRenderer();
            rendererServer->activate(renderer, commandList, material);
            renderer->drawSprite(data);
        }
    };
    auto* instance = newDrawElement<SpriteDrawElement>();
    instance->data = data;
    instance->material = material;
    setupElement(instance);
}

} // namespace ln

