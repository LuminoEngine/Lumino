
#pragma once
#include "Common.h"
#include "../Graphics/Bitmap.h"
#include "GraphicsResourceObject.h"

namespace Lumino
{
LN_NAMESPACE_GRAPHICS_BEGIN

/**
	@brief		テクスチャのクラスです。
*/
class Texture
	: public GraphicsResourceObject
{
public:

	/**
		@brief		指定したサイズのテクスチャを作成します。
		@param[in]	size		: テクスチャサイズ (ピクセル単位)
		@param[in]	mipLevels	: ミップマップレベル (0 を指定すると、1x1 までのすべてのミップマップテクスチャを作成する)
		@param[in]	format		: テクスチャのピクセルフォーマット
		@param[in]	manager		: 作成に使用する GraphicsManager
	*/
	static Texture* Create(const Size& size, TextureFormat format = TextureFormat_R8G8B8A8, int mipLevels = 1, GraphicsManager* manager = NULL);

	/**
		@brief		ファイルからテクスチャを作成します。
		@param[in]	filePath	: ファイルパス
		@param[in]	mipLevels	: ミップマップレベル (0 を指定すると、1x1 までのすべてのミップマップテクスチャを作成する)
		@param[in]	format		: テクスチャのピクセルフォーマット
		@param[in]	manager		: 作成に使用する GraphicsManager
	*/
	static Texture* Create(const TCHAR* filePath, TextureFormat format = TextureFormat_R8G8B8A8, int mipLevels = 1, GraphicsManager* manager = NULL);

	/**
		@brief		画像ファイルデータのストリームを指定してテクスチャを作成します。
		@param[in]	stream		: 画像ファイルデータのストリーム
		@param[in]	mipLevels	: ミップマップレベル (0 を指定すると、1x1 までのすべてのミップマップテクスチャを作成する)
		@param[in]	format		: テクスチャのピクセルフォーマット
		@param[in]	manager		: 作成に使用する GraphicsManager
	*/
	static Texture* Create(Stream* stream, TextureFormat format = TextureFormat_R8G8B8A8, int mipLevels = 1, GraphicsManager* manager = NULL);

	/**
		@brief		メモリ上に展開された画像ファイルデータからテクスチャを作成します。
		@param[in]	data		: 画像ファイルデータの先頭アドレス
		@param[in]	size		: データのバイト数
		@param[in]	mipLevels	: ミップマップレベル (0 を指定すると、1x1 までのすべてのミップマップテクスチャを作成する)
		@param[in]	format		: テクスチャのピクセルフォーマット
		@param[in]	manager		: 作成に使用する GraphicsManager
	*/
	static Texture* Create(const void* data, size_t size, TextureFormat format = TextureFormat_R8G8B8A8, int mipLevels = 1, GraphicsManager* manager = NULL);

	/**
		@brief		深度バッファを作成します。
		@param[in]	size		: テクスチャのサイズ (ピクセル単位)
		@param[in]	format		: テクスチャのピクセルフォーマット
	*/
	static Texture* CreateDepthBuffer(const Size& size, TextureFormat format = TextureFormat_R8G8B8A8);
	
	/**
		@brief		深度バッファを作成します。
		@param[in]	size		: テクスチャのサイズ (ピクセル単位)
		@param[in]	format		: テクスチャのピクセルフォーマット
		@details	この関数はデフォルト以外の GraphicsManager を指定して作成する場合に使用します。
	*/
	static Texture* CreateDepthBuffer(GraphicsManager* manager, const Size& size, TextureFormat format = TextureFormat_R8G8B8A8);

public:

	/**
		@brief		テクスチャのサイズを取得します。
		@return		テクスチャのサイズ (ピクセル単位)
	*/
	const Size& GetSize() const;
	
	/**
		@brief		テクスチャの実サイズを取得します。
		@return		テクスチャの実サイズ (ピクセル単位)
	*/
	const Size& GetRealSize() const;

	/**
		@brief		テクスチャのピクセルフォーマットを取得します。
	*/
	TextureFormat GetFormat() const;
	
	/*
		@brief		
	*/
	void SetSubData(const Point& offset, Bitmap* bitmap);
	void SetSubData(const Point& offset, const void* data);

	Bitmap* Lock();
	void Unlock();
	//Device::ITexture* GetDeviceObject() const { return m_deviceObj; }

protected:
	Texture(GraphicsManager* manager);
	Texture(GraphicsManager* manager, Driver::ITexture* deviceObj, Bitmap* primarySurface = NULL);
	Texture(GraphicsManager* manager, bool isDefaultBackBuffer);
	virtual ~Texture();
	void FlushPrimarySurface();
	virtual void OnChangeDevice(Driver::IGraphicsDevice* device);

LN_INTERNAL_ACCESS:
	void AttachDefaultBackBuffer(Driver::ITexture* deviceObj);
	void DetachDefaultBackBuffer();

protected:
	friend struct SetRenderTargetCommand;	// TODO: ダサイ
	friend struct SetDepthBufferCommand;
	friend struct ReadLockTextureCommand;
	friend struct ReadUnlockTextureCommand;
	friend struct PresentCommand;	// TODO
	friend class ShaderVariable;
	friend class SwapChain;
	GraphicsManager*	m_manager;
	Driver::ITexture*	m_deviceObj;
	Bitmap*				m_primarySurface;
	bool				m_isDefaultBackBuffer;
	//bool				m_primarySurfaceModified;

	friend class Helper;
};


/**
	@brief		レンダリングターゲットのクラスです。
*/
class RenderTarget
	: public Texture
{
public:

	/**
		@brief		レンダリングターゲットを作成します。
		@param[in]	size		: レンダリングターゲットのサイズ (ピクセル単位)
		@param[in]	mipLevels	: ミップマップレベル (0 を指定すると、1x1 までのすべてのミップマップテクスチャを作成する)
		@param[in]	format		: テクスチャのピクセルフォーマット
	*/
	static Texture* Create(const Size& size, int mipLevels = 1, TextureFormat format = TextureFormat_R8G8B8A8);

protected:
	RenderTarget(GraphicsManager* manager, const Size& size, int mipLevels, TextureFormat format);
	virtual ~RenderTarget();

private:
};

LN_NAMESPACE_GRAPHICS_END
} // namespace Lumino
