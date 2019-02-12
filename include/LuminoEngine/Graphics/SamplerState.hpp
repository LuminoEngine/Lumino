
#pragma once
#include "GraphicsResource.hpp"

namespace ln {
namespace detail { class ISamplerState; }




class SamplerState
	: public GraphicsResource
{
public:
	void setFilterMode(TextureFilterMode value);
	void setAddressMode(TextureAddressMode value);

LN_CONSTRUCT_ACCESS:
	SamplerState();
	virtual ~SamplerState();
	virtual void init();
	virtual void onDispose(bool explicitDisposing) override;

LN_INTERNAL_ACCESS:
	detail::ISamplerState* resolveRHIObject();
	void setFrozen(bool value) { m_frozen = value; }

protected:
	virtual void onChangeDevice(detail::IGraphicsDeviceContext* device) override;

private:
	Ref<detail::ISamplerState> m_rhiObject;
	SamplerStateData m_desc;
	bool m_modified;
	bool m_frozen;
};


///// テクスチャフィルタ
//enum TextureFilterMode
//{
//	TextureFilterMode_Point = 0,		///< 補間を行わない
//	TextureFilterMode_Linear,			///< 補間を行う
//};
//
///// テクスチャアドレッシング
//enum TextureWrapMode
//{
//	TextureWrapMode_Repeat = 0,		///< 繰り返し
//	TextureWrapMode_Clamp,			///< 境界のピクセルが引き延ばされる
//};
//
///**
//	@brief	テクスチャのサンプリングステートを表すクラスです。
//*/
//class SamplerState
//{
//public:
//
//	TextureFilterMode	FilterMode;		///< テクスチャフィルタ(default:TextureFilterMode_Point)
//	TextureWrapMode		WrapMode;		///< テクスチャアドレッシング(default:TextureWrapMode_Repeat)
//
//public:
//
//	/**
//		@brief	デフォルト値で初期化します。
//	*/
//	SamplerState();
//};

} // namespace ln
