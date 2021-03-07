#pragma once

namespace ln {

/** 経過時間を測定するための機能を提供します。 */
class ElapsedTimer
{
public:
	/** インスタンス構築と同時に、経過時間の計測を開始します。 */
	ElapsedTimer() LN_NOEXCEPT;

	ElapsedTimer(const char* tag) LN_NOEXCEPT;
	~ElapsedTimer() LN_NOEXCEPT;

	/** 経過時間をゼロにリセットして、経過時間の計測を開始します。 */
	void start() LN_NOEXCEPT;

	/** 経過時間の取得します。(ミリ秒単位) */
	uint64_t elapsedMilliseconds() const LN_NOEXCEPT;

	/** 経過時間の取得します。(マイクロ秒単位) */
	uint64_t elapsedMicroseconds() const LN_NOEXCEPT;

	/** 経過時間の取得します。(ナノ秒単位) */
	uint64_t elapsedNanoseconds() const LN_NOEXCEPT;

	double elapsed() const LN_NOEXCEPT { return static_cast<double>(elapsedNanoseconds()) / 1000000000; }

private:
	uint64_t m_value1;
	uint64_t m_value2;
	const char* m_tag;
};

} // namespace ln

