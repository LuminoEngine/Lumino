
#include "Internal.hpp"
#include <LuminoEngine/Tilemap/Voxel.hpp>

namespace ln {

//==============================================================================
// VoxelmapModel

LN_OBJECT_IMPLEMENT(VoxelmapModel, Object)
{
    context->registerType<VoxelmapModel>({});
}

void VoxelmapModel::serialize(Archive& ar)
{
    Object::serialize(ar);
}

VoxelmapModel::VoxelmapModel()
{
}

void VoxelmapModel::init()
{
    Object::init();
    resizeMap(16, 16, 16);
}

void VoxelmapModel::resizeMap(int width, int height, int depth)
{
    m_width = width;
    m_height = height;
    m_depth = depth;
    m_mapData.resize(m_width * m_height * m_depth);
}

} // namespace ln




namespace ln {

//==============================================================================
// VoxelmapComponent

LN_OBJECT_IMPLEMENT(VoxelmapComponent, VisualComponent)
{
    context->registerType<VoxelmapComponent>({});
}

VoxelmapComponent::VoxelmapComponent()
{
}

void VoxelmapComponent::init()
{
    VisualComponent::init();
    m_model = makeObject<VoxelmapModel>();
}

void VoxelmapComponent::onRender(RenderingContext* context)
{
}

void VoxelmapComponent::serialize(Archive& ar)
{
    VisualComponent::serialize(ar);
}

} // namespace ln
