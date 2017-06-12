
#pragma once
#include <Lumino/Animation/Common.h>
#include <Lumino/Animation/AnimationCurve.h>

LN_NAMESPACE_BEGIN
class AnimationClip;
using AnimationClipPtr = RefPtr<AnimationClip>;

class AnimationClip
	: public RefObject
{
public:
	static AnimationClipPtr create(const StringRef& filePath);

public:
	struct AnimationCurveEntry
	{
		String					RelativePath;
		AnimationCurve*			Curve;
	};

	typedef List<AnimationCurveEntry>	AnimationCurveEntryList;

public:
	AnimationClip();
	virtual ~AnimationClip();

public:

	/// アニメーションクリップ名の設定
	void SetName(const String& name) { m_name = name; }

	/// アニメーションクリップ名取得
	const String& GetName() const { return m_name; }

	/// 終端フレームの時間
	double GetLastFrameTime() { return m_lastFrameTime; }

	/// デフォルトの再生方法の設定
	void SetDefaultWrapMode(WrapMode mode) { m_defaultrapMode = mode; }

	/// デフォルトの再生方法の取得
	WrapMode GetDefaultWrapMode() { return m_defaultrapMode; }

	/// アニメーションの追加 (3Dモデルモーションであれば、ボーン名, AttitudeTransformAnimation)
	void AddAnimationCurve(const String& relativePath, AnimationCurve* curve);

	/// 時間を設定し、各 AnimationCurve の補間値を計算する
	//void SetTime(double oldTime, double newTime);

	/// AnimationCurve とその割り当て先パス名のリスト
	const AnimationCurveEntryList& GetAnimationCurveEntryList() const { return m_animationCurveEntryList; }

private:
	String						m_name;
	double						m_lastFrameTime;	///< Get のたびに計算すると時間かかるので予め計算して持っておく
	WrapMode					m_defaultrapMode;
	AnimationCurveEntryList		m_animationCurveEntryList;
};

LN_NAMESPACE_END

