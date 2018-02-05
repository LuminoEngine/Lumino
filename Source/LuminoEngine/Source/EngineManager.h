
#pragma once
#include <Lumino/Base/RefObject.h>
#include <Lumino/Base/String.h>
#include <Lumino/Base/List.h>
#include <Lumino/Threading/Thread.h>
#include "Platform/PlatformManager.h"
#include <Lumino/Platform/EventListener.h>
#include <Lumino/Game/FpsController.h>
#include <Lumino/EngineSettings.h>

LN_NAMESPACE_BEGIN

namespace detail { class AnimationManager; }
namespace detail { class InputManager; }
namespace detail { class GraphicsManager; }
namespace detail { class EffectManager; }
namespace detail { class ModelManager; }
namespace detail { class UIManager; }
namespace detail { class AudioManager; }
namespace detail { class PhysicsManager; }
namespace detail { class DocumentsManager; }
class FileManager;
namespace detail { class ArchiveManager; }
class SceneGraphManager;
class Application;
class EngineDiagViewer;
class AssetsManager;
class World2D;
class World3D;

namespace detail
{

class EngineSettings
{
public:
	static EngineSettings instance;

	struct ArchiveFileEntry
	{
		PathName	filePath;
		String		password;
	};

	struct CacheCapacity
	{
		int			objectCount = 32;	/**< キャッシュに保持できる最大オブジェクト数 */
		size_t		memorySize = 0;		/**< キャッシュに保持できる最大メモリ量 (byte単位。0 の場合はメモリ量を考慮しない) */
	};

	/** エンジンのアクセラレータキー */
	struct EngineAcceleratorKeys
	{
		InputBindingPtr	toggleShowDiag;
	};


public:

	/** メインウィンドウのクライアント領域の幅と高さです。(初期値:640x480) */
	SizeI mainWindowSize = SizeI(640, 480);

	/** メインウィンドウに対して作成されるバックバッファのサイズです。(初期値:640x480) */
	SizeI mainBackBufferSize = SizeI(640, 480);

	/** メインウィンドウのタイトル文字列です。*/
	String mainWindowTitle = _LT("Lumino");
	
	/** デバッグ用のログファイルの出力有無を設定します。(初期値:Debug ビルドの場合true、それ以外は false) */
	bool engineLogEnabled = false;

	/**
		@brief		標準入出力用のコンソールウィンドウを割り当てるかどうかを設定します。(初期値:false)
	*/
	bool ConsoleEnabled;
	// TODO: いらない。Core の Console クラスを利用する。

	/**
		@brief		登録するアーカイブファイルのリストです。
	*/
	List<ArchiveFileEntry>	ArchiveFileEntryList;

	List<PathName> assetsDirectoryList;
	
	/**
		@brief		ファイルを開く時の検索場所の優先順です。
	*/
	FileAccessPriority	FileAccessPriority;

	/** ウィンドウシステムで使用する API を指定します。 */
	WindowSystemAPI	windowSystemAPI = WindowSystemAPI::Default;

	/**
		@brief		グラフィックス機能で使用する描画 API を指定します。
		@details	初期値は Windows の場合 DirectX9 で、それ以外は OpenGL です。
	*/
	GraphicsAPI		graphicsAPI = GraphicsAPI::DirectX9;

	/**
		@brief		グラフィックス機能で使用するレンダリング方法です。(初期値:Threaded)
	*/
	GraphicsRenderingType	renderingType = GraphicsRenderingType::Threaded;

	bool		delayedRenderingSkip = false;

	/** ユーザー定義のウィンドウハンドル (windows の場合は HWND) */
	intptr_t			userMainWindow = 0;

	/**
		@brief		Direct3D の浮動小数点計算の精度に関する情報です。詳しくは MSDN の D3DCREATE_ENABLE_PRESENTSTATS を参照してください。
	*/
	bool	fpuPreserveEnabled = false;

#ifdef LN_OS_WIN32
	/** 既に作成済みの IDirect3DDevice9 インターフェイスを利用する場合、そのポインタを指定します。*/
	void*	D3D9Device;
#endif
	
	/**
		@brief		音声データのキャッシュ容量です。
	*/
	CacheCapacity	soundCacheCapacity;
	
	/**
		@brief		DirectMusic の初期化方法の指定です。
	*/
	DirectMusicMode	directMusicMode = DirectMusicMode::NotUse;

	/**
		@brief		DirectMusic のリバーブエフェクトの強さです。(規定値:0.75f)
	*/
	float	DirectMusicReverbLevel;

	/** エンジンのアクセラレータキー */
	EngineAcceleratorKeys	acceleratorKeys;

	/**
		@brief	内部的に COM の初期化を行います。
	*/
	bool autoCoInitialize = true;

	// UI Skin
	PathName	defaultSkinFilePath;

	// internal
	PathName	assetDirecotry;

public:
	EngineSettings()
		: ConsoleEnabled(false)
		, ArchiveFileEntryList()
		, FileAccessPriority(FileAccessPriority_DirectoryFirst)
#ifdef LN_OS_WIN32
		, D3D9Device(NULL)
#endif
		, DirectMusicReverbLevel(0.75f)
	{
#ifdef LN_DEBUG
		engineLogEnabled = true;
		acceleratorKeys.toggleShowDiag = KeyGesture::create(Keys::F3);
#endif
		//engineAcceleratorKeys[(int)EngineAcceleratorKey::ToggleShowDiag] = Key::F3;
	}
};
} // namespace detail

class EngineManager
	: public RefObject
{
public:
	static EngineManager*	Instance;

	static EngineManager* getInstance(EngineManager* priority = nullptr);

public:
	static const Char*	LogFileName;

public:

	/**
		@brief		
	*/
	static EngineManager* create(const detail::EngineSettings& configData);

public:
	void setFrameUpdateMode(FrameUpdateMode mode) { m_frameUpdateMode = mode; }

	bool updateUnitily();

	void updateFrame();
	//bool BeginRendering();
	void renderFrame();
	void endRendering();
	void presentFrame();
	void resetFrameDelay();
	bool isEndRequested() const { return m_endRequested; }
	void exit();

	const PathName& getInstallDir() const { return m_installDir; }

	PlatformManager* getPlatformManager() const { return m_platformManager; }
	const Ref<detail::ArchiveManager>& getArchiveManager() const { return m_archiveManager; }
	detail::AnimationManager* getAnimationManager() const { return m_animationManager; }
	detail::AudioManager* getAudioManager() const { return m_audioManager; }
	detail::PhysicsManager* getPhysicsManager() const;
	detail::GraphicsManager* getGraphicsManager() const { return m_graphicsManager; }
	detail::ModelManager* getModelManager() const { return m_modelManager; }
	detail::DocumentsManager* getDocumentsManager() const { return m_documentsManager; }
	detail::UIManager* getUIManager() const { return m_uiManager; }
	SceneGraphManager* getSceneGraphManager() const { return m_sceneGraphManager; }
	//Application* GetApplication() const { return m_application; }

	const FpsController& getFpsController() const { return m_fpsController; }
	EngineDiagViewer* getEngineDiagViewer() const { return m_diagViewer; }
	World2D* defaultWorld2D() const;
	World3D* defaultWorld3D() const;
	World2D* activeWorld2D() const;
	World3D* activeWorld3D() const;

protected:
	EngineManager(const detail::EngineSettings& configData);
	virtual ~EngineManager();

public:
	void initialize();
	void initializeCommon();
	void initializeAnimationManager();
	void initializeFileManager();
	void initializePlatformManager();
	void initializeInputManager();
	void initializeAudioManager();
	void initializePhysicsManager();
	void initializeGraphicsManager();
	void initializeEffectManager();
	void initializeModelManager();
	void initializeDocumentsManager();
	void initializeUIManager();
	void initializeSceneGraphManager();
	void initializeAssetsManager();
	//void onLostDevice();
	//void onResetDevice();

private:
	//class NativeWindowEventListener;

	detail::EngineSettings				m_configData;
	PathName							m_installDir;
	FpsController						m_fpsController;
	FrameUpdateMode						m_frameUpdateMode;

	detail::AnimationManager*			m_animationManager;
	FileManager*						m_fileManager;
	Ref<detail::ArchiveManager>			m_archiveManager;
	Ref<PlatformManager>				m_platformManager;
	detail::InputManager*				m_inputManager;
	detail::AudioManager*				m_audioManager;
	Ref<detail::PhysicsManager>		m_physicsManager;
	detail::GraphicsManager*			m_graphicsManager;
	detail::EffectManager*				m_effectManager;
	detail::ModelManager*				m_modelManager;
	detail::DocumentsManager*			m_documentsManager;
	detail::UIManager*					m_uiManager;
	SceneGraphManager*					m_sceneGraphManager;
	AssetsManager*						m_assetsManager;

	EngineDiagViewer*					m_diagViewer;
	Ref<World2D>						m_defaultWorld2D;
	Ref<World3D>						m_defaultWorld3D;
	Ref<World2D>						m_activeWorld2D;
	Ref<World3D>						m_activeWorld3D;

	//bool								m_frameRenderingSkip;
	bool								m_frameRenderd;
	bool								m_commonInitied;
	bool								m_endRequested;
	bool								m_comInitialized;
};

LN_NAMESPACE_END
