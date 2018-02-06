
#pragma once
#include "Framework/Application.h"
#include "EngineSettings.h"

LN_NAMESPACE_BEGIN
class UIFrameWindow;
class UIViewport;
class RenderView;
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

	/** エンジンによって作成されたデフォルトの World2D を取得します。 */
	static World2D* defaultWorld2D();

	/** エンジンによって作成されたデフォルトの World3D を取得します。 */
	static World3D* defaultWorld3D();

	/** 現在のアクティブな 2D World を取得します。 */
	static World2D* activeWorld2D();

	/** 現在のアクティブな 3D World を取得します。 */
	static World3D* activeWorld3D();

	/** 現在のアクティブな 2D World を設定します。2D の WorldObject はインスタンス作成と同時にこの World へ追加されます。 */
	static void setActiveWorld2D(World2D* world);

	/** 現在のアクティブな 3D World を設定します。3D の WorldObject はインスタンス作成と同時にこの World へ追加されます。 */
	static void setActiveWorld3D(World3D* world);


	static Camera* getCamera3D();

	/** エンジンによって作成されたデフォルトの UIFrameWindow を取得します。 */
	static UIFrameWindow* getMainWindow();

	/** エンジンによって作成されたデフォルトの UIViewport を取得します。 */
	static UIViewport* getMainViewport();



	static RenderView* getDefault2DLayer();
	static RenderView* getDefault3DLayer();
	static UILayoutLayer* getDefaultUILayer();		// TODO: name Builtin
};

LN_NAMESPACE_END
