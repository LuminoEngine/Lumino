#pragma once
#include "../Graphics/GeometryStructs.hpp"

namespace ln {
class Application;
class GraphicsContext;
//class UIContext;
class UIFrameWindow;
class UIViewport;
class UIControl;
class UIFocusNavigator;
class WorldRenderView;
class PhysicsWorld;
class PhysicsWorld2D;
class World;
class Camera;
class EnvironmentLight;
class Dispatcher;

/** アプリケーション全体にかかわる処理を行います。 */
LN_CLASS(Static)
class LN_API Engine
{
public:

	/** エンジンの初期化処理を行います。 */
	LN_METHOD(RuntimeInitializer)
	static void initialize();

	/** エンジンの終了処理を行います。 */
	LN_METHOD()
	static void terminate();

	/**
	@brief		1フレーム分の更新処理を行います。
	@return		アプリケーションの終了が要求されている場合は false を返します。
	@details	この関数はグラフィックスと入力を更新し、指定されたフレームレートになるように待機します。
	*/
	LN_METHOD()
	static bool update();


	

	/**
	 * 指定した Application の実行を開始します。
	 *
	 * この機能を呼び出した場合、Engine::initialize(), Engine::finalize(), Engine::update() を呼び出すことはできなくなります。
	 * 代わりに Application::onInit(), Application::onUpdate() などを使用してください。
	 */
	static void run(Application* app);	// TODO: deprecated


    static void quit();

    static void resetFrameDelay();

    /** アプリケーション開始からの経過時間を取得します。この値はタイムスケールの影響を受けます。 */
	LN_METHOD(Property)
    static double time();
    
    /** アプリケーション全体のタイムスケールを設定します。 */
    static void setTimeScale(float value);

	/** セーブデータなど、アプリケーションデータを保存するための永続的なデータディレクトリのパスを返します。 */
	static const Path& persistentDataPath();

    /** メインウィンドウのタイトルバーに、秒間の平均 FPS を表示します。簡易的なパフォーマンス測定に利用できます。(default: false) */
    //static void setShowDebugFpsEnabled(bool enabled);



	static GraphicsContext* graphicsContext();
    //static UIContext* mainUIContext();
	static UIFrameWindow* mainWindow();
    /** デフォルトで作成される UIViewport は、MainWindow の直接の子要素となっています。 */
    static UIViewport* mainViewport();
	/** 。 */
	//LN_METHOD()
	static UIControl* mainUIView();
    static Size mainViewSize();

	/** デフォルトで作成されるメインの World です。 */
	LN_METHOD(Property)
    static World* world();

	/** デフォルトで作成されるメインの Camera です。 */
	LN_METHOD(Property)
    static Camera* mainCamera();

	/** デフォルトで作成されるメインの Light です。 */
	LN_METHOD(Property)
	static EnvironmentLight* mainLight();

	///** デフォルトで作成されるメインの AmbientLight です。 */
	////LN_METHOD(Property)
	//static AmbientLight* ambientLight();

	/** デフォルトで作成されるメインの RenderView です。 */
	LN_METHOD(Property)
	static WorldRenderView* renderView();
	
	/** デフォルトで作成されるメインの UIFocusNavigator です。 */
	//LN_METHOD(Property)
	static UIFocusNavigator* navigator();

	static UIControl* ui() { return mainUIView(); }

	static Dispatcher* dispatcher();

    //static AmbientLight* mainAmbientLight();
    //static DirectionalLight* mainDirectionalLight();
    static PhysicsWorld* mainPhysicsWorld();
    static PhysicsWorld2D* mainPhysicsWorld2D();

    // TODO: internal
	static World* activeWorld();
    static void setActiveWorld(World* world);
	static ln::Path findProjectLocalRoot();
};

} // namespace ln

