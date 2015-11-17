
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
	void UpdateFrame();

private:
	void ChangeNextScene();



	RefPtr<GameScene>			m_nextScene;
	RefPtr<GameScene>			m_runningScene;
	Stack<RefPtr<GameScene>>	m_sceneStack;	// m_runningScene �͊܂܂Ȃ�
};

LN_NAMESPACE_END
