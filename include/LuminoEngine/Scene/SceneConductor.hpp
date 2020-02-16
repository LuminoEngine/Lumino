
#pragma once
#include "Common.hpp"

namespace ln {
class Scene;

/** Scene の実行状態を管理します。 */
class SceneConductor
{
public:
    /** Scene を読み込み、アクティブな World にアタッチします。 */
    static Scene* loadScene(const StringRef& sceneAssetFilePath);

    static void unloadScene(Scene* scene);

#if 0
	static void gotoScene(Scene* scene);
	static void callScene(Scene* scene);
	static void returnScene();
	static Scene* activeScene();
#endif
};

} // namespace ln

