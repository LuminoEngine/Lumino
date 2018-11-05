#pragma once

namespace ln {
class GraphicsContext;
class UIFrameWindow;
class UIViewport;

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

	static GraphicsContext* graphicsContext();
	static UIFrameWindow* mainWindow();
    static UIViewport* mainViewport();

};

} // namespace ln

