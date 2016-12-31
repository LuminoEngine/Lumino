
#pragma once
#include "Foundation/Application.h"
#include "EngineSettings.h"

LN_NAMESPACE_BEGIN
class UIMainWindow;
class SceneGraph2D;
class SceneGraph3D;
class Light;

/**
	@brief		アプリケーション全体にかかわる処理を行います。
*/
class Engine
{
public:
	
	/**
		@brief		デフォルトの設定でエンジンの初期化処理を行います。
	*/
	static void Initialize();
	
	/**
		@brief		エンジンの終了処理を行います。
	*/
	static void Terminate();

	/**
		@brief		1フレーム分の更新処理を行います。
		@return		アプリケーションの終了が要求されている場合は false を返します。
		@details	この関数はグラフィックスと入力を更新し、指定されたフレームレートになるように待機します。
	*/
	static bool Update();

	static void BeginFrameUpdate();
	static void EndFrameUpdate();
	static bool BeginRendering();	// TODO: 描画リスト作成開始前に描画中かを判定しても、描画リスト作成中に並列描画できない。十分にリソースを活用できていない。
	static void EndRendering();
	static void Render();
	static void Exit();
	static void SetFrameUpdateMode(FrameUpdateMode mode);

	
	/**
		@brief		遅延をリセットします。
		@details	リソースのロード等で時間がかかり長い時間更新処理が行われなかった場合、
					UpdateFrame() は本来あるべき時間に追いつこうとしてしばらくの間ノーウェイトでフレーム更新が行われます。
					その間はアプリケーションが非常に高速に動作しているように見えてしまします。
					これを回避するため、時間のかかる処理の直後でこの関数を呼ぶことで、FPS 制御に遅延が発生していないことを伝えます。
	*/
	static void ResetFrameDelay();






	static UIMainWindow* GetMainWindow();
	static Viewport* GetMainViewport();
	static ViewportLayer* GetDefault2DLayer();
	static ViewportLayer* GetDefault3DLayer();
	static SceneGraph2D* GetDefaultSceneGraph2D();
	static SceneGraph3D* GetDefaultSceneGraph3D();
	static Light* GetMainLight3D();
};

LN_NAMESPACE_END
