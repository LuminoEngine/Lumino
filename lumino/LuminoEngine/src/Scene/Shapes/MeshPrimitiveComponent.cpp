
#include "Internal.hpp"
#include <LuminoEngine/Base/Serializer.hpp>
#include <LuminoGraphics/Rendering/Material.hpp>
#include <LuminoGraphics/Rendering/FeatureRenderer/PrimitiveMeshRenderer.hpp>
#include <LuminoGraphics/Mesh/MeshModel.hpp>
#include <LuminoGraphics/Mesh/MeshModelFactory.hpp>
#include <LuminoEngine/Scene/Shapes/MeshPrimitiveComponent.hpp>
//#include "../SceneManager.hpp"
#include <LuminoGraphics/Rendering/detail/RenderingManager.hpp>

/*
 * Geometry 設計メモ
 * ----------
 * 
 *
 */

namespace ln {

//==============================================================================
// ShapeComponent

LN_OBJECT_IMPLEMENT(ShapeComponent, VisualComponent) {
}

ShapeComponent::ShapeComponent()
    : m_material() {
}

bool ShapeComponent::init() {
    if (!VisualComponent::init()) return false;
    m_material = detail::RenderingManager::instance()->primitiveMeshDefaultMaterial();
    return true;
}

void ShapeComponent::setMaterial(Material* material) {
    m_material = material;
}

Material* ShapeComponent::material() const {
    return m_material;
}

void ShapeComponent::serialize_deprecated(Serializer2_deprecated& ar) {
    VisualComponent::serialize_deprecated(ar);
    ar& makeNVP(_TT("material"), m_material);
}

//==============================================================================
// PlaneMeshComponent

LN_OBJECT_IMPLEMENT(PlaneMeshComponent, VisualComponent) {
}

PlaneMeshComponent::PlaneMeshComponent()
    : m_size(10.0f, 10.0f)
    , m_uvParUnit(0.0f, 0.0f) {
}

PlaneMeshComponent::~PlaneMeshComponent() {
}

void PlaneMeshComponent::init() {
    VisualComponent::init();
}

void PlaneMeshComponent::serialize_deprecated(Serializer2_deprecated& ar) {
    ShapeComponent::serialize_deprecated(ar);
    ar& makeNVP(_TT("size"), m_size);
    ar& makeNVP(_TT("uvParUnit"), m_uvParUnit);
}

void PlaneMeshComponent::onRender(RenderingContext* context) {
    auto uv1 = Vector2(
        Math::nearEqual(m_uvParUnit.x, 0.0f) ? 1.0f : m_size.x / m_uvParUnit.x,
        Math::nearEqual(m_uvParUnit.y, 0.0f) ? 1.0f : m_size.y / m_uvParUnit.y);
    PrimitiveMeshRenderer::drawPlane(context, material(), m_size.x, m_size.y, Vector2::Zero, uv1, Color::White);
}

//==============================================================================
// SphereMeshComponent

SphereMeshComponent::SphereMeshComponent() {
}

SphereMeshComponent::~SphereMeshComponent() {
}

bool SphereMeshComponent::init() {
    return ShapeComponent::init();
}

void SphereMeshComponent::onRender(RenderingContext* context) {
    PrimitiveMeshRenderer::drawSphere(context, material(), 0.5, 16, 16, Color::White);
}

////==============================================================================
//// BoxMeshComponent
//
//BoxMeshComponent::BoxMeshComponent()
//{
//}
//
//BoxMeshComponent::~BoxMeshComponent()
//{
//}
//
//bool BoxMeshComponent::init()
//{
//    return ShapeComponent::init();
//}
//
//bool BoxMeshComponent::init(const Vector3& size)
//{
//    if (!init()) return false;
//	//m_box = Box(Vector3::Zero, size);
//
//    m_model = detail::MeshModelFactory::createBox(size, detail::EngineDomain::sceneManager()->primitiveMeshDefaultMaterial());
//    setMaterial(makeObject_deprecated<Material>());
//    return true;
//}
//
//void BoxMeshComponent::setSize(const Vector3& size)
//{
//    m_model = detail::MeshModelFactory::createBox(size);
//}
//
//void BoxMeshComponent::onRender(RenderingContext* context)
//{
//    context->setMaterial(material());
//	//context->drawBox(Box(Vector3::Zero, 1, 1, 1), Color::White);
//    //context->setTransfrom(m_model->nodeGlobalTransform(m_model->m_nodes[0]->index()));
//    context->drawMesh(m_model->meshContainers()[0]->meshPrimitive(), 0);
//}

} // namespace ln
