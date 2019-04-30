#pragma once
#include "../Font/Common.hpp"
#include "GraphicsResource.hpp"
#include "ColorStructs.hpp"
#include "GeometryStructs.hpp"

namespace ln {
namespace detail {
class TextureInternal;
}

/** テクスチャのベースクラスです。 */
LN_CLASS()
class Texture
    : public GraphicsResource
{
public:
    /** テクスチャの幅を取得します。(ピクセル単位) */
    int width() const { return m_width; }

    /** テクスチャの高さを取得します。 (ピクセル単位) */
    int height() const { return m_height; }

    /** テクスチャのピフォーマットを取得します。 */
    TextureFormat format() const { return m_format; }

    /** ミップマップの有無を取得します。 */
    bool mipmap() const { return m_mipmap; }

    /** このテクスチャに関連付けられている SamplerState を取得します。 */
    SamplerState* samplerState() const;

    /** このテクスチャに関連付ける SamplerState を設定します。 */
    void setSamplerState(SamplerState* value);

protected:
    Texture();
    virtual ~Texture();
    void init();
    virtual detail::ITexture* resolveRHIObject(bool* outModified) = 0;

private:
    void setDesc(int width, int height, TextureFormat format);

    int m_width;
    int m_height;
    TextureFormat m_format;
    bool m_mipmap;
    Ref<SamplerState> m_samplerState;

    friend class detail::TextureInternal;
    friend class detail::GraphicsResourceInternal;
};

/** 2D テクスチャのクラスです。 */
LN_CLASS()
class Texture2D
    : public Texture
{
public:
    /**
     * テクスチャを作成します。ピクセルフォーマットは RGBA8 です。
     * @param[in]   width   : 幅 (px 単位)
     * @param[in]   height  : 高さ (px 単位)
     */
    static Ref<Texture2D> create(int width, int height);

    /**
     * テクスチャを作成します。
     * @param[in]   width   : 幅 (px 単位)
     * @param[in]   height  : 高さ (px 単位)
     * @param[in]   format  : ピクセルフォーマット
     */
    static Ref<Texture2D> create(int width, int height, TextureFormat format);

public:
    /** Mipmap の有無を設定します。(default: false) */
    void setMipmapEnabled(bool value);

    /** テクスチャが保持するビットマップデータにアクセスします。 */
    Bitmap2D* map(MapMode mode);

    /** リソースの使用方法を変更します。(default: Static) */
    void setResourceUsage(GraphicsResourceUsage usage);

    /** リソースの管理方法を変更します。(default: Managed) */
    void setResourcePool(GraphicsResourcePool pool);

    /** 指定した色でテクスチャ全体をクリアします。 */
    void clear(const Color& color);

    /** 指定した色でテクスチャ全体をクリアします。 */
    void setPixel(int x, int y, const Color& color);

    /** 別のテクスチャからこのテクスチャへ、ビットマップ転送を行います。 */
    void blit(int x, int y, Texture2D* srcTexture, int sx, int sy, int sw, int sh);

    void drawText(const StringRef& text, const Rect& rect, Font* font, const Color& color, TextAlignment alignment = TextAlignment::Left);

protected:
    virtual void onDispose(bool explicitDisposing) override;
    virtual void onChangeDevice(detail::IGraphicsDevice* device) override;
    virtual detail::ITexture* resolveRHIObject(bool* outModified) override;

LN_CONSTRUCT_ACCESS:
    Texture2D();
    virtual ~Texture2D();

    /** @copydoc create(int, int) */
    void init(int width, int height);

    /** @copydoc create(int, int, TextureFormat) */
    void init(int width, int height, TextureFormat format);

    void init(const StringRef& filePath, TextureFormat format = TextureFormat::RGBA8);
    void init(Stream* stream, TextureFormat format = TextureFormat::RGBA8);
    void init(Bitmap2D* bitmap, TextureFormat format = TextureFormat::RGBA8);

private:
    Ref<detail::ITexture> m_rhiObject;
    GraphicsResourceUsage m_usage;
    GraphicsResourcePool m_pool;
    Ref<Bitmap2D> m_bitmap;
    void* m_rhiLockedBuffer;
    bool m_initialUpdate;
    bool m_modified;

    friend class detail::GraphicsResourceInternal;
};

/** レンダーターゲットテクスチャのクラスです。 */
class RenderTargetTexture
    : public Texture
{
public:
    /**
     * レンダーターゲットテクスチャを作成します。ピクセルフォーマットは RGBA8 です。
     * @param[in]   width   : 幅 (px 単位)
     * @param[in]   height  : 高さ (px 単位)
     */
    static Ref<RenderTargetTexture> create(int width, int height);

    /**
     * レンダーターゲットテクスチャを作成します。
     * @param[in]   width   : 幅 (px 単位)
     * @param[in]   height  : 高さ (px 単位)
     * @param[in]   format  : ピクセルフォーマット
     */
    static Ref<RenderTargetTexture> create(int width, int height, TextureFormat format);

    /** 一時的な RenderTargetTexture を取得します。 */
    static Ref<RenderTargetTexture> getTemporary(int width, int height, TextureFormat format, bool mipmap);

    /** getTemporary で取得した一時的な RenderTargetTexture を解放します。 */
    static void releaseTemporary(RenderTargetTexture* renderTarget);

protected:
    virtual void onDispose(bool explicitDisposing) override;
    virtual void onChangeDevice(detail::IGraphicsDevice* device) override;
    virtual detail::ITexture* resolveRHIObject(bool* outModified) override;

LN_CONSTRUCT_ACCESS:
    RenderTargetTexture();
    virtual ~RenderTargetTexture();

    /** @copydoc create(int, int) */
    void init(int width, int height);

    /** @copydoc create(int, int, TextureFormat) */
    void init(int width, int height, TextureFormat format);

    void init(int width, int height, TextureFormat format, bool mipmap);
    void init(SwapChain* owner);

private:
    Ref<Bitmap2D> readData();
    void resetSwapchainFrameIfNeeded(bool force);

    Ref<detail::ITexture> m_rhiObject;
    SwapChain* m_ownerSwapchain;
    int m_swapchainImageIndex;
    bool m_modified;

    friend class detail::TextureInternal;
    friend class detail::GraphicsResourceInternal;
};

namespace detail {

class TextureInternal
{
public:
    static void setMappedData(Texture2D* texture, const void* data);
    static void setDesc(Texture* texture, int width, int height, TextureFormat format) { texture->setDesc(width, height, format); }
    static void setMipmapEnabled(Texture* texture, bool value) { texture->m_mipmap = value; }
    static Ref<Bitmap2D> readData(RenderTargetTexture* renderTarget) { return renderTarget->readData(); }
    static void resetSwapchainFrameIfNeeded(RenderTargetTexture* renderTarget, bool force) { renderTarget->resetSwapchainFrameIfNeeded(force); }
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
    void init(int width, int height, int depth, TextureFormat format = TextureFormat::RGBA8, bool mipmap = false, GraphicsResourceUsage usage = GraphicsResourceUsage::Static);

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
