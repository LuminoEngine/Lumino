
#pragma once
#include <Lumino/Base/Cache.h>
#include "Common.h"
#include "GraphicsResourceObject.h"

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace detail { class RenderTargetTextureCache; }
class Bitmap;
class Font;
class Texture;
class Texture3D;
using TexturePtr = RefPtr<Texture>;
using Texture3DPtr = RefPtr<Texture3D>;

/**
	@brief		テクスチャのクラスです。
*/
class Texture
	: public GraphicsResourceObject
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

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
		@brief		テクスチャのピクセルフォーマットを取得します。
	*/
	TextureFormat GetFormat() const;

	Bitmap* Lock();
	void Unlock();

protected:
	Texture();
	virtual ~Texture();

LN_INTERNAL_ACCESS:
	Driver::ITexture* GetDeviceObjectConst() const { return m_deviceObj; }
	Driver::ITexture* ResolveDeviceObject() { ApplyModifies(); return m_deviceObj; }
	const SizeI& GetSize() const;
	const SizeI& GetRealSize() const;

protected:
	friend struct ReadLockTextureCommand;
	friend struct ReadUnlockTextureCommand;
	Driver::ITexture*	m_deviceObj;
	SizeI				m_size;
	TextureFormat		m_format;
	Bitmap*				m_primarySurface;
};

/**
	@brief		テクスチャのクラスです。
*/
class Texture2D
	: public Texture
	, public ICacheObject
{
	LN_CACHE_OBJECT_DECL;
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	/**
		@brief		指定したサイズのテクスチャを作成します。
		@param[in]	width		: テクスチャの幅 (ピクセル単位)
		@param[in]	height		: テクスチャの高さ (ピクセル単位)
		@param[in]	format		: テクスチャのピクセルフォーマット
		@param[in]	mipmap		: ミップマップの有無
	*/
	static Texture2DPtr Create(int width, int height, TextureFormat format = TextureFormat::R8G8B8A8, bool mipmap = false);
	// TODO: mipMap は unity のように、有無だけで指定したい
	
	/**
		@brief		指定したサイズのテクスチャを作成します。
		@param[in]	size		: テクスチャサイズ (ピクセル単位)
		@param[in]	format		: テクスチャのピクセルフォーマット
		@param[in]	mipmap		: ミップマップの有無
	*/
	static Texture2DPtr Create(const SizeI& size, TextureFormat format = TextureFormat::R8G8B8A8, bool mipmap = false);

	/**
		@brief		ファイルからテクスチャを作成します。
		@param[in]	filePath	: ファイルパス
		@param[in]	format		: テクスチャのピクセルフォーマット
		@param[in]	mipmap		: ミップマップの有無
	*/
	static Texture2DPtr Create(const StringRef& filePath, TextureFormat format = TextureFormat::R8G8B8A8, bool mipmap = false);

	/**
		@brief		画像ファイルデータのストリームを指定してテクスチャを作成します。
		@param[in]	stream		: 画像ファイルデータのストリーム
		@param[in]	format		: テクスチャのピクセルフォーマット
		@param[in]	mipmap		: ミップマップの有無
	*/
	static Texture2DPtr Create(Stream* stream, TextureFormat format = TextureFormat::R8G8B8A8, bool mipmap = false);

	/**
		@brief		メモリ上に展開された画像ファイルデータからテクスチャを作成します。
		@param[in]	data		: 画像ファイルデータの先頭アドレス
		@param[in]	size		: データのバイト数
		@param[in]	format		: テクスチャのピクセルフォーマット
		@param[in]	mipmap		: ミップマップの有無
	*/
	static Texture2DPtr Create(const void* data, size_t size, TextureFormat format = TextureFormat::R8G8B8A8, bool mipmap = false);

	static Texture2DPtr GetWhiteTexture();

public:

	void Clear(const Color32& color);

	void SetPixel(int x, int y, const Color& color);

	//void Blt(int x, int y, Texture* srcTexture, const Rect& srcRect);	// TODO: アルファブレンド有無
	
	void Blt(int x, int y, Bitmap* srcBitmap/*, const Rect& srcRect*/);

#pragma push_macro("DrawText")
#undef DrawText
	void DrawText(const StringRef& text, const Rect& rect, Font* font, const Color32& fillColor, const Color32& strokeColor, int strokeThickness, TextAlignment alignment);
	void LN_AFX_FUNCNAME(DrawText)(const StringRef& text, const Rect& rect, Font* font, const Color32& fillColor, const Color32& strokeColor, int strokeThickness, TextAlignment alignment);
	// TODO: ↑ TextAlignment じゃなくて TextLayoutFlags の方が良いと思う
#pragma pop_macro("DrawText")
	
	/*
		@brief		
	*/
	void SetSubData(const PointI& offset, Bitmap* bitmap);
	void SetSubData(const PointI& offset, const void* data);

	//Device::ITexture* GetDeviceObject() const { return m_deviceObj; }

LN_PROTECTED_INTERNAL_ACCESS:
	Texture2D();
	virtual ~Texture2D();
	void FlushPrimarySurface();
	virtual void OnChangeDevice(Driver::IGraphicsDevice* device);

LN_INTERNAL_ACCESS:
	void Initialize(detail::GraphicsManager* manager, const SizeI& size, TextureFormat format, bool mipmap, ResourceUsage usage);
	void Initialize(detail::GraphicsManager* manager, const StringRef& filePath, TextureFormat format, bool mipmap);
	void Initialize(detail::GraphicsManager* manager, Stream* stream, TextureFormat format, bool mipmap);
	void Initialize(detail::GraphicsManager* manager, bool isDefaultBackBuffer);
	void TryLock();
	//Driver::ITexture* GetDeviceObject() const { return m_deviceObj; }

protected:
	virtual void ApplyModifies() override;

	friend struct SetRenderTargetCommand;	// TODO
	friend struct SetDepthBufferCommand;
	friend struct PresentCommand;	// TODO
	friend class ShaderVariable;
	bool			m_mipmap;
	bool			m_isPlatformLoaded;

	ResourceUsage	m_usage;
	RefPtr<Bitmap>	m_primarySurface2;
	bool			m_locked;
	bool			m_initializing;

	friend class Helper;
};

/**
	@brief		
*/
class Texture3D
	: public Texture
	, public ICacheObject
{
	LN_CACHE_OBJECT_DECL;
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:

	/**
		@brief		指定したサイズの 3D テクスチャを作成します。
		@param[in]	width		: テクスチャの幅 (ピクセル単位)
		@param[in]	height		: テクスチャの高さ (ピクセル単位)
		@param[in]	depth		: テクスチャの奥行き (ピクセル単位)
		@param[in]	mipLevels	: ミップマップレベル (0 を指定すると、1x1 までのすべてのミップマップテクスチャを作成する)
		@param[in]	format		: テクスチャのピクセルフォーマット
	*/
	static Texture3DPtr Create(int width, int height, int depth, TextureFormat format = TextureFormat::R8G8B8A8, int mipLevels = 1, ResourceUsage usage = ResourceUsage::Static);

public:
	
	/**
		@brief		テクスチャの奥行きを取得します。
	*/
	int GetDepth() const;

	void SetPixel32(int x, int y, int z, const Color32& color);

LN_PROTECTED_INTERNAL_ACCESS:
	Texture3D();
	virtual ~Texture3D();
	void Initialize(detail::GraphicsManager* manager, int width, int height, int depth, TextureFormat format, int mipLevels, ResourceUsage usage);

protected:
	virtual void ApplyModifies() override;
	virtual void OnChangeDevice(Driver::IGraphicsDevice* device) override;

LN_INTERNAL_ACCESS:
	void TryLock();
	//Driver::ITexture* GetDeviceObject() const { return m_deviceObj; }

private:
	int				m_depth;
	int				m_mipLevels;
	ResourceUsage	m_usage;
	RefPtr<Bitmap>	m_primarySurface;
	bool			m_locked;
	bool			m_initializing;
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
	static RenderTargetPtr Create(const SizeI& size, TextureFormat format = TextureFormat::R8G8B8A8, int mipLevels = 1);

LN_INTERNAL_ACCESS:
	RenderTarget();
	void CreateImpl(detail::GraphicsManager* manager, const SizeI& size, int mipLevels, TextureFormat format);
	void CreateCore(detail::GraphicsManager* manager, bool isDefaultBackBuffer);
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
	: public Texture	// TODO: 震度バッファはテクスチャ扱いしないほうがいい
{
public:

	/**
		@brief		深度バッファを作成します。
		@param[in]	size		: テクスチャのサイズ (ピクセル単位)
		@param[in]	format		: ピクセルフォーマット
	*/
	static Texture* Create(const SizeI& size, TextureFormat format = TextureFormat::D24S8);

	void Resize(const SizeI& newSize);

LN_INTERNAL_ACCESS:
	DepthBuffer();
	void CreateImpl(detail::GraphicsManager* manager, const SizeI& size, TextureFormat format);

protected:
	virtual ~DepthBuffer();
	virtual void OnChangeDevice(Driver::IGraphicsDevice* device);

private:
	void RefreshDeviceResource();
};

class ScopedTextureLock
{
public:
	ScopedTextureLock(Texture* texture)
	{
		m_texture = texture;
		m_bitmap = m_texture->Lock();
	}
	~ScopedTextureLock()
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
