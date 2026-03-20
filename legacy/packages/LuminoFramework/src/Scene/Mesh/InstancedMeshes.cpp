
#include "Internal.hpp"
#include <LuminoEngine/Rendering/Material.hpp>
#include <LuminoEngine/Rendering/InstancedMeshesModel.hpp>
#include <LuminoFramework/Scene/Mesh/InstancedMeshes.hpp>
#include <LuminoEngine/Rendering/detail/RenderingManager.hpp>

namespace ln {

InstancedMeshesComponent::InstancedMeshesComponent()
{}

InstancedMeshesComponent::~InstancedMeshesComponent()
{}

bool InstancedMeshesComponent::init()
{
    if (!VisualComponent::init()) return false;
    m_material = makeObject_deprecated<Material>();
    m_material->setShader(detail::RenderingManager::instance()->builtinShader(detail::BuiltinShader::Sprite));
    m_material->setShadingModel(ShadingModel::Unlit);
    return true;
}

void InstancedMeshesComponent::setModel(InstancedMeshList* value)
{
    m_model = value;
}

InstancedMeshList* InstancedMeshesComponent::model() const
{
    return m_model;
}

void InstancedMeshesComponent::onRender(RenderingContext* context)
{
    if (m_model) {
        context->drawMeshInstanced(m_material, m_model);
    }
}

//==============================================================================
// InstancedMeshes

InstancedMeshes::InstancedMeshes()
{
}

InstancedMeshes::~InstancedMeshes()
{
}

bool InstancedMeshes::init()
{
    if (!VisualObject::init()) return false;
    m_component = makeObject_deprecated<InstancedMeshesComponent>();
    addComponent(m_component);
    setMainVisualComponent(m_component);
    return true;
}

InstancedMeshesComponent* InstancedMeshes::instancedMeshesComponent() const
{
    return m_component;
}

} // namespace ln

