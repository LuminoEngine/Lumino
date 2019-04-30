
#include "Internal.hpp"
#include <LuminoEngine/Scene/SceneConductor.hpp>
#include "SceneManager.hpp"

namespace ln {

//==============================================================================
// SceneConductor

void SceneConductor::gotoScene(Scene* scene)
{
    detail::EngineDomain::sceneManager()->gotoScene(scene);
}

void SceneConductor::callScene(Scene* scene)
{
    detail::EngineDomain::sceneManager()->callScene(scene);
}

void SceneConductor::returnScene()
{
    detail::EngineDomain::sceneManager()->returnScene();
}

Scene* SceneConductor::activeScene()
{
    return detail::EngineDomain::sceneManager()->activeScene();
}

} // namespace ln
