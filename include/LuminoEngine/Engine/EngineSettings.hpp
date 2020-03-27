
#pragma once
#include "Common.hpp"
#include "../Asset/Common.hpp"
#include "../Graphics/Common.hpp"

namespace ln {

/** アプリケーション起動時に参照する初期化設定です。 */
LN_CLASS(Static)
class EngineSettings
{
public:

	/** メインウィンドウのクライアント領域の幅と高さを設定します。(default: 640x480) */
	LN_METHOD()
	static void setMainWindowSize(int width, int height);

	/** メインウィンドウに対して作成される WorldView のサイズを設定します。(default: クライアント領域のサイズと同等) */
	LN_METHOD()
	static void setMainWorldViewSize(int width, int height);

	/** メインウィンドウのタイトル文字列を設定します。*/
	LN_METHOD()
	static void setMainWindowTitle(const String& title);

	/** アセットが保存されているディレクトリを登録します。 */
	LN_METHOD()
	static void addAssetDirectory(const String& path);

	/** アセットファイルを登録します。 */
	LN_METHOD()
	static void addAssetArchive(const String& fileFullPath, const String& password);

	/** アセットが保存されている場所へのアクセス優先度を設定します。(default:DirectoryFirst) */
	static void setAssetStorageAccessPriority(AssetStorageAccessPriority value);
	
	/** フレームレートを設定します。(default: 60) */
	LN_METHOD()
	static void setFrameRate(int value);
//
//	/** 指定したフォルダをアセットフォルダとして扱います。アセットフォルダのファイルにアクセスするには Assets クラスの機能を使用します。 */
//	static void addAssetsDirectory(const StringRef& directoryPath);
//
	/** グラフィックス機能で使用する描画 API を設定します。(default: false) */
	static void setGraphicsAPI(GraphicsAPI graphicsAPI);

	/** グラフィックスバックエンドのデバッグ機能を有効にします。 実際にグラフィックスバックエンドがデバッグ機能をサポートしている場合、診断結果がログに出力されます。 (default: Default) */
	static void setGraphicsDebugEnabled(bool enabled);
	

//	/** グラフィックス機能で使用するレンダリング方法を設定します。(default: Threaded) */
//	static void setGraphicsRenderingType(RenderingType renderingType);
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
	
	/**
	 * (default: Debug ビルドの場合true、それ以外は false)
	 */
	LN_METHOD()
	static void setDebugToolEnabled(bool enabled);

	/**
	 * デバッグ用のログファイルの出力有無を設定します。(default: Debug ビルドの場合true、それ以外は false)
	 */
	LN_METHOD()
	static void setEngineLogEnabled(bool enabled);
	
	/**
	 * デバッグ用のログファイルの出力先ファイルパスを設定します。(default: Empty(実行ファイルのディレクトリへ出力))
	 */
	LN_METHOD()
	static void setEngineLogFilePath(const String& filePath);



	/** ユーザー指定のメインウィンドウのウィンドウハンドルを設定します。*/
	static void setUserMainWindow(intptr_t value);


    static void setStandaloneFpsControl(bool enabled);
    static void setEngineFeatures(Flags<EngineFeature> features);

    static void setDefaultObjectsCreation(bool value);
    static void setUseGLFWWindowSystem(bool value);
    static void setGraphicsContextManagement(bool value);
    static void setExternalMainLoop(bool value);
    static void setExternalRenderingManagement(bool value);
};

} // namespace ln
