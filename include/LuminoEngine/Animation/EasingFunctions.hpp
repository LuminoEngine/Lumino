
#pragma once
#include <cmath>
#include "Common.hpp"

namespace ln {

typedef std::function<float(float, float, float, float)>		FloatEasingFunction;
typedef std::function<float(double, double, double, double)>	DoubleEasingFunction;

/** 
 * 補間関数の定義。
 * 各関数の引数は、t:現在時間(0.0～d) b:開始値 c:値の変化量 (目標値-開始値) d:変化にかける時間。
 */
class EasingFunctions
{
public:
	/** simple linear tweening - no easing, no acceleration */
	template<typename TValue, typename TTime>
	static TValue linearTween(TTime t, TValue b, TValue c, TTime d)
	{
		return c * (t / d) + b;
	};

	/** quadratic easing in - accelerating from zero velocity */
	template<typename TValue, typename TTime>
	static TValue easeInQuad(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d;
		return c*t*t + b;
	};

	/** quadratic easing out - decelerating to zero velocity */
	template<typename TValue, typename TTime>
	static TValue easeOutQuad(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d;
		return -c * t*(t - 2) + b;
	};

	/** quadratic easing in/out - acceleration until halfway, then deceleration */
	template<typename TValue, typename TTime>
	static TValue easeInOutQuad(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d / 2;
		if (t < 1) return c / 2 * t*t + b;
		t--;
		return -c / 2 * (t*(t - 2) - 1) + b;
	};

	/** cubic easing in - accelerating from zero velocity */
	template<typename TValue, typename TTime>
	static TValue easeInCubic(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d;
		return c*t*t*t + b;
	};

	/** cubic easing out - decelerating to zero velocity */
	template<typename TValue, typename TTime>
	static TValue easeOutCubic(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d;
		t--;
		return c*(t*t*t + 1) + b;
	};

	/** cubic easing in/out - acceleration until halfway, then deceleration */
	template<typename TValue, typename TTime>
	static TValue easeInOutCubic(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d / 2;
		if (t < 1) return c / 2 * t*t*t + b;
		t -= 2;
		return c / 2 * (t*t*t + 2) + b;
	};

	/** quartic easing in - accelerating from zero velocity */
	template<typename TValue, typename TTime>
	static TValue easeInQuart(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d;
		return c*t*t*t*t + b;
	};

	/** quartic easing out - decelerating to zero velocity */
	template<typename TValue, typename TTime>
	static TValue easeOutQuart(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d;
		t--;
		return -c * (t*t*t*t - 1) + b;
	};

	/** quartic easing in/out - acceleration until halfway, then deceleration */
	template<typename TValue, typename TTime>
	static TValue easeInOutQuart(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d / 2;
		if (t < 1) return c / 2 * t*t*t*t + b;
		t -= 2;
		return -c / 2 * (t*t*t*t - 2) + b;
	};

	/** quintic easing in - accelerating from zero velocity */
	template<typename TValue, typename TTime>
	static TValue easeInQuint(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d;
		return c*t*t*t*t*t + b;
	};

	/** quintic easing out - decelerating to zero velocity */
	template<typename TValue, typename TTime>
	static TValue easeOutQuint(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d;
		t--;
		return c*(t*t*t*t*t + 1) + b;
	};

	/** quintic easing in/out - acceleration until halfway, then deceleration */
	template<typename TValue, typename TTime>
	static TValue easeInOutQuint(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d / 2;
		if (t < 1) return c / 2 * t*t*t*t*t + b;
		t -= 2;
		return c / 2 * (t*t*t*t*t + 2) + b;
	};

	/** sinusoidal easing in - accelerating from zero velocity */
	template<typename TValue, typename TTime>
	static TValue easeInSine(TTime t, TValue b, TValue c, TTime d)
	{
		return -c * std::cos(t / d * ((3.14159265358979323846f) / 2.0f)) + c + b;
	};

	/** sinusoidal easing out - decelerating to zero velocity */
	template<typename TValue, typename TTime>
	static TValue easeOutSine(TTime t, TValue b, TValue c, TTime d)
	{
		return c * std::sin(t / d * ((3.14159265358979323846f) / 2.0f)) + b;
	};

	/** sinusoidal easing in/out - accelerating until halfway, then decelerating */
	template<typename TValue, typename TTime>
	static TValue easeInOutSine(TTime t, TValue b, TValue c, TTime d)
	{
		return -c / 2.0f * (std::cos((3.14159265358979323846f) * t / d) - 1.0f) + b;
	};

	/** exponential easing in - accelerating from zero velocity */
	template<typename TValue, typename TTime>
	static TValue easeInExpo(TTime t, TValue b, TValue c, TTime d)
	{
		return c * std::pow(2.0f, 10.0f * (t / d - 1.0f)) + b;
	};

	/** exponential easing out - decelerating to zero velocity */
	template<typename TValue, typename TTime>
	static TValue easeOutExpo(TTime t, TValue b, TValue c, TTime d)
	{
		return c * (-std::pow(2.0f, -10.0f * t / d) + 1.0f) + b;
	};

	/** exponential easing in/out - accelerating until halfway, then decelerating */
	template<typename TValue, typename TTime>
	static TValue easeInOutExpo(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d / 2.0f;
		if (t < 1) return c / 2.0f * std::pow(2.0f, 10.0f * (t - 1.0f)) + b;
		t--;
		return c / 2.0f * (-std::pow(2.0f, -10.0f * t) + 2.0f) + b;
	};

	/** circular easing in - accelerating from zero velocity */
	template<typename TValue, typename TTime>
	static TValue easeInCirc(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d;
		return -c * (std::sqrt(1.0f - t*t) - 1.0f) + b;
	};

	/** circular easing out - decelerating to zero velocity */
	template<typename TValue, typename TTime>
	static TValue easeOutCirc(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d;
		t--;
		return c * std::sqrt(1.0f - t*t) + b;
	};

	/** circular easing in/out - acceleration until halfway, then deceleration */
	template<typename TValue, typename TTime>
	static TValue easeInOutCirc(TTime t, TValue b, TValue c, TTime d)
	{
		t /= d / 2.0f;
		if (t < 1.0f) return -c / 2.0f * (std::sqrt(1.0f - t*t) - 1.0f) + b;
		t -= 2.0f;
		return c / 2.0f * (std::sqrt(1.0f - t*t) + 1.0f) + b;
	};


	typedef float(*FloatEasingFunctionPtr)(float, float, float, float);

	constexpr static FloatEasingFunctionPtr function(EasingMode easing)
	{
		FloatEasingFunctionPtr table[] =
		{
			EasingFunctions::linearTween,
			EasingFunctions::easeInQuad,
			EasingFunctions::easeOutQuad,
			EasingFunctions::easeInOutQuad,
			EasingFunctions::easeInCubic,
			EasingFunctions::easeOutCubic,
			EasingFunctions::easeInOutCubic,
			EasingFunctions::easeInQuart,
			EasingFunctions::easeOutQuart,
			EasingFunctions::easeInOutQuart,
			EasingFunctions::easeInQuint,
			EasingFunctions::easeOutQuint,
			EasingFunctions::easeInOutQuint,
			EasingFunctions::easeInSine,
			EasingFunctions::easeOutSine,
			EasingFunctions::easeInOutSine,
			EasingFunctions::easeInExpo,
			EasingFunctions::easeOutExpo,
			EasingFunctions::easeInOutExpo,
			EasingFunctions::easeInCirc,
			EasingFunctions::easeOutCirc,
			EasingFunctions::easeInOutCirc,
		};
		assert(LN_ARRAY_SIZE_OF(table) == 22);
		return table[(int)easing];
	}

	using Vector4EasingFunctionPtr = Vector4(*)(float, Vector4, Vector4, float);

	constexpr static Vector4EasingFunctionPtr functionVector4(EasingMode easing)
	{
		Vector4EasingFunctionPtr table[] =
		{
			EasingFunctions::linearTween,
			EasingFunctions::easeInQuad,
			EasingFunctions::easeOutQuad,
			EasingFunctions::easeInOutQuad,
			EasingFunctions::easeInCubic,
			EasingFunctions::easeOutCubic,
			EasingFunctions::easeInOutCubic,
			EasingFunctions::easeInQuart,
			EasingFunctions::easeOutQuart,
			EasingFunctions::easeInOutQuart,
			EasingFunctions::easeInQuint,
			EasingFunctions::easeOutQuint,
			EasingFunctions::easeInOutQuint,
			EasingFunctions::easeInSine,
			EasingFunctions::easeOutSine,
			EasingFunctions::easeInOutSine,
			EasingFunctions::easeInExpo,
			EasingFunctions::easeOutExpo,
			EasingFunctions::easeInOutExpo,
			EasingFunctions::easeInCirc,
			EasingFunctions::easeOutCirc,
			EasingFunctions::easeInOutCirc,
		};
		assert(LN_ARRAY_SIZE_OF(table) == 22);
		return table[(int)easing];
	}
};

/** イージングによりシンプルに遷移する値のクラスです。 */
template < typename TValue, typename TTime = float >
class EasingValue
{
public:
    typedef TValue(*EasingFunction)(TTime, TValue, TValue, TTime);

    /** コンストラクタ */
    EasingValue()
        : m_startValue()
        , m_targetValue()
        , m_value()
        , m_currentTime(0.0)
        , m_totalTime(0.0)
        , m_wrapMode(AnimationWrapMode::Once)
        , m_easing(nullptr)
    {}

    /**
	 * 値の遷移を開始します。
	 * @param[in]	startValue	: 開始値
	 * @param[in]	targetValue	: 終了値
	 * @param[in]	time		: 変更にかける時間
	 */
    void start(const TValue& startValue, const TValue& targetValue, const TTime& time)
    {
        m_startValue = startValue;
        m_targetValue = targetValue;
        m_totalTime = time;
        m_currentTime = 0.0;
        setTime(m_currentTime);
    }

    /**
	 * 値の遷移を開始します。開始として現在の値を使用します。
	 * @param[in]	targetValue	: 終了値
	 * @param[in]	time		: 変更にかける時間
     */
    void start(const TValue& targetValue, const TTime& time)
    {
        m_startValue = m_value;
        m_targetValue = targetValue;
        m_totalTime = time;
        m_currentTime = 0.0;
        setTime(m_currentTime);
    }

	void reset(const TValue& value)
	{
		m_startValue = value;
		m_targetValue = value;
		m_value = value;
	}

    /** アニメーションの繰り返し方法を設定します。 */
    void setWrapMode(AnimationWrapMode mode)
    {
        m_wrapMode = mode;
    }

    /** イージング関数を設定します。 */
    void setEasingFunction(EasingFunction easing)
    {
        m_easing = easing;
    }

    /** 経過時間を設定します。 */
    void setTime(const TTime& time)
    {
        m_currentTime = time;
        updateValue();
    }

    /** 指定した値だけ時間を進めます。 */
    void advanceTime(const TTime& deltaTime)
    {
        m_currentTime += deltaTime;
        updateValue();
    }

    /**
	 * 現在の値を取得します。
	 * 
	 * 経過時間が 0 以前の場合は 開始時の値を、経過時間が 変更にかける時間以上 の場合は 終了時の値 を返します。
	 */
    const TValue& value() const { return m_value; }

    /** 開始値を取得します。 */
    const TValue& startValue() const { return m_startValue; }

    /** 終了値を取得します。 */
    const TValue& targetValue() const { return m_targetValue; }

    /** 現在の経過時間を取得します。 */
    const TTime& currentTime() const { return m_currentTime; }

    /** 値の遷移が完了しているかを確認します。 */
    bool isFinished() const { return (m_currentTime >= m_totalTime); }


private:

    void updateValue()
    {
        if (Math::nearEqual(m_totalTime, 0.0f)) {
            m_value = m_targetValue;
            return;
        }

        TTime time;
        switch (m_wrapMode)
        {
        case AnimationWrapMode::Once:
            time = m_currentTime;
            break;
        case AnimationWrapMode::Loop:
            if (m_currentTime > m_totalTime)
            {
                m_currentTime = 0;
            }
            time = m_currentTime;
            break;
        case AnimationWrapMode::Alternate:
            if (m_currentTime > m_totalTime)
            {
                m_currentTime = -m_currentTime;
            }
            time = std::abs(m_currentTime) / m_totalTime;
            break;
        }

        if (m_currentTime >= m_totalTime)
        {
            m_value = m_targetValue;
        }
        else if (m_currentTime <= 0 || m_totalTime <= 0)
        {
            m_value = m_startValue;
        }
        else if (m_easing != nullptr)
        {
            m_value = m_easing(m_currentTime, m_startValue, m_targetValue - m_startValue, m_totalTime);
        }
        else
        {
            m_value = m_startValue + (TValue)((m_targetValue - m_startValue) * (m_currentTime / m_totalTime));	// 線形補間
        }
    }

private:
    TValue m_startValue;
    TValue m_targetValue;
    TValue m_value;
    TTime m_currentTime;
    TTime m_totalTime;
    AnimationWrapMode m_wrapMode;
    EasingFunction m_easing;
};

} // namespace ln
