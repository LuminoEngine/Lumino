
#include "Internal.hpp"
#include <LuminoEngine/Scene/World.hpp>
#include <LuminoEngine/Scene/Scene.hpp>
#include <LuminoEngine/Scene/MeshVoxelmap/MeshVoxelmapModel.hpp>
#include <LuminoEngine/Scene/MeshVoxelmap/MeshVoxelmapComponent.hpp>

namespace ln {

//==============================================================================
// MeshVoxelmapComponent

LN_OBJECT_IMPLEMENT(MeshVoxelmapComponent, VisualComponent) {}

MeshVoxelmapComponent::MeshVoxelmapComponent()
{
}

void MeshVoxelmapComponent::init()
{
    VisualComponent::init();
}

MeshVoxelmapModel* MeshVoxelmapComponent::model() const
{
    return m_model;
}

void MeshVoxelmapComponent::setModel(MeshVoxelmapModel* value)
{
    m_model = value;
}

void MeshVoxelmapComponent::onRender(RenderingContext* context)
{
    if (m_model) {
        m_model->draw(context, Matrix::Identity);
    }
}

} // namespace ln

