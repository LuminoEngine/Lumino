
#pragma once
#include "Foundation/Application.h"
#include "EngineSettings.h"

LN_NAMESPACE_BEGIN
class RenderingContext;
class DrawingContext;

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
	static bool UpdateFrame();

	static bool BeginRendering();
	static void EndRendering();
	static void Render();
	static RenderingContext* GetMainRenderingContext();
	static DrawingContext* GetMainDrawingContext();
	static void Exit();
	static void SetFixedDeltaTime(float deltaTime);

	
	/**
		@brief		遅延をリセットします。
		@details	リソースのロード等で時間がかかり長い時間更新処理が行われなかった場合、
					UpdateFrame() は本来あるべき時間に追いつこうとしてしばらくの間ノーウェイトでフレーム更新が行われます。
					その間はアプリケーションが非常に高速に動作しているように見えてしまします。
					これを回避するため、時間のかかる処理の直後でこの関数を呼ぶことで、FPS 制御に遅延が発生していないことを伝えます。
	*/
	static void ResetFrameDelay();






	static ViewportLayer* GetDefault2DLayer();
	static ViewportLayer* GetDefault3DLayer();
};

LN_NAMESPACE_END
