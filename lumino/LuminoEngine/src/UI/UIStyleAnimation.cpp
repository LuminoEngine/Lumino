/*
    [2020/4/8]
    ----------
    値の優先度は、
    1. デフォルト値
    2. 継承した値
    3. アニメーション値
    4. ローカル値

    例えば ListItem の背景を特別に赤くしたい時は直接 element->setBackgroundColor() とかするが、
    このときはユーザープログラムが強い意志で色を変えたいということで、アニメーション値は上書きされる。


    beginFrame
        updateFrame
            setPosition とか setVisible とか focus とか、メインでコードベースで色々やるのはここ。
            invalidate flag 付いたり、visual state 変わったりする。
            時間を正しく認識できるのはここだけ。作られている instance に対して elapsedTime を適用する。
            値の適用もここでやる。invalidateStyle が無ければ、このフレームでは updateStyle は実行されないこともある。
        updateStyle
            update後、アニメーションによる値の適用を行う。updateFrame() でやったのと同じ処理。
            こうしておかないと、継承した値をアニメーション値で上書きできない。
            updateStyle が呼ばれるフレームでは updateFrame と同じ処理が動くがやむなし。
            updateFrame 側で、invalidateStyle 状態だったら アニメ適用しない対応はできるかも。
        updateLayout
            
        render
    endFrame

  [2020/4/7]
  ----------
  結局 WPF のようなプロパティ単位で極限まで抽象化されたアニメーションの実装はやめることにした。
  メンテナンスの難易度もそうだが、文字列検索が頻繁に発生したり、パフォーマンスへの影響も大きい。

  なので、Flutter や Android を参考にしつつ、CSS の形に寄せてみることにした。
  簡単に言うと定義済みのプロパティに対してのみアニメーションを適用できるようにする。

  例えば次のような具合。
  setBackgroundColorAnimation(Color::Red, 1.0);
  setOpacity(0.5, 1.0);
  setFontSize(20, 0.5);
  setSize(100, 2.0);    // setWidth() setHeight() の統合ユーティリティ

  基本単位。
  UIStyleAnimationKey
    parcent,
    Optional<backgroundColor>,
    Optional<opacity>,
    Optional<fontSize>,
    Optional<width>,
    Optional<height>,
  細かくするとパフォーマンスが気になるなら、Flutter のように Width, Height をまとめて Size にするとかしてもよい。

  UIStyleAnimationTrack
    CSS の @keyframes 相当。UIStyleAnimationKey のリスト。

  UIAnimationClip
    animation: に , 区切りで指定する 1 要素。
    ひとつの UIStyleAnimationTrack と、開始時間や t の生成方法などを指定する。


    Animation には２種類ある。
    - animation: キーフレームアニメーション。値はすべて事前定義。
    - transition: アクティブになったら、現在値から目標値へ遷移する。



    Animation は Style の要素単位で持つ。
    "Vector3 position" ではなく、"float positionX, positionY, positionZ" のようにプリミティブ単位で。
    こうしておかないと、たとえば x y 「どちらかだけ省略する」ができなくなる。
    Size を例にすると、高さは Alignment.Center で決めたいが幅はマウスオーバーしたらアニメ付きで伸ばしたい、といったときに弊害になる。


    適用時は、
    StyleInstance に TransitionInstance を、こちらも要素単位で持たせておく。
    AnimationInstance は、combinedStyle に集まってくる AnimationElement (ポインタ)をキーに作る。revsiton 見てもいいかも。

    Animation と Transition の同時指定は無効。
    Style に一応設定はできるが、Instance を作るときは Transition が優先される。



    Color は要素ごとにアニメしなくていいかもしれない。というか要素単位アニメほとんど使わない気がする。CSSでも無いし。

    
    
*/
#include "Internal.hpp"
#include <LuminoGraphics/Animation/AnimationCurve.hpp>
#include <LuminoEngine/UI/UIStyle.hpp>
#include <LuminoEngine/UI/UIStyleAnimation.hpp>

namespace ln {

//==============================================================================
// UITransitionClip

UIAnimationClip::UIAnimationClip()
{
}

bool UIAnimationClip::init()
{
    return Object::init();
}

void UIAnimationClip::addCurve(detail::UIStyleAnimationElement element, float startValue, float targetValue, float duration, EasingMode func, AnimationWrapMode wrapMode)
{
    m_curves[element] = EasingAnimationCurve::create(startValue, targetValue, duration, func);
    m_curves[element]->setWrapMode(wrapMode);
}

//==============================================================================
// UITransitionClip

UITransitionClip::UITransitionClip()
{
}

bool UITransitionClip::init()
{
    return Object::init();
}

void UITransitionClip::addCurve(detail::UIStyleAnimationElement element, float targetValue, float duration, EasingMode func, float delay)
{
    m_curves[element] = Curve{ EasingFunctions::function(func), targetValue, duration, delay };
}

} // namespace ln

