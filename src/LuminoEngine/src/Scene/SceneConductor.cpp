
#include "Internal.hpp"
#include <LuminoEngine/Scene/SceneConductor.hpp>
#include "SceneManager.hpp"

namespace ln {

//==============================================================================
// SceneConductor

Scene* SceneConductor::loadScene(const StringRef& sceneAssetFilePath)
{
    return detail::EngineDomain::sceneManager()->loadScene(sceneAssetFilePath);
}

void SceneConductor::unloadScene(Scene* scene)
{
    detail::EngineDomain::sceneManager()->unloadScene(scene);
}

#if 0
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
#endif

} // namespace ln
