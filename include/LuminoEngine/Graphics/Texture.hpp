#pragma once
#include "../Font/Common.hpp"
#include "GraphicsResource.hpp"
#include "ColorStructs.hpp"
#include "GeometryStructs.hpp"

namespace ln {
class SamplerState;
class ShaderPass;
namespace detail { class ITexture; }
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
	int width() const { return m_size.width; }

	/** テクスチャの高さを取得します。 (ピクセル単位) */
	int height() const { return m_size.height; }

	/** テクスチャのピフォーマットを取得します。 */
	TextureFormat format() const { return m_format; }

	bool mipmap() const { return m_mipmap; }

	/** このテクスチャに関連付けられている SamplerState を取得します。 */
	SamplerState* samplerState() const;

	/** このテクスチャに関連付ける SamplerState を設定します。 */
	void setSamplerState(SamplerState* value);

    // TODO: internal
    const SizeI& size() const { return m_size; }

protected:
	Texture();
	virtual ~Texture();
	void init();
	virtual detail::ITexture* resolveRHIObject() = 0;
	void setSize(const SizeI& size) { m_size = size; }
	void setFormat(TextureFormat format) { m_format = format; }
	void setMipmap(bool mipmap) { m_mipmap = mipmap; }

	SizeI m_size;   // TODO: リアルタイムな部分で使うのは float が多い。対して、blit など lumino としてはあまりリアルタイムにしてほしくない部分は int がおおい。ので、float にしたい。
	TextureFormat m_format;
	Ref<SamplerState> m_samplerState;
	bool m_mipmap;

	friend class ShaderPass;
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
	virtual detail::ITexture* resolveRHIObject() override;

private:
	//void resizeInternal(int width, int height);

	Ref<detail::ITexture> m_rhiObject;
	GraphicsResourceUsage m_usage;
	GraphicsResourcePool m_pool;

	Ref<Bitmap2D> m_bitmap;
	void* m_rhiLockedBuffer;
	bool m_initialUpdate;
	bool m_modified;

	friend class detail::GraphicsResourceHelper;
};


/** 3D テクスチャのクラスです。 */
LN_CLASS()
class Texture3D
	: public Texture
{
public:
	/** テクスチャの奥行きを取得します。 (ピクセル単位) */
	int depth() const { return m_depth; }

	/** テクスチャが保持するビットマップデータにアクセスします。 */
	Bitmap3D* map(MapMode mode);

	/** リソースの管理方法を変更します。(default: Managed) */
	void setResourcePool(GraphicsResourcePool pool);

LN_CONSTRUCT_ACCESS:
	Texture3D();
	virtual ~Texture3D();
	void init(int width, int height, int depth, TextureFormat format = TextureFormat::RGBA32, bool mipmap = false, GraphicsResourceUsage usage = GraphicsResourceUsage::Static);

protected:
	virtual void onDispose(bool explicitDisposing) override;
	virtual void onChangeDevice(detail::IGraphicsDevice* device) override;
	virtual detail::ITexture* resolveRHIObject() override;

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
	virtual detail::ITexture* resolveRHIObject() override;
	virtual void onChangeDevice(detail::IGraphicsDevice* device) override;

    void resetSwapchainFrameIfNeeded();
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
};

} // namespace detail
} // namespace ln
