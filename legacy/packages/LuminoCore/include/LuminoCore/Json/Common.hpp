// Copyright (c) 2018+ lriki. Distributed under the MIT license...
#pragma once

namespace ln {

/** Json を構成するノードの種類 */
enum class JsonNode
{
    /** 無効値 */
    None,

    /** オブジェクト定義の開始 ('{') */
    StartObject,

    /** オブジェクト定義の終了 ('}') */
    EndObject,

    /** 配列定義の開始 ('[') */
    StartArray,

    /** 配列定義の終了 (']') */
    EndArray,

    /** オブジェクトのプロパティの名前 */
    PropertyName,

    /** 数値型 (Int32) */
    Int32,

    /** 数値型 (Int64) */
    Int64,

    /** 数値型 (float) */
    Float,

    /** 数値型 (double) */
    Double,

    /** "null" 値 */
    Null,

    /** 真偽値 ("true" または "false"。GetValue() で値を確認すること) */
    Boolean,

    /** 文字列値 */
    String,
};

enum class JsonParseResult
{
    Success = 0,
    Error = 1,
};

/** JSON 文字列の整形方法 */
enum class JsonFormatting
{
    None,
    Indented,
};

} // namespace ln
