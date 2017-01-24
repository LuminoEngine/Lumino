
#pragma once
#include "Audio/Common.h"
#include "Graphics/Common.h"
#include "Input/InputBinding.h"

LN_NAMESPACE_BEGIN

/**
	@brief		アプリケーションの初期化設定です。
*/
class EngineSettings
{
public:

	/** メインウィンドウのクライアント領域の幅と高さを設定します。(default: 640x480) */
	static void SetMainWindowSize(const SizeI& size);
	static void SetMainWindowSize(int width, int height);		/**< @overload SetMainWindowSize */

	/** メインウィンドウに対して作成されるバックバッファのサイズを設定します。(default: 640x480) */
	static void SetMainBackBufferSize(const SizeI& size);
	static void SetMainBackBufferSize(int width, int height);	/**< @overload SetMainBackBufferSize */

	/** メインウィンドウのタイトル文字列を設定します。*/
	static void SetMainWindowTitle(const StringRef& title);
	
	/** デバッグ用のログファイルの出力有無を設定します。(default: Debug ビルドの場合true、それ以外は false) */
	static void SetEngineLogEnabled(bool enabled);

	/** グラフィックス機能で使用する描画 API を設定します。(default: Windows の場合 DirectX9、それ以外は OpenGL) */
	static void SetGraphicsAPI(GraphicsAPI graphicsAPI);

	/** グラフィックス機能で使用するレンダリング方法を設定します。(default: Threaded) */
	static void SetGraphicsRenderingType(GraphicsRenderingType renderingType);
	
	/** 浮動小数点計算の精度を指定します。true の場合、グラフィックス機能で使用する描画 API が DirectX9 である場合、D3DCREATE_FPU_PRESERVE が設定されます。*/
	static void SetFpuPreserveEnabled(bool enabled);

	/** DirectMusic の初期化方法を設定します。(default: NotUse) */
	static void SetDirectMusicMode(DirectMusicMode mode);

	/** DirectMusic のリバーブエフェクトの強さを設定します。(default: 0.75) */
	static void SetDirectMusicReverbLevel(float level);

#ifdef LN_OS_WIN32
	/** ユーザー指定のメインウィンドウのウィンドウハンドルを設定します。*/
	static void SetUserWindowHandle(intptr_t hWnd);
#endif
};

LN_NAMESPACE_END
