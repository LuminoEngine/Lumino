
#pragma once
#include "Common.hpp"
#include <LuminoEngine/Asset/Common.hpp>
#include "../Input/Common.hpp"
#include <LuminoPlatform/Common.hpp>
#include <LuminoGraphics/Common.hpp>

namespace ln {

/** アプリケーション起動時に参照する初期化設定です。 */
LN_CLASS(Static)
class EngineSettings
{
public:
	/** メインウィンドウのタイトル文字列を設定します。*/
	LN_METHOD()
	static void setMainWindowTitle(const String& title);

	/** メインウィンドウのクライアント領域の幅と高さを設定します。(default: 640x480) */
	LN_METHOD()
	static void setMainWindowSize(int width, int height);

	/** メインウィンドウに対して作成される WorldView のサイズを設定します。(default: クライアント領域のサイズと同等) */
	LN_METHOD()
	static void setMainWorldViewSize(int width, int height);
	
	/** メインウィンドウのサイズをユーザーが変更できるかどうかを指定します。(default: false) */
	LN_METHOD()
	static void setMainWindowResizable(bool value);

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
//	static void addAssetsDirectory(const StringView& directoryPath);
//
	/** グラフィックス機能で使用する描画 API を設定します。(default: false) */
	static void setGraphicsAPI(GraphicsAPI graphicsAPI);

	/** グラフィックスバックエンドのデバッグ機能を有効にします。 実際にグラフィックスバックエンドがデバッグ機能をサポートしている場合、診断結果がログに出力されます。 (default: Default) */
	static void setGraphicsDebugEnabled(bool enabled);

	/** デフォルトの UI テーマ名を設定します。 */
	LN_METHOD()
	static void setUITheme(const String& value);
	
	/** デフォルトのフォントファイルを設定します。 */
	LN_METHOD()
	static void setFontFile(const String& filePath);

	/**
	 * エンジン初期化時に、シーンを表示するために必要な各オブジェクトを自動作成するかどうかを指定します。 (default: true)
	 *
	 * true の場合、UIMainWindow, UIViewport, World, Camera など、
	 * ゲームアプリケーションとして動作させるために必要となる基本的なオブジェクトを作成します。
	 *
	 * false の場合、必要なオブジェクトはユーザープログラムで生成しなければなりません。
	 * UIMainWindow はアプリケーションの種類にかかわらず必ず必要となります。
	 * Application::onInit() で setupWindow() を呼び出すことで、独自の UIMainWindow を使用することができます。
	 */
	static void setSceneIntegrationEnabled(bool value);

	

	///**  */
	//LN_METHOD()
	//static void setUseSystemStringEncoding(bool value);


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
	 * デバッグモードの有無を設定します。(default: Debug ビルドの場合true、それ以外は false)
	 */
	LN_METHOD()
	static void setDebugMode(bool enabled);
	
	/**
	 * デバッグ用のログファイルの出力先ファイルパスを設定します。(default: Empty(実行ファイルのディレクトリへ出力))
	 */
	LN_METHOD()
	static void setEngineLogFilePath(const String& filePath);
	
	/**
	 * 優先的に使用する GPU の名前を指定します。
	 * 
	 * 現在は DirectX12 を使用する場合のみ有効で、デフォルトの GPU では動作が不安定な場合に "Microsoft Basic Render Driver" 等を試すことができます。
	 */
	LN_METHOD()
	static void setPriorityGPUName(const String& filePath);

	/**
	 * setDeveloperToolEnabled
	 */
	LN_METHOD()
	static void setDevelopmentToolsEnabled(bool enabled);

	/** ユーザー指定のメインウィンドウのウィンドウハンドルを設定します。*/
	LN_METHOD()
	static void setUserMainWindow(intptr_t value);

    /**
     * デフォルトで生成される PlatformWindow のインスタンスを ProxyPlatformWindow とするかどうかを設定します。
     */
	static void setUseProxyPlatformWindow(bool value);


    static void setUseExternalSwapChain(bool value);

	static void setInputBindingSet(InputBindingSet value);


    static void setStandaloneFpsControl(bool enabled);
    static void setEngineFeatures(Flags<EngineFeature> features);

    static void setDefaultObjectsCreation(bool value);
    static void setWindowSystem(WindowSystem value);
    static void setGraphicsContextManagement(bool value);
    static void setExternalMainLoop(bool value);
};

} // namespace ln
