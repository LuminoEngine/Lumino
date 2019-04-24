#pragma once
#include "../Font/Common.hpp"
#include "GraphicsResource.hpp"
#include "ColorStructs.hpp"
#include "GeometryStructs.hpp"

namespace ln {
class SamplerState;
class ShaderPass;
namespace detail {
class ITexture;
class TextureHelper;
}
class SwapChain;
class Bitmap2D;
class Bitmap3D;
class Font;

/** テクスチャのベースクラスです。 */
LN_CLASS()
class Texture
	: public GraphicsResource
{
public:
	/** テクスチャの幅を取得します。(ピクセル単位) */
	int width() const { return m_desc.width; }

	/** テクスチャの高さを取得します。 (ピクセル単位) */
	int height() const { return m_desc.height; }

	/** テクスチャのピフォーマットを取得します。 */
	TextureFormat format() const { return m_desc.format; }

    /** ミップマップの有無を取得します。 */
	bool mipmap() const { return m_desc.mipmap; }

	/** このテクスチャに関連付けられている SamplerState を取得します。 */
	SamplerState* samplerState() const;

	/** このテクスチャに関連付ける SamplerState を設定します。 */
	void setSamplerState(SamplerState* value);

    // TODO: internal
    //SizeI size() const { return SizeI(m_desc.width, m_desc.height); }

protected:
	Texture();
	virtual ~Texture();
	void init();
	virtual detail::ITexture* resolveRHIObject(bool* outModified) = 0;

private:
    detail::TextureDesc m_desc;
	Ref<SamplerState> m_samplerState;

	friend class ShaderPass;
    friend class detail::TextureHelper;
};

/** 2D テクスチャのクラスです。 */
LN_CLASS()
class Texture2D
	: public Texture
{
public:
    static Ref<Texture2D> create(int width, int height, TextureFormat format = TextureFormat::RGBA32, bool mipmap = false, GraphicsResourceUsage usage = GraphicsResourceUsage::Static);


	/** テクスチャが保持するビットマップデータにアクセスします。 */
	Bitmap2D* map(MapMode mode);

	/** リソースの管理方法を変更します。(default: Managed) */
	void setResourcePool(GraphicsResourcePool pool);

    void clear(const Color& color);
    void setPixel(int x, int y, const Color& color);
    void blit(int x, int y, Texture2D* srcTexture, const RectI& srcRect);	// TODO: アルファブレンド有無
    void drawText(const StringRef& text, const Rect& rect, Font* font, const Color& color, TextAlignment alignment = TextAlignment::Left);


LN_CONSTRUCT_ACCESS:
	Texture2D();
	virtual ~Texture2D();
	void init(int width, int height, TextureFormat format = TextureFormat::RGBA32, bool mipmap = false, GraphicsResourceUsage usage = GraphicsResourceUsage::Static);
	void init(const StringRef& filePath, TextureFormat format = TextureFormat::RGBA32, bool mipmap = false, GraphicsResourceUsage usage = GraphicsResourceUsage::Static);
    void init(Stream* stream, TextureFormat format = TextureFormat::RGBA32, bool mipmap = false, GraphicsResourceUsage usage = GraphicsResourceUsage::Static);
    void init(Bitmap2D* bitmap, TextureFormat format = TextureFormat::RGBA32, bool mipmap = false, GraphicsResourceUsage usage = GraphicsResourceUsage::Static);

protected:
	virtual void onDispose(bool explicitDisposing) override;
	virtual void onChangeDevice(detail::IGraphicsDevice* device) override;
	virtual detail::ITexture* resolveRHIObject(bool* outModified) override;

private:
	//void resizeInternal(int width, int height);

	Ref<detail::ITexture> m_rhiObject;
	GraphicsResourceUsage m_usage;
	GraphicsResourcePool m_pool;

	Ref<Bitmap2D> m_bitmap;
	void* m_rhiLockedBuffer;
	bool m_initialUpdate;
	bool m_modified;

	friend class detail::GraphicsResourceInternal;
};



class RenderTargetTexture
	: public Texture
{
public:
	/** 一時的な RenderTargetTexture を取得します。 */
	static Ref<RenderTargetTexture> getTemporary(int width, int height, TextureFormat format, bool mipmap);

	/** getTemporary で取得した一時的な RenderTargetTexture を解放します。 */
	static void releaseTemporary(RenderTargetTexture* renderTarget);

	Ref<Bitmap2D> readData();

	virtual void onDispose(bool explicitDisposing) override;

LN_CONSTRUCT_ACCESS:
	RenderTargetTexture();
	virtual ~RenderTargetTexture();
	void init(int width, int height, TextureFormat requestFormat, bool mipmap);
	void init(SwapChain* owner/*, detail::ITexture* ref*/);

LN_INTERNAL_ACCESS:
	virtual detail::ITexture* resolveRHIObject(bool* outModified) override;
	virtual void onChangeDevice(detail::IGraphicsDevice* device) override;

    void resetSwapchainFrameIfNeeded(bool force = false);
    //void resetSwapchainFrame(detail::ITexture* ref);

private:
	Ref<detail::ITexture> m_rhiObject;
    SwapChain* m_ownerSwapchain;
    int m_swapchainImageIndex;
	//SizeI m_size;
	//TextureFormat m_requestFormat;
	//bool m_mipmap;
};

namespace detail {

class TextureHelper
{
public:
	static void setMappedData(Texture2D* texture, const void* data);
    static void setDesc(Texture* texture, const TextureDesc& desc) { texture->m_desc = desc; }
};

class Texture3D
	: public Texture
{
public:
	int depth() const { return m_depth; }
	Bitmap3D* map(MapMode mode);
	void setResourcePool(GraphicsResourcePool pool);

LN_CONSTRUCT_ACCESS:
	Texture3D();
	virtual ~Texture3D();
	void init(int width, int height, int depth, TextureFormat format = TextureFormat::RGBA32, bool mipmap = false, GraphicsResourceUsage usage = GraphicsResourceUsage::Static);

protected:
	virtual void onDispose(bool explicitDisposing) override;
	virtual void onChangeDevice(detail::IGraphicsDevice* device) override;
	virtual detail::ITexture* resolveRHIObject(bool* outModified) override;

private:
	Ref<detail::ITexture> m_rhiObject;
	GraphicsResourceUsage m_usage;
	GraphicsResourcePool m_pool;

	int m_depth;
	Ref<Bitmap3D> m_bitmap;
	void* m_rhiLockedBuffer;
	bool m_initialUpdate;
	bool m_modified;
};

} // namespace detail
} // namespace ln
