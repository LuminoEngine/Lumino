
#include "Internal.hpp"
#include <LuminoEngine/Rendering/RenderingContext.hpp>
#include "../CommandListServer.hpp"
#include "RILCulling.hpp"

namespace ln {
namespace detail {

RILCulling::RILCulling()
	: m_mainLight(nullptr)
	, m_visibleLights()
	, m_parts()
{
}

void RILCulling::cull(RenderingContext* renderingContext, CommandListServer* commandListServer)
{
	m_mainLight = nullptr;
	m_visibleLights.clear();

	for (int iPart = 0; iPart < (int)RenderPart::_Count; iPart++) {
		RenderPart renderPart = (RenderPart)iPart;
		Part* partInfo = &m_parts[iPart];
		partInfo->visibleElements.clear();

		// Lights
		for (const DynamicLightInfo& light : renderingContext->dynamicLightInfoList()) {
			if (light.mainLight) {
				m_mainLight = &light;
			}
			m_visibleLights.push_back(&light);
		}

		// Elements
//		commandListServer->enumerateDrawElements(
//			m_currentPart, m_currentProjection,
//			[this](RenderDrawElement* element) {
//
//#if 0		// TODO: 視錘台カリング
//			const Matrix& transform = element->getTransform(elementList);
//
//			Sphere boundingSphere = element->getLocalBoundingSphere();
//			boundingSphere.center += transform.getPosition();
//
//			if (boundingSphere.radius < 0 ||	// マイナス値なら視錐台と衝突判定しない
//				cameraInfo.viewFrustum.intersects(boundingSphere.center, boundingSphere.radius))
//			{
//				// このノードは描画できる
//				m_renderingElementList.add(element);
//			}
//#else
//			m_renderingElementList.add(element);
//#endif
//		});
	}


}

} // namespace detail
} // namespace ln

