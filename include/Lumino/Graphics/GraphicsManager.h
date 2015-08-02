
#pragma once
#include <Lumino/Base/Cache.h>
#include "Common.h"
#include "../Imaging/FontManager.h"
#include "../Imaging/Font.h"
#include "GraphicsDevice.h"
#include "SwapChain.h"

namespace Lumino
{
namespace Graphics
{
class Renderer;
class RenderingThread;
class PainterEngine;
class TextRenderer;
class FontGlyphTextureCache;

struct GraphicsManagerConfigData
{
	GraphicsAPI				GraphicsAPI;
	Platform::Window*		MainWindow;
	Lumino::FileManager*	FileManager;
	bool					PlatformTextureLoading;

	GraphicsManagerConfigData()
		: GraphicsAPI(GraphicsAPI::DirectX9)
		, MainWindow(NULL)
		, FileManager(NULL)
		, PlatformTextureLoading(false)
	{}
};

/**
	@brief		グラフィックス機能の管理クラス
*/
class GraphicsManager
	: public RefObject
{
public:
	GraphicsManager(const GraphicsManagerConfigData& configData);
	~GraphicsManager();

public:

	/**
		@brief		現在のグラフィックスシステムが使用している API の種類を確認します。
	*/
	GraphicsAPI GetGraphicsAPI() const;


	RenderingType GetRenderingType() const { return RenderingType_Deferred; }
	Device::IGraphicsDevice* GetGraphicsDevice() const { return m_graphicsDevice; }
	Renderer* GetRenderer() { return m_renderer; }
	SwapChain* GetMainSwapChain() { return m_mainSwapChain; }
	Imaging::FontManager* GetFontManager() const { return m_fontManager; }
	RenderingCommandList* GetPrimaryRenderingCommandList();

	PainterEngine* GetPainterEngine() { return m_painterEngine; }
	bool IsPlatformTextureLoading() const { return m_platformTextureLoading; }

	Device::ITexture* GetDummyTexture() { return m_dummyTexture; }

	/// (GraphicsDevice を作成したスレッドと同じスレッドで呼び出す)
	void PauseDevice();
	/// (GraphicsDevice を作成したスレッドと同じスレッドで呼び出す)
	void ResumeDevice();

public:	// internal
	struct FontData
	{
		String	Family;
		int		Size;
		int		EdgeSize;
		bool	IsBold;
		bool	IsItalic;
		bool	IsAntiAlias;

		Imaging::Font* CreateFontFromData(Imaging::FontManager* m) const
		{
			Imaging::Font* font = Imaging::Font::Create(m);
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

	FontGlyphTextureCache* LookupGlyphTextureCache(const FontData& fontData);
	FontGlyphTextureCache* LookupGlyphTextureCache(Imaging::Font* font);

private:
	FileManager* GetFileManager() { return m_fileManager; }

private:
	friend class SwapChain;
	friend class Renderer;
	friend class Texture;

	FileManager*			m_fileManager;
	RefPtr<Imaging::FontManager>	m_fontManager;
	
	//RefPtr<Device::IGraphicsDevice>	m_graphicsDevice;
	Device::IGraphicsDevice*	m_graphicsDevice;
	RefPtr<SwapChain>		m_mainSwapChain;
	Device::ITexture*		m_dummyTexture;		///< public にはしないので RefPtr は使えない

	Renderer*				m_renderer;
	RenderingThread*		m_renderingThread;

	PainterEngine*			m_painterEngine;
	bool					m_platformTextureLoading;

	RefPtr<CacheManager>	m_glyphTextureCache;

};

} // namespace Graphics
} // namespace Lumino
