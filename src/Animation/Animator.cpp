/*
	[2015/5/18]
	Static な頂点バッファの描画は、DrawPrimitiveUP よりも少なくとも 4 倍は早い。
	ユーちゃんモデルでは 4ms かかってたのが 0ms 以下になった。(頂点数 37000)

	一方、全 Lock すると Static な方は 50ms とかかかった。
	部分 Lock だと、Lock した頂点数に比例する。
	数個程度なら 0ms、半分の 18500個だと 25ms 前後になる。

	Dynamic な頂点バッファの描画は、Lock しなければ 1ms くらい。
	Lock すると 5～10ms。部分 Lock でも全 Lock でも変わらなかった。


	広範を頻繁に書き換えるなら、平均的に見て DrawPrimitiveUP が一番よさそうではあるが、
	常に 4ms もかかるのはゲームとしては考えもの。


	実際に使われるのはほとんど表情アニメーションで、書き換えの対象となる頂点数はせいぜい1000個程度。
	（たまに艦装を丸ごと縮小して隠すなんてことやってるモデルもあるが）
	Static な 1000個 Lock は約 2ms だった。





	・メッシュコンテナの必要性
	マテリアルでは賄いきれないデータのグループ化に使う。例えば、
	・頂点フォーマットが異なる
	・constant buffer を使用したスキニングのためのボーンのグループ化
	・頂点カラーに透明が含まれるものは後から描画したい




	[2015/5/17]

	IAnimationTargetElement
		virtual IAnimationTargetAttribute[] GetAnimationTargetAttributes() = 0;
		アニメーションを適用する一連のオブジェクトの親要素。
		・3D モデルならモデルそのもの。
		・GUI なら Control。

	IAnimationTargetAttribute
		virtual GetName() = 0;
		virtual SetValue() = 0;
		補間値を適用するオブジェクト。
		・3D モデルならノード、モーフ。
		・GUI ならプロパティ。


	AnimationController
		AnimationClip[]

	AnimationClip
		Name
		{ AnimationCurve, TargetName }[]
		AnimationCurveをグループ化する。
		・3D モデルのアニメーション1つ分に相当する。(Name は "歩く"、"走る" 等)
		・GUI では状態1つ分に相当する。("ボタン押下"、"非アクティブ" 等)
		AnimationCurve は、それが何に対するアニメーションなのかを示すキー名を持たせて管理する。
		・3D モデルではボーン名。
		・GUI ではプロパティ名。
		使い回しもできる。

	AnimationCurve
		KeyFrame[]
		実際に値の補間を行うクラス。それだけ。
		使い回しもできる。


	レイヤー
		[Unity] https://psm.playstation.net/static/general/all/unity_for_psm/ja/Documentation/Manual/MecanimFAQ.html
		レイヤーの順番は何かに影響しますか？
			はい。レイヤーは一番上から下まで順に評価されます。
			 Override とセットされたレイヤーは常に前の順番のレイヤーを上書きします（マスクがあれば、そのマスクにもとづきます）。 
		
	
		・武器を振りながら移動する
			// 初期化
			animCtrl->GetAnimationState("swing")->SetLayer(1);	// デフォルトは 0
			// フレーム更新
			animCtrl->Play("run", WrapMode_Loop);	// wrap は state->SetWrapMode(WrapMode_Loop) であらかじめ設定してもよい
			animCtrl->Play("swing");
		
			下のレイヤーのアニメーションは上書きするので、例えば "run" アニメの "右腕" ボーンは上書きされる。
			下半身は、"swing" アニメにキーフレームがひつもなければ1度も適用されないので、"run" のアニメが生き続けることになる。
		
		・同レイヤーのアニメーションは加算ブレンドされる。
		  これは主にフェイスアニメーション用になりそう。
	
		・フィルタリング
			例えば、あるボーン以下にのみ適用したいアニメーションがあるとする。
			これは Animation モジュールだけでは解決できない。IAnimationTargetAttribute は階層構造を持たないため。
			なので、Modeling モジュールに MMDAnimationController みたいな拡張を行うことになる。

*/

#include "../Internal.h"
#include "AnimationClip.h"
#include "AnimationState.h"
#include "Animator.h"

namespace Lumino
{
namespace Animation
{

//=============================================================================
// Animator
//=============================================================================

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Animator::Animator()
	: m_element(NULL)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
Animator::~Animator()
{
	LN_FOREACH(AnimationState* s, m_animationStateList)
	{
		s->Release();
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Animator::Create(IAnimationTargetElement* element)
{
	m_element = element;

	int count = m_element->GetAnimationTargetAttributeCount();
	m_animationTargetAttributeEntityList.Resize(count);
	for (int i = 0; i < count; i++)
	{
		m_animationTargetAttributeEntityList[i].Target = m_element->GetAnimationTargetAttribute(i);
		m_animationTargetAttributeEntityList[i].Type = ValueType_Float;	// とりあえず初期化
		memset(m_animationTargetAttributeEntityList[i].Buffer, 0, sizeof(m_animationTargetAttributeEntityList[i].Buffer));
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool Animator::IsPlaying() const
{
	LN_THROW(0, NotImplementedException);
	return false;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Animator::Play(const TCHAR* name/*, PlayMode mode*/)
{
	AnimationState* state = FindAnimationState(name);
	if (state != NULL)
	{
		// 再生中状態にする
		state->SetPlayState(PlayState_Playing);
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Animator::AdvanceTime(double elapsedTime)
{
	// 一時バッファをクリアする (TODO: Valiant 型みたいなのを作って、各値毎に適切なクリア処理を実装する方が良いかもしれない…)
	LN_FOREACH(AnimationTargetAttributeEntity& e, m_animationTargetAttributeEntityList)
	{
		memset(e.Buffer, 0, sizeof(e.Buffer));
		e.Modified = false;
	}

	// TODO: レイヤーの考慮
	LN_FOREACH(AnimationState* state, m_animationStateList)
	{
		state->AdvanceTime(elapsedTime);
	}

	LN_FOREACH(AnimationTargetAttributeEntity& e, m_animationTargetAttributeEntityList)
	{
		//if (e.Target->GetAnimationTargetName() == _T("右腕")) {
		//	printf("b");
		//}
		// 一連の処理の中で本当に値がセットされたものだけ通知する
		if (e.Modified) {
			e.Target->SetAnimationTargetValue(e.Type, e.Buffer);
		}
	}
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Animator::AddAnimationClip(AnimationClip* animationClip, int layer)
{
	LN_VERIFY(animationClip != NULL) { return; }

	AnimationState* state = LN_NEW AnimationState(animationClip);
	m_animationStateList.Add(animationClip->GetName(), state);
	state->Refresh(this);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
void Animator::RemoveAnimationClip(AnimationClip* animationClip)
{
	LN_VERIFY(animationClip != NULL) { return; }
	LN_THROW(0, NotImplementedException);
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AnimationTargetAttributeEntity* Animator::FindAnimationTargetAttributeEntity(const String& name) const
{
	LN_FOREACH(AnimationTargetAttributeEntity& e, m_animationTargetAttributeEntityList)
	{
		if (e.Target->GetAnimationTargetName() == name) {
			return &e;
		}
	}
	return NULL;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
AnimationState* Animator::FindAnimationState(const TCHAR* clipName)
{
	LN_VERIFY(clipName != NULL) { return NULL; }
	LN_FOREACH(AnimationState* state, m_animationStateList)
	{
		if (state->GetName() == clipName) {
			return state;
		}
	}
	return NULL;
}


} // namespace Animation
} // namespace Lumino
