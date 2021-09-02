
#include "VisualizedMeshComponent.hpp"

void VisualizedMeshComponent::onRenderGizmo(ln::RenderingContext* context)
{
	return;

	ln::CommandList* commandList = context->getCommandList(ln::RenderPart::Gizmo2D);
	commandList->pushState();
	//commandList->setRenderPhase(RenderPart::Gizmo2D);
	//commandList->setDepthTestEnabled(false);
	//commandList->setDepthWriteEnabled(false);

	commandList->drawPath(nullptr);


	const auto* view = context->currentRenderView;
	const auto& nodes = model()->m_nodes;

	if (!model()->skeletons().isEmpty()) {
		const auto& skeleton = model()->skeleton(0);
		for (const auto& bone : skeleton->m_bones) {
			const auto& node = bone->node();

			const ln::Color c = ln::UIColors::blue();

			auto pos = view->transformProjection(node->globalMatrix().position(), ln::RenderPart::Geometry, ln::RenderPart::Gizmo2D);
			commandList->setTransfrom(ln::Matrix::makeTranslation(pos));
			commandList->drawRegularPolygonPrimitive(16, 8, c, true, ln::Matrix::makeTranslation(0, 0, 0));
			commandList->drawRegularPolygonPrimitive(16, 10, c, false);


			if (node->parentNodeIndex() >= 0) {
				const auto& parentNode = nodes[node->parentNodeIndex()];
				if (parentNode->m_boneNode) {
					const ln::Vector3& parentWorldPos = parentNode->globalMatrix().position();
					const ln::Vector3 parentPos = view->transformProjection(parentWorldPos, ln::RenderPart::Geometry, ln::RenderPart::Gizmo2D);
					if ((pos - parentPos).length() >= 1.0f) {
						const ln::Vector3 right = ln::Vector3::cross(
							ln::Vector3::normalize(pos - parentPos),
							ln::Vector3::UnitZ);

						ln::Vector3 points[3] = {
							parentPos + (right * 10),
							pos,
							parentPos - (right * 10) };
						ln::Color colors[3] = { ln::UIColors::blue(), ln::UIColors::blue(), ln::UIColors::blue() };
						commandList->setTransfrom(ln::Matrix::Identity);
						commandList->drawLineStripPrimitive(3, points, colors);
					}
				}
			}

			if (node->children().isEmpty()) {
				const auto wpos = ln::Vector3::transformCoord(ln::Vector3::UnitZ, node->globalMatrix());
				const auto vpos = view->transformProjection(wpos, ln::RenderPart::Geometry, ln::RenderPart::Gizmo2D);
				commandList->setTransfrom(ln::Matrix::makeTranslation(vpos));
				commandList->drawRegularPolygonPrimitive(16, 10, c, false);
			}

		}
	}


	if (0) {

		for (const auto& node : nodes) {
			const float z = node->index() == 0 ? 100 : 0;
			const ln::Color c = node->index() == 0 ? ln::UIColors::red() : ln::UIColors::blue();

			auto pos = view->transformProjection(node->globalMatrix().position(), ln::RenderPart::Geometry, ln::RenderPart::Gizmo2D);
			//pos.z = z;
			commandList->setTransfrom(ln::Matrix::makeTranslation(pos));
			commandList->drawRegularPolygonPrimitive(16, 8, c, true, ln::Matrix::makeTranslation(0, 0, z));
			commandList->drawRegularPolygonPrimitive(16, 10, c, false);




			if (node->parentNodeIndex() >= 0) {
				const ln::Vector3& parentWorldPos = nodes[node->parentNodeIndex()]->globalMatrix().position();
				const ln::Vector3 parentPos = view->transformProjection(parentWorldPos, ln::RenderPart::Geometry, ln::RenderPart::Gizmo2D);
				if ((pos - parentPos).length() >= 1.0f) {
					const ln::Vector3 right = ln::Vector3::cross(
						ln::Vector3::normalize(pos - parentPos),
						ln::Vector3::UnitZ);

					ln::Vector3 points[3] = {
						parentPos + (right * 10),
						pos,
						parentPos - (right * 10) };
					ln::Color colors[3] = { ln::UIColors::blue(), ln::UIColors::blue(), ln::UIColors::blue() };
					commandList->setTransfrom(ln::Matrix::Identity);
					commandList->drawLineStripPrimitive(3, points, colors);
				}
			}

		}
	}

	//context->beginPath();
	//context->endPath();

#if 0
	//const auto* viewPoint = context->viewPoint();
	//Matrix viewproj = viewPoint->viewProjMatrix;

	const auto* view = context->currentRenderView;
	for (const auto& skeleton : m_modelInstance->skeletons()) {
		for (const auto& bone : skeleton->bones()) {

			//context->setRenderPhase(RenderPart::Gizmo2D);

			//Vector4 trf = Vector4(bone->combinedTransform().position(), 1.0f);
			//trf = Vector4::transform(trf, viewproj);
			//float m_displayScale = 1.0;
			//float m_screenFactor = m_displayScale * 0.15f * trf.w;
			//auto transform = Matrix::multiply(Matrix::makeScaling(m_screenFactor), bone->combinedTransform());
			//context->setTransfrom(transform);
			////context->setTransfrom(bone->combinedTransform());
			//context->drawSphere(1, 8, 8, Color::Red);

			auto pos = view->transformProjection(bone->combinedTransform().position(), detail::ProjectionKind::ViewProjection3D, detail::ProjectionKind::Independent2D);
			commandList->setTransfrom(Matrix::makeTranslation(pos));
			commandList->drawRegularPolygonPrimitive(16, 8, UIColors::blue(), true);
			commandList->drawRegularPolygonPrimitive(16, 10, UIColors::blue(), false);
		}
	}

	//context->setTransfrom(Matrix::makeTranslation(0,0,0));
	//context->drawBox(Box(Vector3(1, 1, 0.5), 100, 100, 0.01), Color::Red);

	//context->setTransfrom(Matrix::makeTranslation(200, 200, 0));
	//context->drawText(u"TEST");


	//context->setTransfrom(Matrix::makeTranslation(300, 200, 0));
	//context->drawRegularPolygonPrimitive(16, 20, Color::Blue, true);
	//context->drawRegularPolygonPrimitive(16, 25, Color::Blue, false);
#endif

	commandList->popState();
}

