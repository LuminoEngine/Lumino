
#pragma once
#include "Common.h"
#include "../Graphics/Bitmap.h"
#include "GraphicsResourceObject.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

/**
	@brief		テクスチャのクラスです。
*/
class Texture
	: public GraphicsResourceObject
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	/**
		@brief		テクスチャのサイズを取得します。
		@return		テクスチャのサイズ (ピクセル単位)
	*/
	const Size& GetSize() const;

	/**
		@brief		テクスチャの幅を取得します。
		@return		テクスチャの幅 (ピクセル単位)
	*/
	int GetWidth() const;

	/**
		@brief		テクスチャの高さを取得します。
		@return		テクスチャの高さ (ピクセル単位)
	*/
	int GetHeight() const;

	/**
		@brief		テクスチャの実サイズを取得します。
		@return		テクスチャの実サイズ (ピクセル単位)
	*/
	const Size& GetRealSize() const;

	/**
		@brief		テクスチャのピクセルフォーマットを取得します。
	*/
	TextureFormat GetFormat() const;

	Bitmap* Lock();
	void Unlock();

protected:
	Texture();
	virtual ~Texture();

LN_INTERNAL_ACCESS:
	Driver::ITexture* GetDeviceObject() const { return m_deviceObj; }

protected:
	friend struct ReadLockTextureCommand;
	friend struct ReadUnlockTextureCommand;
	Driver::ITexture*	m_deviceObj;
	Size				m_size;
	TextureFormat		m_format;
	Bitmap*				m_primarySurface;
};

/**
	@brief		テクスチャのクラスです。
*/
class Texture2D
	: public Texture
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	/**
		@brief		指定したサイズのテクスチャを作成します。
		@param[in]	width		: テクスチャの幅 (ピクセル単位)
		@param[in]	height		: テクスチャの高さ (ピクセル単位)
		@param[in]	mipLevels	: ミップマップレベル (0 を指定すると、1x1 までのすべてのミップマップテクスチャを作成する)
		@param[in]	format		: テクスチャのピクセルフォーマット
	*/
	static Texture2DPtr Create(int width, int height, TextureFormat format = TextureFormat_R8G8B8A8, int mipLevels = 1);
	
	/**
		@brief		指定したサイズのテクスチャを作成します。
		@param[in]	size		: テクスチャサイズ (ピクセル単位)
		@param[in]	mipLevels	: ミップマップレベル (0 を指定すると、1x1 までのすべてのミップマップテクスチャを作成する)
		@param[in]	format		: テクスチャのピクセルフォーマット
	*/
	static Texture2DPtr Create(const Size& size, TextureFormat format = TextureFormat_R8G8B8A8, int mipLevels = 1);

	/**
		@brief		ファイルからテクスチャを作成します。
		@param[in]	filePath	: ファイルパス
		@param[in]	mipLevels	: ミップマップレベル (0 を指定すると、1x1 までのすべてのミップマップテクスチャを作成する)
		@param[in]	format		: テクスチャのピクセルフォーマット
	*/
	static Texture2DPtr Create(const StringRef& filePath, TextureFormat format = TextureFormat_R8G8B8A8, int mipLevels = 1);

	/**
		@brief		画像ファイルデータのストリームを指定してテクスチャを作成します。
		@param[in]	stream		: 画像ファイルデータのストリーム
		@param[in]	mipLevels	: ミップマップレベル (0 を指定すると、1x1 までのすべてのミップマップテクスチャを作成する)
		@param[in]	format		: テクスチャのピクセルフォーマット
	*/
	static Texture2DPtr Create(Stream* stream, TextureFormat format = TextureFormat_R8G8B8A8, int mipLevels = 1);

	/**
		@brief		メモリ上に展開された画像ファイルデータからテクスチャを作成します。
		@param[in]	data		: 画像ファイルデータの先頭アドレス
		@param[in]	size		: データのバイト数
		@param[in]	mipLevels	: ミップマップレベル (0 を指定すると、1x1 までのすべてのミップマップテクスチャを作成する)
		@param[in]	format		: テクスチャのピクセルフォーマット
	*/
	static Texture2DPtr Create(const void* data, size_t size, TextureFormat format = TextureFormat_R8G8B8A8, int mipLevels = 1);

public:

#pragma push_macro("DrawText")
#undef DrawText
	void DrawText(const StringRef& text, const Rect& rect, Font* font, const Color& color, TextAlignment align);
	void LN_AFX_FUNCNAME(DrawText)(const StringRef& text, const Rect& rect, Font* font, const Color& color, TextAlignment align);
#pragma pop_macro("DrawText")
	
	/*
		@brief		
	*/
	void SetSubData(const Point& offset, Bitmap* bitmap);
	void SetSubData(const Point& offset, const void* data);

	//Device::ITexture* GetDeviceObject() const { return m_deviceObj; }

LN_PROTECTED_INTERNAL_ACCESS:
	Texture2D();
	//Texture(GraphicsManager* manager, const Size& size, TextureFormat format, int mipLevels, Bitmap* primarySurface);
	//Texture(GraphicsManager* manager, Stream* stream, TextureFormat format, int mipLevels);
	////Texture(GraphicsManager* manager, Driver::ITexture* deviceObj, Bitmap* primarySurface = NULL);
	//Texture(GraphicsManager* manager, bool isDefaultBackBuffer);
	virtual ~Texture2D();
	void FlushPrimarySurface();
	virtual void OnChangeDevice(Driver::IGraphicsDevice* device);

LN_INTERNAL_ACCESS:
	void CreateCore(GraphicsManager* manager, const Size& size, TextureFormat format, int mipLevels, Bitmap* primarySurface);
	void CreateCore(GraphicsManager* manager, const StringRef& filePath, TextureFormat format, int mipLevels);
	void CreateCore(GraphicsManager* manager, Stream* stream, TextureFormat format, int mipLevels);
	void CreateCore(GraphicsManager* manager, bool isDefaultBackBuffer);
	Driver::ITexture* GetDeviceObject() const { return m_deviceObj; }

protected:
	friend struct SetRenderTargetCommand;	// TODO: ダサイ
	friend struct SetDepthBufferCommand;
	friend struct PresentCommand;	// TODO
	friend class ShaderVariable;
	int					m_mipLevels;
	bool				m_isPlatformLoaded;
	//bool				m_primarySurfaceModified;

	friend class Helper;
};


/**
	@brief		レンダリングターゲットのクラスです。
*/
class RenderTarget		// TODO: RenderTargetTexture
	: public Texture
{
public:

	/**
		@brief		レンダリングターゲットを作成します。
		@param[in]	size		: レンダリングターゲットのサイズ (ピクセル単位)
		@param[in]	format		: テクスチャのピクセルフォーマット
		@param[in]	mipLevels	: ミップマップレベル (0 を指定すると、1x1 までのすべてのミップマップテクスチャを作成する)
	*/
	static RenderTargetPtr Create(const Size& size, TextureFormat format = TextureFormat_R8G8B8A8, int mipLevels = 1);

LN_INTERNAL_ACCESS:
	RenderTarget();
	void CreateImpl(GraphicsManager* manager, const Size& size, int mipLevels, TextureFormat format);
	void CreateCore(GraphicsManager* manager, bool isDefaultBackBuffer);
	void AttachDefaultBackBuffer(Driver::ITexture* deviceObj);
	void DetachDefaultBackBuffer();

protected:
	virtual ~RenderTarget();
	virtual void OnChangeDevice(Driver::IGraphicsDevice* device);

private:
	friend class SwapChain;
	friend class detail::RenderTargetTextureCache;
	//Size			m_size;
	int				m_mipLevels;
	//TextureFormat	m_format;
	bool				m_isDefaultBackBuffer;

	bool			m_usedCacheOnFrame;
};

/**
	@brief		深度バッファのクラスです。
*/
class DepthBuffer
	: public Texture
{
public:

	/**
		@brief		深度バッファを作成します。
		@param[in]	size		: テクスチャのサイズ (ピクセル単位)
		@param[in]	format		: ピクセルフォーマット
	*/
	static Texture* Create(const Size& size, TextureFormat format = TextureFormat_D24S8);

LN_INTERNAL_ACCESS:
	DepthBuffer();
	void CreateImpl(GraphicsManager* manager, const Size& size, TextureFormat format);

protected:
	virtual ~DepthBuffer();
	virtual void OnChangeDevice(Driver::IGraphicsDevice* device);

private:
	//Size			m_size;
	//TextureFormat	m_format;
};

class ScopedLockTexture
{
public:
	ScopedLockTexture(Texture* texture)
	{
		m_texture = texture;
		m_bitmap = m_texture->Lock();
	}
	~ScopedLockTexture()
	{
		m_texture->Unlock();
	}

	Bitmap* GetBitmap() const { return m_bitmap; }
private:
	Texture*	m_texture;
	Bitmap*		m_bitmap;
};

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
