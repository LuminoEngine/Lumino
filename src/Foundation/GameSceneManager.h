
#pragma once
#include <Lumino/Foundation/GameScene.h>

LN_NAMESPACE_BEGIN


class GameSceneManager
{
public:
	GameSceneManager();
	~GameSceneManager();

	void GotoScene(GameScene* scene);
	void PushScene(GameScene* scene);
	void PopScene();

private:
};

LN_NAMESPACE_END
