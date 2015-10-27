
#pragma once

LN_NAMESPACE_BEGIN

/**
	@brief      イージングによりシンプルに遷移する値のクラスです。
*/
template < typename TValue, typename TTime = double >
class EasingValue
{
public:
	typedef TValue(*EasingFunction)(TValue, TValue, TValue, TValue);

public:

	/**
		@brief      コンストラクタ
	*/
	EasingValue()
		: m_startValue()
		, m_targetValue()
		, m_value()
		, m_currentTime( 0.0 )
		, m_totalTime( 0.0 )
		, m_easing(NULL)
	{}

	/**
		@brief      値の遷移を開始します。
		@param[in]	startValue	: 開始値
		@param[in]	targetValue	: 終了値
		@param[in]	time		: 変更にかける時間
	*/
	void Start(const TValue& startValue, const TValue& targetValue, const TTime& time)
	{
		m_startValue = startValue;
		m_targetValue = targetValue;
		m_totalTime = time;
		m_currentTime = 0.0;
		SetTime(m_currentTime);
	}

	/**
		@brief      値の遷移を開始します。開始として現在の値を使用します。
		@param[in]	targetValue	: 終了値
		@param[in]	time		: 変更にかける時間
	*/
	void Start(const TValue& targetValue, const TTime& time)
	{
		m_startValue = m_value;
		m_targetValue = targetValue;
		m_totalTime = time;
		m_currentTime = 0.0;
		SetTime(m_currentTime);
	}

	/**
		@brief      イージング関数を設定します。
	*/
	void SetEasingFunction(EasingFunction easing)
	{
		m_easing = easing;
	}

	/**
		@brief      経過時間を設定します。
	*/
	void SetTime(const TTime& time)
	{
		m_currentTime = time;
		UpdateValue();
	}

	/**
		@brief      指定した値だけ時間を進めます。
	*/
	void AdvanceTime(const TTime& deltaTime)
	{
		m_currentTime += deltaTime;
		UpdateValue();
	}

	/**
		@brief      現在の値を取得します。
		@details	経過時間が 0 以前の場合は 開始時の値を、経過時間が 変更にかける時間以上 の場合は 終了時の値 を返します。
	*/
	const TValue& GetValue() const { return m_value; }

	/**
		@brief      開始値を取得します。
	*/
	const TValue& GetStartValue() const { return m_startValue; }
	
	/**
		@brief      終了値を取得します。
	*/
	const TValue& GetTargetValue() const { return m_targetValue; }

	/**
		@brief      現在の経過時間を取得します。
	*/
	const TTime& GetTime() const { return m_currentTime; }

	/**
		@brief      値の遷移が完了しているかを確認します。
	*/
	bool IsFinished() const { return (m_currentTime >= m_totalTime); }

	
private:

	void UpdateValue()
	{
		if (m_currentTime >= m_totalTime)
		{
			m_value = m_targetValue;
		}
		else if (m_currentTime <= 0 || m_totalTime <= 0)
		{
			m_value = m_startValue;
		}
		else if (m_easing != NULL)
		{
			m_value = m_easing(m_currentTime, m_startValue, m_targetValue - m_startValue, m_totalTime);
		}
		else
		{
			m_value = m_startValue + (TValue)((m_targetValue - m_startValue) * (m_currentTime / m_totalTime));	// 線形補間
		}
	}

private:
	TValue			m_startValue;
	TValue			m_targetValue;
	TValue			m_value;
	TTime			m_currentTime;
	TTime			m_totalTime;
	EasingFunction	m_easing;
};

LN_NAMESPACE_END
