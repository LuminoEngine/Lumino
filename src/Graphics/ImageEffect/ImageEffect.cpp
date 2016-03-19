/*
	[2016/3/16] ImageEffect は OnUpdate は持たない。
		Graphics モジュールとしては、フレームワーク的な時間更新の機能は持つべきではないと思う。
		Particle や Model はユーザーに直接 Update 呼び出してもらうか、
		Scene モジュールでラップしたクラスから呼び出すか。

		何でかというと、Editor モードでは OnUpdate は普通使わないから。
		特にツールウィンドウ。
		描画が必要になったら Invalidate() して再描画が必要なことを示して、
		あとでまとめて描画してもらうというスタイル。
		このときのアニメーションは、OnUpdate ではなくアニメーションを管理するモジュールに
		AnimationClock みたいな (WPF的な) のを登録してプロパティに対してアニメーションするのが一般的。

		ちょっと使いづらくなるかもしれないけど、遷移時間を指定して OnUpdate でアニメーションする従来の方法は廃止。
		ただ、やっぱり使いたくなることは出てくると思うので、
		Game モード用のアニメーション管理モジュールに AnimationClock を登録するような
		ユーティリティ関数を作るのは後々必要そう。
*/
#include "../Internal.h"
#include <Lumino/Graphics/ImageEffect/ImageEffect.h>

LN_NAMESPACE_BEGIN
LN_NAMESPACE_GRAPHICS_BEGIN

//=============================================================================
// ImageEffect
//=============================================================================
LN_TR_REFLECTION_TYPEINFO_IMPLEMENT(ImageEffect, Object);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ImageEffect::ImageEffect()
	: m_manager(nullptr)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
ImageEffect::~ImageEffect()
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void ImageEffect::Initialize(GraphicsManager* manager)
{
	LN_CHECK_ARGS_RETURN(manager != nullptr);
	m_manager = manager;
}

LN_NAMESPACE_GRAPHICS_END
LN_NAMESPACE_END
