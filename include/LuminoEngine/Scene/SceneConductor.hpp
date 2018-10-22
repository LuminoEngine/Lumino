
#pragma once
#include "Common.hpp"

namespace ln {
class Scene;
namespace detail { class SceneManager; }

/** Scene の実行状態を管理します。 */
class SceneConductor
{
public:
	static void gotoScene(Scene* scene);
	static void callScene(Scene* scene);
	static void returnScene();
	static Scene* activeScene();
};

} // namespace ln
