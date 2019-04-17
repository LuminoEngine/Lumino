
#pragma once
#include "GraphicsResource.hpp"

namespace ln {
namespace detail { class ISamplerState; }




/** サンプラーステートのクラスです。 */
class SamplerState
	: public GraphicsResource
{
public:
    /** テクスチャフィルタモードを設定します。(default: Point) */
	void setFilterMode(TextureFilterMode value);

    /** テクスチャフィルタモードを取得します。(default: Point) */
    TextureFilterMode filterMode() const { return m_desc.filter; }

    /** テクスチャアドレッシングモードを設定します。(default: Repeat) */
	void setAddressMode(TextureAddressMode value);

    /** テクスチャアドレッシングモードを取得します。(default: Repeat) */
    TextureAddressMode addressMode() const { return m_desc.address; }

    // anisotropyEnable

protected:
	virtual void onChangeDevice(detail::IGraphicsDevice* device) override;

LN_CONSTRUCT_ACCESS:
	SamplerState();
	virtual ~SamplerState();
	void init();
    void init(TextureFilterMode filter);
    void init(TextureFilterMode filter, TextureAddressMode value);
    void init(TextureFilterMode filter, TextureAddressMode value, bool anisotropyEnable);
	virtual void onDispose(bool explicitDisposing) override;

LN_INTERNAL_ACCESS:
	detail::ISamplerState* resolveRHIObject();
	void setFrozen(bool value) { m_frozen = value; }


private:
	Ref<detail::ISamplerState> m_rhiObject;
	SamplerStateData m_desc;
	bool m_modified;
	bool m_frozen;
};

} // namespace ln
