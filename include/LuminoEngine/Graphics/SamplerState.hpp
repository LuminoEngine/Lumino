// Copyright (c) 2019+ lriki. Distributed under the MIT license.
#pragma once
#include "Common.hpp"
#include "GraphicsResource.hpp"

namespace ln {

/** サンプラーステートのクラスです。 */
class SamplerState
    : public Object
    , public IGraphicsResource
{
public:
    /**
     * デフォルトの設定で SamplerState を作成します。
     */
    static Ref<SamplerState> create();

    /**
     * SamplerState を作成します。
     * @param[in]  filter            : テクスチャフィルタモード
     */
    static Ref<SamplerState> create(TextureFilterMode filter);

    /**
     * SamplerState を作成します。
     * @param[in]  filter            : テクスチャフィルタモード
     * @param[in]  address           :  テクスチャアドレッシングモード
     */
    static Ref<SamplerState> create(TextureFilterMode filter, TextureAddressMode address);

    /**
     * SamplerState を作成します。
     * @param[in]  filter            : テクスチャフィルタモード
     * @param[in]  address           :  テクスチャアドレッシングモード
     * @param[in]  anisotropyEnabled : 異方性フィルタリングの有無
     */
    static Ref<SamplerState> create(TextureFilterMode filter, TextureAddressMode address, bool anisotropyEnabled);

    static SamplerState* pointClamp();
    static SamplerState* linearClamp();

    /** テクスチャフィルタモードを設定します。(default: Point) */
    void setFilterMode(TextureFilterMode value);

    /** テクスチャフィルタモードを取得します。(default: Point) */
    TextureFilterMode filterMode() const { return m_desc.filter; }

    /** テクスチャアドレッシングモードを設定します。(default: Repeat) */
    void setAddressMode(TextureAddressMode value);

    /** テクスチャアドレッシングモードを取得します。(default: Repeat) */
    TextureAddressMode addressMode() const { return m_desc.address; }

    /** 異方性フィルタリングの有効状態を設定します。(default: false) */
    void setAnisotropyEnabled(bool value);

    /** 異方性フィルタリングの有効状態を取得します。(default: false) */
    bool anisotropyEnabled() const { return m_desc.anisotropy; }

protected:
    void onDispose(bool explicitDisposing) override;
    void onManagerFinalizing() override { dispose(); }
    void onChangeDevice(detail::IGraphicsDevice* device) override;

LN_CONSTRUCT_ACCESS:
    SamplerState();
    virtual ~SamplerState();

    /** @copydoc create() */
    void init();

    /** @copydoc create(TextureFilterMode) */
    void init(TextureFilterMode filter);

    /** @copydoc create(TextureFilterMode, TextureAddressMode) */
    void init(TextureFilterMode filter, TextureAddressMode address);

    /** @copydoc create(TextureFilterMode, TextureAddressMode, bool) */
    void init(TextureFilterMode filter, TextureAddressMode address, bool anisotropyEnabled);

private:
    detail::ISamplerState* resolveRHIObject(GraphicsContext* context, bool* outModified);
    void setFrozen(bool value) { m_frozen = value; }

    detail::GraphicsManager* m_manager;
    Ref<detail::ISamplerState> m_rhiObject;
	detail::SamplerStateData m_desc;
    bool m_modified;
    bool m_frozen;

    friend class detail::GraphicsResourceInternal;
    friend class detail::GraphicsManager;
};

} // namespace ln
