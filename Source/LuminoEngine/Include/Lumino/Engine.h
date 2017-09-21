
#pragma once
#include "Framework/Application.h"
#include "EngineSettings.h"

LN_NAMESPACE_BEGIN
class UIMainWindow;
class UIViewport;
class RenderLayer;
class UILayoutLayer;
//class SceneGraph2D;
//class SceneGraph3D;
class LightComponent;
class World;
class World2D;
class World3D;
class Camera;

/**
	@brief		アプリケーション全体にかかわる処理を行います。
*/
LN_CLASS(Static)
class Engine
{
public:
	
	/**
		@brief		エンジンの初期化処理を行います。
	*/
	LN_METHOD(RuntimeInitializer)
	static void initialize();
	
	/**
		@brief		エンジンの終了処理を行います。
	*/
	LN_METHOD()
	static void terminate();

	/**
		@brief		1フレーム分の更新処理を行います。
		@return		アプリケーションの終了が要求されている場合は false を返します。
		@details	この関数はグラフィックスと入力を更新し、指定されたフレームレートになるように待機します。
	*/
	LN_METHOD()
	static bool update();

	static void updateFrame();
	//static bool BeginRendering();	// TODO: 描画リスト作成開始前に描画中かを判定しても、描画リスト作成中に並列描画できない。十分にリソースを活用できていない。
	
	static void renderFrame();
	static void presentFrame();
	static bool isEndRequested();
	static void exit();
	static void setFrameUpdateMode(FrameUpdateMode mode);

	
	/**
		@brief		遅延をリセットします。
		@details	リソースのロード等で時間がかかり長い時間更新処理が行われなかった場合、
					updateFrame() は本来あるべき時間に追いつこうとしてしばらくの間ノーウェイトでフレーム更新が行われます。
					その間はアプリケーションが非常に高速に動作しているように見えてしまします。
					これを回避するため、時間のかかる処理の直後でこの関数を呼ぶことで、FPS 制御に遅延が発生していないことを伝えます。
	*/
	static void resetFrameDelay();

	/** エンジンによって作成されるデフォルトの World2D を取得します。 */
	static World2D* getWorld2D();

	/** エンジンによって作成されるデフォルトの World3D を取得します。 */
	static World3D* getWorld3D();

	static Camera* getCamera3D();


	/** エンジンによって作成されるデフォルトの UIViewport を取得します。 */
	static UIViewport* getMainViewport();



	static RenderLayer* getDefault2DLayer();
	static RenderLayer* getDefault3DLayer();
	static UILayoutLayer* getDefaultUILayer();		// TODO: name Builtin
};

LN_NAMESPACE_END
