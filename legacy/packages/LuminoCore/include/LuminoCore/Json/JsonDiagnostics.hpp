// Copyright (c) 2018+ lriki. Distributed under the MIT license..
#pragma once
#include "../Base/EnumFlags.hpp"
#include "../Base/String.hpp"

namespace ln {

/** JSON 解析のエラーコード */
enum class JsonDiagCode
{
    /** エラーは発生していない */
    None,

    /** 文字列の終端が見つかる前に EOF が見つかった。 */
    UnterminatedString,

    /** 文字列内に JSON では使用できない文字が見つかった。(制御文字など) */
    InvalidStringChar,

    /** 文字列内に無効なエスケープシーケンスが見つかった。 */
    InvalidStringEscape,

    /** 予期しないトークンが見つかった。 */
    UnexpectedToken,

    /** 不正な数値形式。 */
    InvalidNumber,

    /** 数値の変換でオーバーフローが発生した。 */
    NumberOverflow,

    /** オブジェクトが正しく閉じられていない。 */
    InvalidObjectClosing,

    /** 配列が正しく閉じられていない。 */
    ArrayInvalidClosing,

    /** 無効な値 */
    ValueInvalid,
};

/** JSON 解析中に発生した問題を表します。 */
class JsonDiag
{
public:
    JsonDiagCode code;
    int line;
    int column;
    String message;

    JsonDiag();
};

} // namespace ln
