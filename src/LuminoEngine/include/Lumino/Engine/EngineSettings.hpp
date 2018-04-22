
#pragma once

namespace ln {

/** アプリケーションの初期化設定です。 */
class EngineSettings
{
public:

	/** メインウィンドウのクライアント領域の幅と高さを設定します。(default: 640x480) */
	static void setMainWindowSize(int width, int height);

	/** メインウィンドウに対して作成されるバックバッファのサイズを設定します。(default: 640x480) */
	static void setMainBackBufferSize(int width, int height);

	/** メインウィンドウのタイトル文字列を設定します。*/
	static void setMainWindowTitle(const StringRef& title);
	
//	/** デバッグ用のログファイルの出力有無を設定します。(default: Debug ビルドの場合true、それ以外は false) */
//	static void setEngineLogEnabled(bool enabled);
//
//	/** 指定したフォルダをアセットフォルダとして扱います。アセットフォルダのファイルにアクセスするには Assets クラスの機能を使用します。 */
//	static void addAssetsDirectory(const StringRef& directoryPath);
//
//	/** グラフィックス機能で使用する描画 API を設定します。(default: Windows の場合 DirectX9、それ以外は OpenGL) */
//	static void setGraphicsAPI(GraphicsAPI graphicsAPI);
//
//	/** グラフィックス機能で使用するレンダリング方法を設定します。(default: Threaded) */
//	static void setGraphicsRenderingType(GraphicsRenderingType renderingType);
//	
//	/** 浮動小数点計算の精度を指定します。true の場合、グラフィックス機能で使用する描画 API が DirectX9 である場合、D3DCREATE_FPU_PRESERVE が設定されます。*/
//	static void setFpuPreserveEnabled(bool enabled);
//
//	/** DirectMusic の初期化方法を設定します。(default: NotUse) */
//	static void setDirectMusicMode(DirectMusicMode mode);
//
//	/** DirectMusic のリバーブエフェクトの強さを設定します。(default: 0.75) */
//	static void setDirectMusicReverbLevel(float level);
//
//#ifdef LN_OS_WIN32
//	/** ユーザー指定のメインウィンドウのウィンドウハンドルを設定します。*/
//	static void setUserWindowHandle(intptr_t hWnd);
//
//	/** 既に作成済みの IDirect3DDevice9 インターフェイスを利用する場合、そのポインタを指定します。*/
//	static void setD3D9Device(void* device);
//#endif
};

} // namespace ln
