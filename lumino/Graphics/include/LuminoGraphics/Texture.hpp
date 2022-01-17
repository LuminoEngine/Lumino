#pragma once
#include <LuminoEngine/Asset/AssetObject.hpp>
#include <LuminoEngine/Graphics/ColorStructs.hpp>
#include <LuminoFont/Common.hpp>
#include "Common.hpp"
#include "GraphicsResource.hpp"

namespace ln {
namespace detail {
class TextureInternal;
}

/** テクスチャのベースクラスです。 */
LN_CLASS()
class Texture
    : public AssetObject
    , public IGraphicsResource
{
    LN_OBJECT;
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

    detail::DescriptorResourceType descriptorResourceType() const override { return detail::DescriptorResourceType_Texture; }

protected:
    Texture();
    virtual ~Texture();
    bool init();
    void onDispose(bool explicitDisposing) override;
    void onManagerFinalizing() override { dispose(); }
    virtual detail::RHIResource* resolveRHIObject(GraphicsContext* context, bool* outModified) = 0;

private:
    void setDesc(int width, int height, TextureFormat format);

    detail::GraphicsManager* m_manager;
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
    LN_OBJECT;
public:
    /**
     * テクスチャを作成します。ピクセルフォーマットは RGBA8 です。
     *
     * @param[in] width   : 幅 (px 単位)
     * @param[in] height  : 高さ (px 単位)
     * @return 作成されたテクスチャ
     */
    static Ref<Texture2D> create(int width, int height);

    /**
     * テクスチャを作成します。
     * @param[in] width   : 幅 (px 単位)
     * @param[in] height  : 高さ (px 単位)
     * @param[in] format  : ピクセルフォーマット
     */
    static Ref<Texture2D> create(int width, int height, TextureFormat format);

    /**
     * ローカルのファイルを読み込み、テクスチャを作成します。
	 *
     * @param[in] filePath : 読み込むファイルのパス
     * @param[in] format   : ピクセルフォーマット
	 *
	 * このメソッドは TextureImporter のユーティリティです。
     */
    //static Ref<Texture2D> create(const StringView& filePath, TextureFormat format = TextureFormat::RGBA8);

    /**
     * アセットからテクスチャを読み込みます。
     *
     * @param[in] filePath : 読み込むファイルのパス
     *
     * サポートしているフォーマットは次の通りです。
     * PNG(.png), JPG(.jpg), TGA(.tga), BMP(.bmp), GIF(.gif)
     */
	LN_METHOD()
    static Ref<Texture2D> load(const StringView& filePath);

    static Ref<Texture2DPromise> loadAsync(const StringView& filePath);

    /**
     * loadEmoji
     *
     * @param[in] code : xxxx
     *
     */
	LN_METHOD()
	static Ref<Texture2D> loadEmoji(StringView code);

	static Ref<Texture2D> loadEmoji(uint32_t codePoint);

	static Texture2D* blackTexture();

	static Texture2D* whiteTexture();

public:
    /** Mipmap の有無を設定します。(default: false) */
    void setMipmapEnabled(bool value);

    /** テクスチャが保持するビットマップデータにアクセスします。 */
    Bitmap2D* map(MapMode mode);
    void unmap() {}

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

    void drawText(const StringView& text, const Rect& rect, Font* font, const Color& color, TextAlignment alignment = TextAlignment::Forward);

    // TODO: internal
    //void setAssetSource(const Path& path) { m_assetSourcePath = path; }

protected:
    void onDispose(bool explicitDisposing) override;
    void onChangeDevice(detail::IGraphicsDevice* device) override;
    detail::RHIResource* resolveRHIObject(GraphicsContext* context, bool* outModified) override;
    void serialize(Serializer2& ar) override;
    const std::vector<const Char*>& resourceExtensions() const override;
    void onLoadResourceFile(Stream* stream, const detail::AssetPath& assetPath) override;

LN_CONSTRUCT_ACCESS:
    Texture2D();
    virtual ~Texture2D();
    bool init();

    /** @copydoc create(int, int) */
    LN_METHOD()
    void init(int width, int height);

    /** @copydoc create(int, int, TextureFormat) */
    LN_METHOD(OverloadPostfix = "WithFormat")
    void init(int width, int height, TextureFormat format);

    //bool init(const Path& assetPath);

    void init(Stream* stream, TextureFormat format = TextureFormat::RGBA8);
    void init(Bitmap2D* bitmap, TextureFormat format = TextureFormat::RGBA8);

private:
    Ref<detail::RHIResource> m_rhiObject;
    GraphicsResourceUsage m_usage;
    GraphicsResourcePool m_pool;
    Ref<Bitmap2D> m_bitmap;
    //Path m_assetSourcePath;
    //detail::AssetPath m_sourceFilePath; // see Assets/README.md
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
    // Note: 内部利用は禁止。外部の、特にカスタムのポストエフェクトを実装するときに使う。
    static void releaseTemporary(RenderTargetTexture* renderTarget);

    bool isBackbuffer() const { return m_ownerSwapchain != nullptr; }

    bool m_cleared = false;

    bool m_msaa = false;


    static Ref<RenderTargetTexture> realloc(RenderTargetTexture* renderTarget, int width, int height, TextureFormat format, bool mipmap, SamplerState* samplerState);

protected:
    virtual void onDispose(bool explicitDisposing) override;
    virtual void onChangeDevice(detail::IGraphicsDevice* device) override;
    virtual detail::RHIResource* resolveRHIObject(GraphicsContext* context, bool* outModified) override;

LN_CONSTRUCT_ACCESS:
    RenderTargetTexture();
    virtual ~RenderTargetTexture();

    /** @copydoc create(int, int) */
    void init(int width, int height);

    /** @copydoc create(int, int, TextureFormat) */
    void init(int width, int height, TextureFormat format);

    bool init(int width, int height, TextureFormat format, bool mipmap, bool msaa);
    bool init(SwapChain* owner);
    bool init(intptr_t nativeObject, TextureFormat format);
    void resetNativeObject(intptr_t nativeObject);
    void resetSize(int width, int height);

private:
    bool init();
	void resetRHIObject(detail::RHIResource* rhiObject);
    Ref<Bitmap2D> readData(GraphicsContext* context);

    Ref<detail::RHIResource> m_rhiObject;
    SwapChain* m_ownerSwapchain;
    intptr_t m_nativeObject;
    bool m_modified;
    bool m_hasNativeObject;

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
    static Ref<Bitmap2D> readData(RenderTargetTexture* renderTarget, GraphicsContext* context) { return renderTarget->readData(context); }
	static void resetRHIObject(RenderTargetTexture* renderTarget, detail::RHIResource* rhiObject) { renderTarget->resetRHIObject(rhiObject); }
    static void resetNativeObject(RenderTargetTexture* renderTarget, intptr_t value) { renderTarget->resetNativeObject(value); }
    static void resetSize(RenderTargetTexture* renderTarget, int width, int height) { renderTarget->resetSize(width, height); }
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
    virtual detail::RHIResource* resolveRHIObject(GraphicsContext* context, bool* outModified) override;

private:
    Ref<detail::RHIResource> m_rhiObject;
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
