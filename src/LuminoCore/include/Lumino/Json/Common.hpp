// Copyright (c) 2018 lriki. Distributed under the MIT license.

#pragma once

namespace ln {
namespace tr {

enum class JsonToken
{
	None = 0,

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


enum class JsonFormatting
{
	None = 0,
	Indented = 1,
};

} // namespace tr
} // namespace ln

