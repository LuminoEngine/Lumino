
#include "Internal.hpp"
#include <LuminoGraphics/Rendering/Material.hpp>
#include <LuminoGraphics/Mesh/MeshPrimitive.hpp>
#include <LuminoEngine/Scene/Shapes/PathShape.hpp>
#include "../../Graphics/src/Rendering/RenderFeature/NanoVGHelper.hpp"
#include "../SceneManager.hpp"
#include "PathShapeContext.hpp"

namespace ln {

//==============================================================================
// PathShapeComponent

LN_OBJECT_IMPLEMENT(PathShapeComponent, VisualComponent) {}

PathShapeComponent::PathShapeComponent()
    : m_material(nullptr)
    , m_mesh(nullptr)
    , m_pathContext(nullptr)
{
}

bool PathShapeComponent::init()
{
    if (!VisualComponent::init()) return false;

    m_material = makeObject_deprecated<Material>();

    //m_mesh = makeObject_deprecated<MeshPrimitive>(4, 6);
    //m_mesh->setVertex(0, Vertex(Vector3(-1, 1, 0), Vector3::UnitZ, Vector2(0, 0), Color::Red));
    //m_mesh->setVertex(1, Vertex(Vector3(-1, -1, 0), Vector3::UnitZ, Vector2(0, 0), Color::Red));
    //m_mesh->setVertex(2, Vertex(Vector3(1, 1, 0), Vector3::UnitZ, Vector2(0, 0), Color::Red));
    //m_mesh->setVertex(3, Vertex(Vector3(1, -1, 0), Vector3::UnitZ, Vector2(0, 0), Color::Red));
    //    
    //m_mesh->setIndex(0, 0);
    //m_mesh->setIndex(1, 1);
    //m_mesh->setIndex(2, 2);
    //m_mesh->setIndex(3, 2);
    //m_mesh->setIndex(4, 1);
    //m_mesh->setIndex(5, 3);

    //m_mesh->addSection(0, 2, 0, PrimitiveTopology::TriangleList);



    //detail::PathShape_NVGcontext* m_glnvgContext;
    //NVGcontext* m_nvgContext = detail::PathShape_NVGcontext::nvgCreate(0/*NVG_ANTIALIAS*/, &m_glnvgContext);

    //nvgBeginPath(m_nvgContext);
    ////nvgRect(m_nvgContext, 0, 0, 20, 10);
    //nvgMoveTo(m_nvgContext, 1, 2);
    //nvgLineTo(m_nvgContext, 1, 5);
    //nvgLineTo(m_nvgContext, 3, 3);
    //nvgFillColor(m_nvgContext, nvgRGBA(255, 192, 0, 255));
    //nvgFill(m_nvgContext);

    ////nvgStrokeColor(m_nvgContext, nvgRGBA(255, 255, 255, 255));
    ////nvgStrokeWidth(m_nvgContext, 1);
    ////nvgStroke(m_nvgContext);

    //nvgEndFrame(m_nvgContext);
    //

    //m_mesh = m_glnvgContext->m_mesh;
    //detail::PathShape_NVGcontext::nvgDelete(m_nvgContext);



    return true;
}

void PathShapeComponent::beginCanvas()
{
    float minWidth = 0.001f;
    m_pathContext = detail::EngineDomain::sceneManager()->pathContext().get();
    m_pathContext->m_space = PathPlacementSpace::XY;
    m_pathContext->m_currentStrokeColor = Color::Black;
    m_pathContext->m_currentStrokeWidth = 1.0f;
    nvgBeginFrame(m_pathContext->m_nvgContext, 100.0f, 100.0f, 1.0f / minWidth);
    nvgReset(m_pathContext->m_nvgContext);
}

void PathShapeComponent::endCanvas(PathPlacementSpace space)
{
    m_pathContext->m_space = space;
    nvgEndFrame(m_pathContext->m_nvgContext);
    m_mesh = m_pathContext->m_mesh;
    m_pathContext = nullptr;
}

void PathShapeComponent::beginPath()
{
    nvgBeginPath(m_pathContext->m_nvgContext);
}

void PathShapeComponent::moveTo(float x, float y)
{
    nvgMoveTo(m_pathContext->m_nvgContext, x, y);
}

void PathShapeComponent::lineTo(float x, float y)
{
    nvgLineTo(m_pathContext->m_nvgContext, x, y);
}

void PathShapeComponent::fill()
{
    nvgFill(m_pathContext->m_nvgContext);
}

void PathShapeComponent::stroke()
{
    nvgStroke(m_pathContext->m_nvgContext);
}

void PathShapeComponent::closePath()
{
    nvgClosePath(m_pathContext->m_nvgContext);
}

void PathShapeComponent::setFillColor(const Color& value)
{
    nvgFillColor(m_pathContext->m_nvgContext, nvgRGBAf(value.r, value.g, value.b, value.a));
}

void PathShapeComponent::setStrokeColor(const Color& value)
{
    nvgStrokeColor(m_pathContext->m_nvgContext, nvgRGBAf(value.r, value.g, value.b, value.a));
    m_pathContext->m_currentStrokeColor = value;
}

void PathShapeComponent::setStrokeWidth(float value)
{
    nvgStrokeWidth(m_pathContext->m_nvgContext, value);
    m_pathContext->m_currentStrokeWidth = value;
    //nvgGlobalAlpha(m_pathContext->m_nvgContext, (1.0f / value) * (1.0f / value));
}

void PathShapeComponent::onRender(RenderingContext* context)
{
    if (!m_mesh) return;

    context->setMaterial(m_material);

    const int sectionCount = m_mesh->sections().size();
    for (int i = 0; i < sectionCount; i++) {
        context->drawMesh(m_mesh, i);
    }
}

//==============================================================================
// PathShape

LN_OBJECT_IMPLEMENT(PathShape, VisualObject) {}

PathShape::PathShape()
{
}

bool PathShape::init()
{
    if (!VisualObject::init()) return false;
    m_component = makeObject_deprecated<PathShapeComponent>();
    addComponent(m_component);
    setMainVisualComponent(m_component);
    return true;
}

} // namespace ln

