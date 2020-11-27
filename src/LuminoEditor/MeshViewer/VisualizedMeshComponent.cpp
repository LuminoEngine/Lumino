
#include "VisualizedMeshComponent.hpp"

void VisualizedMeshComponent::onRenderGizmo(ln::RenderingContext* context)
{

	ln::CommandList* commandList = context->getCommandList(ln::RenderPart::Gizmo2D, ln::detail::ProjectionKind::Independent2D);
	commandList->pushState();
	//commandList->setRenderPhase(RenderPart::Gizmo2D);
	commandList->setDepthTestEnabled(false);
	commandList->setDepthWriteEnabled(false);


	const auto* view = context->currentRenderView;
	for (const auto& node : model()->m_nodes) {

		auto pos = view->transformProjection(node->globalMatrix().position(), ln::detail::ProjectionKind::ViewProjection3D, ln::detail::ProjectionKind::Independent2D);
		commandList->setTransfrom(ln::Matrix::makeTranslation(pos));
		commandList->drawRegularPolygonPrimitive(16, 8, ln::UIColors::blue(), true);
		commandList->drawRegularPolygonPrimitive(16, 10, ln::UIColors::blue(), false);
	}

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

