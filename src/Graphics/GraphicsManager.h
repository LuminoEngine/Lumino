
#pragma once
#include <Lumino/Base/Cache.h>
#include <Lumino/Animation/Common.h>
#include <Lumino/Platform/Common.h>
#include <Lumino/Graphics/Common.h>
#include "Text/FontManager.h"	// TODO: remove
#include <Lumino/Graphics/Text/Font.h>
#include <Lumino/Graphics/SwapChain.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
class RenderingThread;
class RenderingContext2;
class GraphicsContext;
class PainterEngine;
class TextRenderer;

namespace detail
{
	class TextRendererCore;
}

/**
	@brief		グラフィックス機能の管理クラスです。
*/
class GraphicsManager
	: public RefObject
{
public:
	static GraphicsManager*	GetInstance(GraphicsManager* priority = nullptr);

public:
	void Finalize();

	/** 現在のグラフィックスシステムが使用している API の種類を確認します。*/
	GraphicsAPI GetGraphicsAPI() const;

	/** 現在のグラフィックスシステムのレンダリング方法を確認します。*/
	RenderingType GetRenderingType() const { return m_renderingType; }

	/** グラフィックスシステムのメイン Renderer を取得します。*/
	Details::Renderer* GetRenderer() const { return m_renderer; }

	/** メインの SwapChain を取得します。これは Create() で指定されたメインウィンドウへのスワップチェインです。*/
	SwapChain* GetMainSwapChain() { return m_mainSwapChain; }

	Viewport* GetMainViewport() { return m_mainViewport; }

	detail::AnimationManager* GetAnimationManager() const { return m_animationManager; }

	/** 関連付けられている FileManager を取得します。*/
	FileManager* GetFileManager() const { return m_fileManager; }

	PlatformWindow* GetMainWindow() const { return m_mainWindow; }

	/** 関連付けられている FontManager を取得します。*/
	FontManager* GetFontManager() const { return m_fontManager; }

	GraphicsContext* GetGraphicsContext() const { return m_graphicsContext; }

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
		int		EdgeSize;
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

	struct ConfigData
	{
		GraphicsAPI				GraphicsAPI;			/**< レンダリングに使用する API の種類 */
		RenderingType			RenderingType;
		PlatformWindow*			MainWindow;				/**< アプリケーションのメインウィンドウ */
		Size					backBufferSize;			// バックバッファのサイズ
		detail::AnimationManager*	animationManager = nullptr;
		FileManager*				FileManager;			/**< FileManager */
		bool						PlatformTextureLoading;	/**< 画像リソースの読み込みにプラットフォーム固有の機能を使用するか */
		void*						D3D9Device;				/**< 作成済みの IDirect3DDevice9 インターフェイス */
		bool						fpuPreserveEnabled = false;

		ConfigData()
			: GraphicsAPI(GraphicsAPI::DirectX9)
			, RenderingType(RenderingType::Deferred)
			, MainWindow(NULL)
			, FileManager(NULL)
			, PlatformTextureLoading(false)
			, D3D9Device(NULL)
		{}
	};

	GraphicsManager(const ConfigData& configData);
	~GraphicsManager();

LN_INTERNAL_ACCESS:
	void AddResourceObject(GraphicsResourceObject* obj) { m_resourceObjectList.Add(obj); }
	void RemoveResourceObject(GraphicsResourceObject* obj) { m_resourceObjectList.Remove(obj); }
	Driver::IGraphicsDevice* GetGraphicsDevice() { return m_graphicsDevice; }
	RenderingThread* GetRenderingThread() { return m_renderingThread; }
	bool IsPlatformTextureLoading() { return m_platformTextureLoading; }
	RenderingContext2* GetRenderingContext() { return m_renderingContext; }
	RenderingCommandList* GetPrimaryRenderingCommandList();
	PainterEngine* GetPainterEngine() { return m_painterEngine; }
	detail::TextRendererCore* GetTextRendererCore() { return m_textRendererCore; }
	Driver::ITexture* GetDummyTexture() { return m_dummyTexture; }

private:
	detail::AnimationManager*		m_animationManager;
	FileManager*					m_fileManager;
	PlatformWindow*					m_mainWindow;
	RefPtr<FontManager>				m_fontManager;
	RenderingType					m_renderingType;
	RefPtr<CacheManager>			m_glyphTextureCache;
	Array<GraphicsResourceObject*>	m_resourceObjectList;
	
	Driver::IGraphicsDevice*		m_graphicsDevice;
	RefPtr<SwapChain>				m_mainSwapChain;
	Details::Renderer*				m_renderer;
	RenderingThread*				m_renderingThread;
	Viewport*						m_mainViewport;

	detail::IContext*				m_activeContext;
	RenderingContext2*				m_renderingContext;
	GraphicsContext*				m_graphicsContext;
	detail::TextRendererCore*		m_textRendererCore;
	PainterEngine*					m_painterEngine;
	Driver::ITexture*				m_dummyTexture;
	bool							m_platformTextureLoading;

};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
