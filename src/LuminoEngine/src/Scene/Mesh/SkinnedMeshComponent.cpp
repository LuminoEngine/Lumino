
#include "Internal.hpp"
#include <LuminoEngine/Graphics/Texture.hpp>
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Mesh/AnimationController.hpp>
#include <LuminoEngine/Mesh/SkinnedMeshModel.hpp>
#include <LuminoEngine/Scene/Mesh/SkinnedMeshComponent.hpp>
#include <LuminoEngine/Scene/WorldObject.hpp>
#include <LuminoEngine/Rendering/RenderView.hpp>
#include "../../Mesh/MeshModelInstance.hpp"

namespace ln {


//=============================================================================
// SkinnedMeshComponent

SkinnedMeshComponent::SkinnedMeshComponent()
    : m_model(nullptr)
{
}

SkinnedMeshComponent::~SkinnedMeshComponent()
{
}

bool SkinnedMeshComponent::init()
{
    return VisualComponent::init();
}

void SkinnedMeshComponent::setModel(SkinnedMeshModel* model)
{
    m_model = model;
	m_modelInstance = m_model->createMeshModelInstance();
}

SkinnedMeshModel* SkinnedMeshComponent::model() const
{
    return m_model;
}

void SkinnedMeshComponent::onUpdate(float elapsedSeconds)
{
    m_model->beginUpdate();

	// TODO: worldMatrix() を使うので、オブジェクトに追従する形にしたい。 OnLateUpdate へ
	m_model->setWorldTransform(worldObject()->worldMatrix());

	//m_meshModel->getAnimator()->advanceTime(elapsedTime);
	if (m_model->animationController()) {
		m_model->animationController()->advanceTime(elapsedSeconds);
	}
	//m_model->animationController()->updateTargetElements();
	//static bool init = false;
	//if (!init)
	//{
	//	m_meshModel->getAnimator()->advanceTime(808.11);
	//	init = true;
	//}
	m_model->preUpdate();
	m_modelInstance->updateSkinningMatrices();



	//for (int i = 0; i < m_meshModel->bones().getCount(); i++)
	//{
	//	SkinnedMeshBone* bone = m_meshModel->bones()[i];
	//	if (bone->name() == _T("左足首"))
	//	{
	//		AttitudeTransform t;
	//		t.scale = Vector3(1, 1, 0.2);
	//		bone->setLocalTransform(t);
	//	}
	//}


}

void SkinnedMeshComponent::onRender(RenderingContext* context)
{

	m_model->postUpdate();
//#if 1
//	LN_NOTIMPLEMENTED();
//#else
//    for (auto& meshContainer : m_model->meshContainers())
//    {
//        MeshResource* meshResource = meshContainer->meshResource();
//        for (int iSection = 0; iSection < meshResource->sections().size(); iSection++)
//        {
//            context->setMaterial(m_model->materials()[meshResource->sections()[iSection].materialIndex]);
//            context->drawMesh(meshResource, iSection);
//        }
//    }
//#endif
//

	for (const auto& node : m_model->meshNodes()) {
		if (node->meshContainerIndex() >= 0 && node->isVisible()) {
			context->setTransfrom(m_model->nodeGlobalTransform(node->index()));


			const auto& meshContainer = m_model->meshContainers()[node->meshContainerIndex()];

			Mesh* mesh = meshContainer->mesh();
			if (mesh) {
				for (int iSection = 0; iSection < mesh->sections().size(); iSection++) {
					context->setMaterial(m_model->materials()[mesh->sections()[iSection].materialIndex]);


					if (node->skeletonIndex >= 0) {
						context->drawSkinnedMesh(mesh, iSection, m_modelInstance->skeletons()[node->skeletonIndex]);
					}
					else {
						context->drawMesh(mesh, iSection);
					}
				}
			}

		}
	}
}

void SkinnedMeshComponent::onRenderGizmo(RenderingContext* context)
{
	context->pushState();
	context->setRenderPhase(RenderPhaseClass::Gizmo2D);
	context->setDepthTestEnabled(false);
	context->setDepthWriteEnabled(false);

	//const auto* viewPoint = context->viewPoint();
	//Matrix viewproj = viewPoint->viewProjMatrix;

	const auto* view = context->currentRenderView;
	for (const auto& skeleton : m_modelInstance->skeletons()) {
		for (const auto& bone : skeleton->bones()) {

            //context->setRenderPhase(RenderPhaseClass::Gizmo2D);

			//Vector4 trf = Vector4(bone->combinedTransform().position(), 1.0f);
			//trf = Vector4::transform(trf, viewproj);
			//float m_displayScale = 1.0;
			//float m_screenFactor = m_displayScale * 0.15f * trf.w;
			//auto transform = Matrix::multiply(Matrix::makeScaling(m_screenFactor), bone->combinedTransform());
			//context->setTransfrom(transform);
			////context->setTransfrom(bone->combinedTransform());
			//context->drawSphere(1, 8, 8, Color::Red);

			auto pos = view->transformProjection(bone->combinedTransform().position(), detail::ProjectionKind::ViewProjection3D, detail::ProjectionKind::Independent2D);
			context->setTransfrom(Matrix::makeTranslation(pos));
			context->drawRegularPolygonPrimitive(16, 20, Color::Red, true);
		}
	}

	//context->setTransfrom(Matrix::makeTranslation(0,0,0));
	//context->drawBox(Box(Vector3(1, 1, 0.5), 100, 100, 0.01), Color::Red);

	//context->setTransfrom(Matrix::makeTranslation(200, 200, 0));
	//context->drawText(u"TEST");


	//context->setTransfrom(Matrix::makeTranslation(300, 200, 0));
	//context->drawRegularPolygonPrimitive(16, 20, Color::Blue, true);
	//context->drawRegularPolygonPrimitive(16, 25, Color::Blue, false);

	context->popState();
}

} // namespace ln

