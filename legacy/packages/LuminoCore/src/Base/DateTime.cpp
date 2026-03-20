/*
[2022/2/21]  Design Note
----------
参考にできるのは C# と Rust の DateTime だろうか。
JavaScript の Date は使い方に注意が多いらしい。

	JavaScript の Date は罠が多すぎる
	https://qiita.com/labocho/items/5fbaa0491b67221419b4

C# の DateTime にはタイムゾーン情報が付いていない。
そのため計算するときは UTC か Local のどちらかに合わせてから計算する必要がある。
この種類は Kind プロパティで確認できる。

Rust の DateTime はテンプレートであり、型引数としてタイムゾーン情報を指定する。
このためそもそも異なったタイムゾーン同士の計算はコンパイルエラーとして検出できる。

time_t について
----------
C++の仕様としては、基準は明記されていない。（ほとんどのシステムでは 協定世界時(UTC) です）
https://en.cppreference.com/w/c/chrono/gmtime

Linuxの場合、man には「time() は、紀元 (Epoch; 1970-01-01 00:00:00 (UTC)) からの秒数で返す」とある。
https://linuxjm.osdn.jp/html/LDP_man-pages/man2/time.2.html

Windowsの場合、「time 関数は、協定世界時刻 (UTC) の 1970 年 1 月 1 日午前 0 時 (00:00:00) 以降の経過時間をシステム クロックに従って返します」とある。
https://docs.microsoft.com/ja-jp/cpp/c-runtime-library/reference/time-time32-time64?view=msvc-170

macOS も同様。
https://developer.apple.com/library/archive/documentation/System/Conceptual/ManPages_iPhoneOS/man3/time.3.html

Lumino が相手にするシステムとしては、time_t = UTC と考えて問題ない。
もし、もしこれ以外のシステムを相手にする場合、Lumino は使えない、ということになる。
*/

#include "Internal.hpp"
#include <LuminoCore/Base/DateTime.hpp>

namespace ln {

//==============================================================================
// DateTime

DateTime DateTime::now() {
    return DateTime(::time(nullptr));
}

DateTime::DateTime()
    : m_time(0) {
}

DateTime::DateTime(time_t t)
    : m_time(t) {
}

String DateTime::toString() const {
    tm* t = ::localtime(&m_time);
    char buf[128];
    strftime(buf, sizeof(buf), "%Y/%m/%d %H:%M:%S", t);
    return String::fromCString(buf);
}

} // namespace ln
