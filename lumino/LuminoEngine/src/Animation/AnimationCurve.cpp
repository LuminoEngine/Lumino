
#include "Internal.hpp"
#include <LuminoEngine/Animation/AnimationCurve.hpp>

namespace ln {

//==============================================================================
// AnimationCurve

AnimationCurve::AnimationCurve()
	: m_wrapMode(AnimationWrapMode::Once)
{
}

AnimationCurve::~AnimationCurve()
{
}

float AnimationCurve::evaluate(float time)
{
	return onEvaluate(calculateLocalTime(time, lastFrameTime(), m_wrapMode));
}

float AnimationCurve::lastFrameTime() const
{
	return onGetLastFrameTime();
}

float AnimationCurve::calculateLocalTime(float time, float duration, AnimationWrapMode wrapMode)
{
	float localTime = 0.0f;

	switch (wrapMode) {
	case AnimationWrapMode::Once:
		localTime = std::min(time, duration);
		break;
	case AnimationWrapMode::Loop:
		localTime = std::fmod(time, duration);
		break;
	case AnimationWrapMode::Alternate:
	{
		float freq = duration * 2;
		float t = std::fmod(time, freq);
		float phase = t / freq;
		if (phase <= 0.5) {
			localTime = t;
		}
		else {
			localTime = freq - t;
		}
		break;
	}
	default:
		LN_UNREACHABLE();
		break;
	}

	return localTime;
}

//==============================================================================
// KeyFrameAnimationCurve

Ref<KeyFrameAnimationCurve> KeyFrameAnimationCurve::create()
{
	return makeObject<KeyFrameAnimationCurve>();
}

KeyFrameAnimationCurve::KeyFrameAnimationCurve()
	: m_keyFrames()
	, m_defaultValue(0.0f)
{
}

KeyFrameAnimationCurve::~KeyFrameAnimationCurve()
{
}

bool KeyFrameAnimationCurve::init()
{
	return AnimationCurve::init();
}

void KeyFrameAnimationCurve::addKeyFrame(const AnimationKeyFrame& keyFrame)
{
	// そのまま追加できる
	if (m_keyFrames.isEmpty() || m_keyFrames.back().time <= keyFrame.time) {
		m_keyFrames.add(keyFrame);
	}
	// 追加後のソートが必要
	else {
		struct compare
		{
			bool operator()(const AnimationKeyFrame& l, const AnimationKeyFrame& r)
			{
				return l.time < r.time;
			}
		};

		m_keyFrames.add(keyFrame);
		std::stable_sort(m_keyFrames.begin(), m_keyFrames.end(), compare());
	}
}

void KeyFrameAnimationCurve::addKeyFrame(float time, float value, TangentMode rightTangentMode, float tangent)
{
	AnimationKeyFrame k;
	k.time = time;
	k.value = value;
	k.rightTangentMode = rightTangentMode;
	k.rightTangent = tangent;

	if (!m_keyFrames.isEmpty() && m_keyFrames.front().time <= time) {
		const AnimationKeyFrame* key0 = findKeyFrame(time);
		k.leftTangentMode = key0->rightTangentMode;
		k.leftTangent = -key0->rightTangent;
	}
	else {
		k.leftTangentMode = TangentMode::Constant;
		k.leftTangent = 0.0f;
	}

	addKeyFrame(k);
}

float KeyFrameAnimationCurve::onEvaluate(float time)
{
	if (m_keyFrames.isEmpty()) {
		return m_defaultValue;
	}
	// time が最初のフレーム位置より前の場合は Clamp
	if (time < m_keyFrames.front().time) {
		return m_keyFrames.front().value;
	}
	// キーがひとつだけの場合はそのキーの値
	else if (m_keyFrames.size() == 1) {
		return m_keyFrames.front().value;
	}
	// time が終端以降の場合は終端の値
	else if (time >= m_keyFrames.back().time) {
		return m_keyFrames.back().value;
	}
	// 以上以外の場合は補間する
	else
	{
		//struct Compare
		//{
		//	static int cmpKey(const void* a_, const void* b_)
		//	{
		//		AnimationKeyFrame* l = (AnimationKeyFrame*)a_;
		//		AnimationKeyFrame* r = (AnimationKeyFrame*)b_;

		//		if ((l)->time < (r)->time)
		//			return -1;
		//		else if ((l)->time >= (r)->time && (l)->time < ((r + 1))->time)
		//			return 0;
		//		else
		//			return 1;
		//	}
		//};

		const AnimationKeyFrame* key0 = findKeyFrame(time);
		const AnimationKeyFrame* key1 = key0 + 1;

		float p0 = key0->value;
		float p1 = key1->value;
		float t0 = static_cast<float>(key0->time);
		float t1 = static_cast<float>(key1->time);
		float t = static_cast<float>(time - t0) / (t1 - t0);

		// まず2種類のモードで保管する。最後にそれらを t で線形補間することで、異なる TangentMode を補間する
		TangentMode modes[2] = { key0->rightTangentMode, key1->leftTangentMode };
		float values[2];
		for (int i = 0; i < 2; i++)
		{
			switch (modes[i])
			{
			// 補間無し
			case TangentMode::Constant:
			{
				values[i] = p0;
				break;
			}
			// 線形
			case TangentMode::Linear:
			{
				values[i] = p0 + (p1 - p0) * t;
				break;
			}
			// 三次補間
			case TangentMode::Tangent:
			{
				values[i] = Math::hermite(
					p0, key0->rightTangent,
					p1, key1->leftTangent,
					t);
				break;
			}
			// Catmull-Rom
			case TangentMode::Auto:
			{
				// ループ再生で time が終端を超えている場合、
				// この時点でkey の値は ループ開始位置のひとつ前のキーを指している

				const AnimationKeyFrame& begin = m_keyFrames.front();
				const AnimationKeyFrame& end = m_keyFrames.back();

				// この補間には、begin のひとつ前と end のひとつ後の値が必要。
				// それぞれが始点、終点の場合はループするように補間する
				values[i] = Math::catmullRom(
					((key0->time == begin.time) ? end.value : (key0 - 1)->value),
					p0,
					p1,
					((key1->time == end.time) ? begin.value : (key0 + 2)->value),
					t);
				break;
			}
			}
		}

		return values[0] + (values[1] - values[0]) * t;
	}
}

float KeyFrameAnimationCurve::onGetLastFrameTime() const
{
	if (m_keyFrames.isEmpty()) return 0.0f;
	return m_keyFrames.back().time;
}

AnimationKeyFrame* KeyFrameAnimationCurve::findKeyFrame(float time)
{
	// TODO: 二分探索
	//for (auto& key : m_keyFrames)
	for (int i = m_keyFrames.size() - 1; i >= 0; i--) {
		if (m_keyFrames[i].time <= time) {
			return &m_keyFrames[i];
		}
	}
	return nullptr;
	//struct Compare
	//{
	//	static int cmpKey(const void* a_, const void* b_)
	//	{
	//		AnimationKeyFrame* l = (AnimationKeyFrame*)a_;
	//		AnimationKeyFrame* r = (AnimationKeyFrame*)b_;

	//		if ((l)->time < (r)->time)
	//			return -1;
	//		else if ((l)->time >= (r)->time && (l)->time < ((r + 1))->time)
	//			return 0;
	//		else
	//			return 1;
	//	}
	//};

	//return (AnimationKeyFrame*)bsearch(&time, &(m_keyFrames[0]), m_keyFrames.getCount(), sizeof(AnimationKeyFrame), Compare::cmpKey);
}

//==============================================================================
// EasingAnimationCurve

Ref<EasingAnimationCurve> EasingAnimationCurve::create(float startValue, float targetValue, float duration, EasingMode function)
{
	return makeObject<EasingAnimationCurve>(startValue, targetValue, duration, function);
}

EasingAnimationCurve::EasingAnimationCurve()
	: m_function(nullptr)
	, m_startValue(0.0f)
	, m_targetValue(0.0f)
	, m_duration(0.0f)
{
}

bool EasingAnimationCurve::init(float startValue, float targetValue, float duration, EasingMode function)
{
	if (!Object::init()) return false;
	m_function = EasingFunctions::function(function);
	m_startValue = startValue;
	m_targetValue = targetValue;
	m_duration = duration;
	return true;
}

float EasingAnimationCurve::onEvaluate(float time)
{
	return m_function(time, m_startValue, m_targetValue - m_startValue, m_duration);
}

float EasingAnimationCurve::onGetLastFrameTime() const
{
	return m_duration;
}

} // namespace ln
