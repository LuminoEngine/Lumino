#include "Internal.hpp"
#include <LuminoFramework/Scene/World.hpp>
#include <LuminoFramework/Scene/Scene.hpp>
#include <LuminoFramework/Scene/MeshTilemap/MeshTilemapModel.hpp>
#include <LuminoFramework/Scene/MeshTilemap/MeshTilemapComponent.hpp>

namespace ln {

//==============================================================================
// MeshTilemapComponent

LN_OBJECT_IMPLEMENT(MeshTilemapComponent, VisualComponent) {}

MeshTilemapComponent::MeshTilemapComponent()
{
}

void MeshTilemapComponent::init()
{
    VisualComponent::init();
}

MeshTilemapModel* MeshTilemapComponent::model() const
{
    return m_model;
}

void MeshTilemapComponent::setModel(MeshTilemapModel* value)
{
    m_model = value;
}

void MeshTilemapComponent::onRender(RenderingContext* context)
{
    if (m_model) {
        m_model->draw(context, Matrix::Identity);
    }
}

} // namespace ln

