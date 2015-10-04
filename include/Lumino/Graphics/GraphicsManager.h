
#pragma once
#include <Lumino/Base/Cache.h>
#include "Common.h"
#include "../Graphics/FontManager.h"
#include "../Graphics/Font.h"
#include "SwapChain.h"

namespace Lumino
{
LN_NAMESPACE_GRAPHICS_BEGIN
class RenderingThread;
class PainterEngine;
class TextRenderer;


typedef RefPtr<GraphicsManager>	GraphicsManagerPtr;

/**
	@brief		グラフィックス機能の管理クラスです。
*/
class GraphicsManager
	: public RefObject
{
public:

	/** 現在のグラフィックスシステムが使用している API の種類を確認します。*/
	GraphicsAPI GetGraphicsAPI() const;

	/** 現在のグラフィックスシステムのレンダリング方法を確認します。*/
	RenderingType GetRenderingType() const { return m_renderingType; }

	/** グラフィックスシステムのメイン Renderer を取得します。*/
	Details::Renderer* GetRenderer() const { return m_renderer; }

	/** メインの SwapChain を取得します。これは Create() で指定されたメインウィンドウへのスワップチェインです。*/
	SwapChain* GetMainSwapChain() { return m_mainSwapChain; }

	/** 関連付けられている FileManager を取得します。*/
	FileManager* GetFileManager() const { return m_fileManager; }

	/** 関連付けられている FontManager を取得します。*/
	FontManager* GetFontManager() const { return m_fontManager; }

	/// TODO: (GraphicsDevice を作成したスレッドと同じスレッドで呼び出す)
	void PauseDevice();
	/// TODO: (GraphicsDevice を作成したスレッドと同じスレッドで呼び出す)
	void ResumeDevice();

public:	// TODO: internal
	struct FontData
	{
		String	Family;
		int		Size;
		int		EdgeSize;
		bool	IsBold;
		bool	IsItalic;
		bool	IsAntiAlias;

		Font* CreateFontFromData(FontManager* m) const
		{
			Font* font = Font::Create(m);
			font->SetName(Family);
			font->SetSize(Size);
			font->SetEdgeSize(EdgeSize);
			font->SetBold(IsBold);
			font->SetItalic(IsItalic);
			font->SetAntiAlias(IsAntiAlias);
			return font;
		}
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
		Platform::Window*		MainWindow;				/**< アプリケーションのメインウィンドウ */
		Lumino::FileManager*	FileManager;			/**< FileManager */
		bool					PlatformTextureLoading;	/**< 画像リソースの読み込みにプラットフォーム固有の機能を使用するか */
#ifdef LN_OS_WIN32
		void*					D3D9Device;				/**< 作成済みの IDirect3DDevice9 インターフェイス */
#endif
		ConfigData()
			: GraphicsAPI(GraphicsAPI::DirectX9)
			, RenderingType(RenderingType::Deferred)
			, MainWindow(NULL)
			, FileManager(NULL)
			, PlatformTextureLoading(false)
#ifdef LN_OS_WIN32
			, D3D9Device(NULL)
#endif
		{}
	};

	GraphicsManager(const ConfigData& configData);
	~GraphicsManager();

LN_INTERNAL_ACCESS:
	Device::IGraphicsDevice* GetGraphicsDevice() { return m_graphicsDevice; }
	RenderingThread* GetRenderingThread() { return m_renderingThread; }
	bool IsPlatformTextureLoading() { return m_platformTextureLoading; }
	RenderingCommandList* GetPrimaryRenderingCommandList();
	PainterEngine* GetPainterEngine() { return m_painterEngine; }
	Device::ITexture* GetDummyTexture() { return m_dummyTexture; }

private:
	FileManager*					m_fileManager;
	RefPtr<FontManager>				m_fontManager;
	RenderingType					m_renderingType;
	RefPtr<CacheManager>			m_glyphTextureCache;
	
	Device::IGraphicsDevice*		m_graphicsDevice;
	RefPtr<SwapChain>				m_mainSwapChain;
	Details::Renderer*				m_renderer;
	RenderingThread*				m_renderingThread;

	PainterEngine*					m_painterEngine;
	Device::ITexture*				m_dummyTexture;
	bool							m_platformTextureLoading;
};

LN_NAMESPACE_GRAPHICS_END
} // namespace Lumino
