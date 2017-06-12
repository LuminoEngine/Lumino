
#pragma once
#include <Lumino/Base/Cache.h>
#include "Common.h"
#include "GraphicsResourceObject.h"

#pragma push_macro("DrawText")
#undef DrawText

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN
namespace detail { class RenderTargetTextureCache; }
class Bitmap;
class Font;
class Texture;
class DepthBuffer;
using TexturePtr = RefPtr<Texture>;
using DepthBufferPtr = RefPtr<DepthBuffer>;

/**
	@brief		テクスチャのクラスです。
*/
LN_CLASS()
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

protected:
	virtual void Dispose() override;

LN_INTERNAL_ACCESS:
	Driver::ITexture* GetDeviceObjectConst() const { return m_deviceObj; }
	Driver::ITexture* ResolveDeviceObject() { ApplyModifies(); return m_deviceObj; }
	const SizeI& getSize() const;

protected:
	Texture();
	virtual ~Texture();
	virtual void ApplyModifies();

	friend struct ReadLockTextureCommand;
	friend struct ReadUnlockTextureCommand;
	Driver::ITexture*	m_deviceObj;
	SizeI				m_size;
	TextureFormat		m_format;
	Bitmap*				m_primarySurface;
};

/**
	@brief		2D テクスチャのクラスです。
*/
LN_CLASS()
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
	
	/**
		@brief		黒い小さなテクスチャを取得します。
		@detail		このテクスチャの全ピクセルは不透明な白 Color(0, 0, 0, 1) です。
	*/
	static Texture2DPtr GetBlackTexture();
	
	/**
		@brief		白い小さなテクスチャを取得します。
		@detail		このテクスチャの全ピクセルは不透明な白 Color(1, 1, 1, 1) です。
	*/
	static Texture2DPtr GetWhiteTexture();

public:

	/** 指定した色でテクスチャ全体を塗りつぶします。*/
	void clear(const Color32& color);

	void SetPixel(int x, int y, const Color& color);

	void Blit(int x, int y, Texture2D* srcTexture, const RectI& srcRect);	// TODO: アルファブレンド有無
	
	void Blt(int x, int y, Bitmap* srcBitmap/*, const RectI& srcRect*/);


	void DrawText(const StringRef& text, const RectI& rect, Font* font, const Color32& fillColor, const Color32& strokeColor, int strokeThickness, TextAlignment alignment);
	void LN_AFX_FUNCNAME(DrawText)(const StringRef& text, const RectI& rect, Font* font, const Color32& fillColor, const Color32& strokeColor, int strokeThickness, TextAlignment alignment);
	// TODO: ↑ TextAlignment じゃなくて TextLayoutFlags の方が良いと思う

LN_PROTECTED_INTERNAL_ACCESS:
	void FlushPrimarySurface();
	virtual void OnChangeDevice(Driver::IGraphicsDevice* device);

LN_CONSTRUCT_ACCESS:
	Texture2D();
	virtual ~Texture2D();
	//LN_METHOD()
	void initialize(const SizeI& size, TextureFormat format, bool mipmap, ResourceUsage usage);
	LN_METHOD()
	void initialize(int width, int height, TextureFormat format = TextureFormat::R8G8B8A8, bool mipmap = false);
	LN_METHOD()
	void initialize(const StringRef& filePath, TextureFormat format = TextureFormat::R8G8B8A8, bool mipmap = false);
	void initialize(Stream* stream, TextureFormat format, bool mipmap);

LN_INTERNAL_ACCESS:
	void TryLock();
	void SetSubData(const PointI& offset, Bitmap* bitmap);
	void SetData(const void* data);

protected:
	virtual void ApplyModifies() override;

	friend struct PresentCommand;
	bool			m_mipmap;
	bool			m_isPlatformLoaded;
	ResourceUsage	m_usage;
	bool			m_usageReadFast;
	RefPtr<Bitmap>	m_primarySurface2;
	bool			m_locked;
	bool			m_initializing;
};

/**
	@brief		レンダリングターゲットテクスチャのクラスです。
*/
class RenderTargetTexture
	: public Texture
{
public:

	/**
		@brief		レンダリングターゲットを作成します。
		@param[in]	size		: レンダリングターゲットのサイズ (ピクセル単位)
		@param[in]	format		: テクスチャのピクセルフォーマット
		@param[in]	mipLevels	: ミップマップレベル (0 を指定すると、1x1 までのすべてのミップマップテクスチャを作成する)
	*/
	static RenderTargetTexturePtr Create(const SizeI& size, TextureFormat format = TextureFormat::R8G8B8A8, int mipLevels = 1);

LN_INTERNAL_ACCESS:
	RenderTargetTexture();
	virtual ~RenderTargetTexture();
	void initialize(const SizeI& size, int mipLevels, TextureFormat format);
	void CreateImpl(detail::GraphicsManager* manager, const SizeI& size, int mipLevels, TextureFormat format);
	void CreateCore(detail::GraphicsManager* manager, bool isDefaultBackBuffer);
	void AttachDefaultBackBuffer(Driver::ITexture* deviceObj);
	void DetachDefaultBackBuffer();
	Bitmap* Lock();
	void Unlock();
	virtual void OnChangeDevice(Driver::IGraphicsDevice* device);

private:
	int				m_mipLevels;
	bool			m_isDefaultBackBuffer;
	bool			m_usedCacheOnFrame;

	friend class detail::RenderTargetTextureCache;
};

/**
	@brief		深度バッファのクラスです。
*/
class DepthBuffer
	: public GraphicsResourceObject
{
public:

	/**
		@brief		深度バッファを作成します。
		@param[in]	size		: テクスチャのサイズ (ピクセル単位)
		@param[in]	format		: ピクセルフォーマット
	*/
	static DepthBufferPtr Create(const SizeI& size, TextureFormat format = TextureFormat::D24S8);

	const SizeI& getSize() const { return m_size; }

protected:
	virtual void Dispose() override;

LN_INTERNAL_ACCESS :
	DepthBuffer();
	virtual ~DepthBuffer();
	void CreateImpl(detail::GraphicsManager* manager, const SizeI& size, TextureFormat format);
	Driver::ITexture* ResolveDeviceObject() const { return m_deviceObj; }
	void resize(const SizeI& newSize);
	virtual void OnChangeDevice(Driver::IGraphicsDevice* device);

private:
	void RefreshDeviceResource();

	Driver::ITexture*	m_deviceObj;
	SizeI				m_size;
	TextureFormat		m_format;
};

namespace tr {
class Texture3D;
using Texture3DPtr = RefPtr<Texture3D>;

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
	void initialize(ln::detail::GraphicsManager* manager, int width, int height, int depth, TextureFormat format, int mipLevels, ResourceUsage usage);

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

} // namespace tr

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END

#pragma pop_macro("DrawText")
