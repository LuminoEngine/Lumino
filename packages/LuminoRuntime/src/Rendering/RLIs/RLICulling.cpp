
#include "Internal.hpp"
#include <LuminoEngine/Rendering/RenderView.hpp>
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchList.hpp>
#include <LuminoEngine/Rendering/Kanata/KBatchProxyCollector.hpp>
#include "../CommandListServer.hpp"
#include "RLICulling.hpp"

namespace ln {
namespace detail {

RLICulling::RLICulling()
    : m_mainLight(nullptr)
    , m_visibleLights()
    , m_parts()
    , m_zSortDistanceBase(ZSortDistanceBase::CameraScreenDistance) {
}

void RLICulling::cull(
    const RenderView* renderView,
    RenderingContext* renderingContext,
    CommandListServer* commandListServer) {
    m_mainLight = nullptr;
    m_visibleLights.clear();

    for (int iPart = 0; iPart < (int)RenderPart::_Count; iPart++) {
        RenderPart renderPart = (RenderPart)iPart;
        Part* partInfo = &m_parts[iPart];
        partInfo->visibleElements.clear();

        // Proxy list -> Batch list
        {
            auto* commandList = renderingContext->getCommandList(renderPart);
            auto& batchProxyCollector = commandList->batchProxyCollector();
            auto& batchCollector = commandList->batchCollector();

            batchProxyCollector->resolveSingleFrameBatchProxies(batchCollector);
            partInfo->batchCollector = batchCollector;
        }

        const RenderViewPoint* view = renderView->viewProjection(renderPart);

        // Lights
        for (const DynamicLightInfo& light : renderingContext->dynamicLightInfoList()) {
            if (light.mainLight) {
                m_mainLight = &light;
            }
            m_visibleLights.push_back(&light);
        }

        // Elements
        commandListServer->enumerateDrawElements(
            renderPart, [&](RenderDrawElement* element) {

#if 0 // TODO: 視錘台カリング
			const Matrix& transform = element->getTransform(elementList);

			Sphere boundingSphere = element->getLocalBoundingSphere();
			boundingSphere.center += transform.getPosition();

			if (boundingSphere.radius < 0 ||	// マイナス値なら視錐台と衝突判定しない
				cameraInfo.viewFrustum.intersects(boundingSphere.center, boundingSphere.radius))
			{
				// このノードは描画できる
				m_renderingElementList.add(element);
			}
#else
			partInfo->visibleElements.push_back(element);
#endif
                {
                    element->calculateActualPriority();

                    auto& position = element->combinedWorldMatrix().position();

                    // calculate distance for ZSort
                    switch (m_zSortDistanceBase) {
                        case ZSortDistanceBase::NodeZ:
                            element->zDistance = position.z;
                            break;
                        case ZSortDistanceBase::CameraDistance:
                            element->zDistance = (position - view->viewPosition).lengthSquared();
                            break;
                        case ZSortDistanceBase::CameraScreenDistance:
                            element->zDistance = Vector3::dot(
                                position - view->viewPosition,
                                view->viewDirection);   // 平面と点の距離
                                                        // TODO: ↑第2引数違くない？要確認
                            break;
                        default:
                            LN_UNREACHABLE();
                            break;
                    }
                }
            });

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
                partInfo->visibleElements.begin(), partInfo->visibleElements.end(), [](const RenderDrawElement* lhs, const RenderDrawElement* rhs) {
                    if (lhs->commandFence == rhs->commandFence) {
                        if (lhs->actualPriority() == rhs->actualPriority())
                            return lhs->zDistance > rhs->zDistance;
                        return lhs->actualPriority() < rhs->actualPriority();
                    }
                    else {
                        return lhs->commandFence < rhs->commandFence;
                    }
                });
        }
    }

    
}

} // namespace detail
} // namespace ln
