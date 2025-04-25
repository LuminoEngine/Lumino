#include "Internal.hpp"
#include <LuminoCore/Base/LinearAllocator.hpp>
#include <LuminoEngine/Rendering/RenderingManager.hpp>
#include <LuminoEngine/Rendering/RenderViewPoint.hpp>
#include <LuminoEngine/Rendering/SceneRenderer.hpp>

namespace ln {

SceneRenderer::SceneRenderer(detail::RenderingManager* manager)
    : m_manager(manager)
    , m_dataAllocator(makeRef<detail::LinearAllocator>(manager->stageDataPageManager()))
    , m_headDrawElement(nullptr)
    , m_tailDrawElement(nullptr)
    , m_visibleDrawElementCount(0)
    , m_currentViewPoint(nullptr)
    , m_zSortDistanceBase(detail::ZSortDistanceBase::CameraScreenDistance) {
}

void SceneRenderer::dispose() {
    destructDrawElementList();
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

void SceneRenderer::render() {
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
    for (int i = 0; i < m_visibleDrawElementCount; i++) {
        DrawElement* element = visibleElements[i];
        element->onRender();
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

void SceneRenderer::drawSprite(const SpriteData& data) {
    struct SpriteDrawElement final : public DrawElement {
        SpriteData data;
        SpriteDrawElement()
            : DrawElement(DrawElementType::Sprite) {}
        const Matrix& worldMatrix() override { return data.transform; }
        void onRender() override {}
    };
    auto* instance = newDrawElement<SpriteDrawElement>();
    instance->data = data;
    setupElement(instance);
}

} // namespace ln

