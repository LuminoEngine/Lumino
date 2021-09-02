/*
エラー処理の方針 (テスト運用中)
========

### 例外 (LN_REQUIRE 系マクロ)
前提条件のミスで使用する。プログラマが悪い系のエラー。
一応例外を発生させているが、assert に直してもユーザーストーリーとしては影響ないようなものに使う。

### Result 戻り値
入力された引数やデータが悪い系のエラーで使用する。
いわゆるビジネスエラー。

return Result::Fail; する前に、原因となった個所では必ず CLI::error() して問題を通知する。
※関数を使う側としては、↑のようにログが出て失敗する可能性があることを知ることができるようにする。bool はこの目的では使わない。


### 将来的な共有モジュール化
Project とかあたりは、今後作成予定の GUI ツールでも共有したい。
で、多分 Qt Widgets 使うことになる。が、これは例外中立であって例外安全ではない。
例外したら基本的にアプリ終了、が良い流れ。
http://doc.qt.io/qt-5/exceptionsafety.html
イベントハンドラの中で、必ず、キャンセルするのか再試行させるのか判断しなければならない。

*/

#pragma once

namespace ln {

enum class ResultCode
{
	Fail,
	Success,
};

class Result
{
public:
	static const Result Fail;
	static const Result Success;

    Result(bool result)
        : m_code(result ? ResultCode::Success : ResultCode::Fail)
    {}

	Result(ResultCode code)
		: m_code(code)
	{}

	Result(const Result& other)
		: m_code(other.m_code)
	{}

	Result& operator=(const Result& other)
	{
		m_code = other.m_code;
		return *this;
	}

	operator bool() const { return m_code != ResultCode::Fail; }

private:
	ResultCode m_code;
};

} // namespace ln

