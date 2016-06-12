
#pragma once
#include <Lumino/Base/Cache.h>
#include <Lumino/Animation/Common.h>
#include <Lumino/IO/Common.h>
#include <Lumino/Platform/Common.h>
#include <Lumino/Graphics/Common.h>
#include <Lumino/Graphics/Text/Font.h>

LN_NAMESPACE_BEGIN
class FileManager;

LN_NAMESPACE_GRAPHICS_BEGIN
class RenderingThread;
class RenderingContext;
class GraphicsContext;
class PainterEngine;
class TextRenderer;
class BitmapTextRenderer;
class SwapChain;

namespace detail
{
	class IContext;
	class TextRendererCore;
}

class IDeviceResetListener
{
public:
	virtual void OnLostDevice() = 0;
	virtual void OnResetDevice() = 0;

protected:
	virtual ~IDeviceResetListener() = default;
};

/**
	@brief		グラフィックス機能の管理クラスです。
*/
class GraphicsManager
	: public RefObject
{
public:

	struct ConfigData
	{
		GraphicsAPI				GraphicsAPI;			/**< レンダリングに使用する API の種類 */
		GraphicsRenderingType			RenderingType;
		PlatformWindow*			MainWindow;				/**< アプリケーションのメインウィンドウ */
		Size					backBufferSize;			// バックバッファのサイズ
		detail::AnimationManager*	animationManager = nullptr;
		FileManager*				FileManager;			/**< FileManager */
		bool						PlatformTextureLoading;	/**< 画像リソースの読み込みにプラットフォーム固有の機能を使用するか */
		void*						D3D9Device;				/**< 作成済みの IDirect3DDevice9 インターフェイス */
		bool						fpuPreserveEnabled = false;

		ConfigData()
			: GraphicsAPI(GraphicsAPI::DirectX9)
			, RenderingType(GraphicsRenderingType::Threaded)
			, MainWindow(NULL)
			, FileManager(NULL)
			, PlatformTextureLoading(false)
			, D3D9Device(NULL)
		{}
	};

	static GraphicsManager*	GetInstance(GraphicsManager* priority = nullptr);

public:
	GraphicsManager();
	~GraphicsManager();

	void Initialize(const ConfigData& configData);
	void Finalize();

	/** 現在のグラフィックスシステムが使用している API の種類を確認します。*/
	GraphicsAPI GetGraphicsAPI() const;

	/** 現在のグラフィックスシステムのレンダリング方法を確認します。*/
	GraphicsRenderingType GetRenderingType() const { return m_renderingType; }

	/** グラフィックスシステムのメイン Renderer を取得します。*/
	Details::Renderer* GetRenderer() const { return m_renderer; }

	/** メインの SwapChain を取得します。これは Create() で指定されたメインウィンドウへのスワップチェインです。*/
	SwapChain* GetMainSwapChain() { return m_mainSwapChain; }


	detail::AnimationManager* GetAnimationManager() const { return m_animationManager; }

	/** 関連付けられている FileManager を取得します。*/
	FileManager* GetFileManager() const { return m_fileManager; }

	PlatformWindow* GetMainWindow() const { return m_mainWindow; }

	/** 関連付けられている FontManager を取得します。*/
	FontManager* GetFontManager() const { return m_fontManager; }

	GraphicsContext* GetGraphicsContext() const { return m_graphicsContext; }


	BitmapTextRenderer* GetBitmapTextRenderer() const { return m_bitmapTextRenderer; }

	void AddDeviceResetListener(IDeviceResetListener* listener) { m_deviceResetListenerList.Add(listener); }
	void RemoveDeviceResetListener(IDeviceResetListener* listener) { m_deviceResetListenerList.Remove(listener); }

	/// TODO: (GraphicsDevice を作成したスレッドと同じスレッドで呼び出す)
	void PauseDevice();
	/// TODO: (GraphicsDevice を作成したスレッドと同じスレッドで呼び出す)
	void ResumeDevice();

	void ChangeDevice(Driver::IGraphicsDevice* device);

	void SwitchActiveContext(detail::IContext* context);
	detail::IContext* GetActiveContext() const { return m_activeContext; }

public:	// TODO: internal
	struct FontData
	{
		String	Family;
		int		Size;
		//int		EdgeSize;
		bool	IsBold;
		bool	IsItalic;
		bool	IsAntiAlias;

		Font* CreateFontFromData(FontManager* m) const;
	};

	static uint64_t CalcFontSettingHash(const FontData& fontData);

	///// 指定したフォント設定に一致する TextRenderer* を検索する。
	///// あくまでグリフテクスチャのキャッシュを使いまわすためのものであることに注意。
	///// 取得した TextRenderer に SetFont してはならないし、
	///// 色や配置設定は全て再設定しなければならない。(者と同じ設定であるとは限らない)
	///// また、参照カウントを増やして返す。
	//TextRenderer* LookupTextRenderer(const FontData& fontData);

	Internal::FontGlyphTextureCache* LookupGlyphTextureCache(const FontData& fontData);
	Internal::FontGlyphTextureCache* LookupGlyphTextureCache(Font* font);

public:	// TODO
	friend class Helper;
	//friend class Application;


LN_INTERNAL_ACCESS:
	void AddResourceObject(GraphicsResourceObject* obj) { m_resourceObjectList.Add(obj); }
	void RemoveResourceObject(GraphicsResourceObject* obj) { m_resourceObjectList.Remove(obj); }
	Driver::IGraphicsDevice* GetGraphicsDevice() { return m_graphicsDevice; }
	RenderingThread* GetRenderingThread() { return m_renderingThread; }
	bool IsPlatformTextureLoading() { return m_platformTextureLoading; }
	RenderingContext* GetRenderingContext() { return m_renderingContext; }
	RenderingCommandList* GetPrimaryRenderingCommandList();
	PainterEngine* GetPainterEngine() { return m_painterEngine; }
	detail::TextRendererCore* GetTextRendererCore() { return m_textRendererCore; }
	Driver::ITexture* GetDummyTexture() { return m_dummyTexture; }

private:
	detail::AnimationManager*		m_animationManager;
	FileManager*					m_fileManager;
	PlatformWindow*					m_mainWindow;
	FontManager*					m_fontManager;
	GraphicsRenderingType			m_renderingType;
	RefPtr<CacheManager>			m_glyphTextureCache;
	Array<GraphicsResourceObject*>	m_resourceObjectList;
	Array<IDeviceResetListener*>	m_deviceResetListenerList;
	
	Driver::IGraphicsDevice*		m_graphicsDevice;
	SwapChain*						m_mainSwapChain;
	Details::Renderer*				m_renderer;
	RenderingThread*				m_renderingThread;

	detail::IContext*				m_activeContext;
	RenderingContext*				m_renderingContext;
	GraphicsContext*				m_graphicsContext;
	detail::TextRendererCore*		m_textRendererCore;
	PainterEngine*					m_painterEngine;
	BitmapTextRenderer*				m_bitmapTextRenderer;

	Driver::ITexture*				m_dummyTexture;
	bool							m_platformTextureLoading;
};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
