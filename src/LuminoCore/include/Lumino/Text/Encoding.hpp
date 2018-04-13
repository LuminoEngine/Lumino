// Copyright (c) 2018 lriki. Distributed under the MIT license.

#pragma once
#include <vector>

namespace ln {
class TextDecoder;
class TextEncoder;
class String;

/** エンコーディングの種類 */
enum class EncodingType
{
	Unknown = 0,	/** 不明な文字コード (判別失敗。またはバイナリファイル) */

	ASCII,			/** ASCII (拡張アスキーは含まない http://www.asciitable.com/) */

	UTF8,
	UTF8N,
	UTF16L,
	UTF16B,
	//UTF16LN,
	//UTF16BN,
	UTF32L,
	UTF32B,
	//UTF32LN,
	//UTF32BN,

	//SJIS,		/** 日本語 (シフト JIS) -- cp932(MS932) Windows-31J ※MS実装 */
	//EUCJP,		/** 日本語 (EUC-JP) */
	//GB2312,		/** 簡体字中国語 (GB2312) -- cp936(MS936) */
	//EUCKR,		/** 韓国語 (EUC-KR)(=KSX1001) -- cp949(MS949) */
	//BIG5,		/** 繁体字中国語 (Big5) -- cp950(MS950) */
};

/** 文字コード変換のオプションを指定するための構造体 */
struct EncodingConversionOptions
{
	bool	NullTerminated;		/** 変換結果の終端に \0 文字を付加する */
};

/** 文字コード変換の結果を受け取るための構造体 */
struct EncodingConversionResult
{
	size_t	BytesUsed;			/** 変換後の有効バイト数 */
	size_t	CharsUsed;			/** 変換後の有効文字数(マルチバイト文字を考慮した文字数) */
	bool	UsedDefaultChar;	/** 変換不可文字をデフォルト文字('?') に変換したかどうか */
};

/**
	@brief		文字エンコーディングを表します。
	@attention	このクラスの各 static メソッドが返す TextEncoding オブジェクトはグローバルなインスタンスです。返されたオブジェクトは削除してはなりません。
*/
class LN_API TextEncoding
{
public:

	/**
		@brief		環境依存のマルチバイト文字コードのエンコーディングを取得します。
		@details	char 型のエンコーディングとみなすことができます。
					Windows 環境では CP_THREAD_ACP が示すコードページエンコーディングです。それ以外の環境では UTF-8 となります。
	*/
	static TextEncoding* systemMultiByteEncoding();
	
	/**
		@brief		環境依存のワイド文字のエンコーディングを取得します。
		@details	wchar_t 型のエンコーディングとみなすことができます。
					wchar_t のサイズによって UTF-16 または UTF-32 となります。
	*/
	static TextEncoding* wideCharEncoding();

	/* 互換性のために残されています */
	static TextEncoding* tcharEncoding();

	/**
		@brief		UTF-8 (BOM 無し)エンコーディングを取得します。
	*/
	static TextEncoding* utf8Encoding();

	/**
		@brief		UTF-16 (Little) エンコーディングを取得します。
	*/
	static TextEncoding* utf16Encoding();

	/**
		@brief		UTF-32 (Little) エンコーディングを取得します。
	*/
	static TextEncoding* utf32Encoding();

	/**
		@brief		種類を指定してエンコーディングを取得します。
	*/
	static TextEncoding* getEncoding(EncodingType type);

	/**
		@brief		テンプレート引数によってエンコーディングを取得します。
	*/
	template<typename TChar>
	static TextEncoding* getEncodingTemplate();

	/**
		@brief		文字コードを from から to へ fromByteCount バイト分だけ変換する際に必要な、変換先バッファの最大バイト数を取得します。
	*/
	static size_t getConversionRequiredByteCount(TextEncoding* from, TextEncoding* to, size_t fromByteCount);

	static String fromBytes(const char* bytes, int size, TextEncoding* encoding = nullptr, bool* outUsedDefaultChar = nullptr);
	static String fromBytes(const byte_t* bytes, int size, TextEncoding* encoding = nullptr, bool* outUsedDefaultChar = nullptr);

	static TextEncoding* detectEncodingSimple(const char* str, int length, bool strict);

public:

	/** エンコード済みバイトシーケンスを String へ変換します。 */
	String decode(const byte_t* bytes, int length, int* outUsedDefaultCharCount = nullptr) const;

	/** String をバイトシーケンスへ変換します。 */
	std::vector<byte_t> encode(const String& str, int* outUsedDefaultCharCount = nullptr) const;

	/**
		@brief		このエンコーディングの名前を取得します。
		@details	名前は IANA 文字セット名です。(http://www.iana.org/assignments/character-sets/character-sets.xhtml)
					Windows コードページによるエンコーディングを使用している場合、"cp437" のようなエイリアス名になることがあります。
	*/
	virtual const TTCHAR* getName() const = 0;

	/**
		@brief		1 文字の最小バイト数を取得する
	*/
	virtual int getMinByteCount() const = 0;

	/**
		@brief		1 文字の最大バイト数を取得する
	*/
	virtual int getMaxByteCount() const = 0;
	
	/**
		@brief		デコードまたはエンコード先にマッピングできない文字が見つかった時に置換する文字の設定
	*/
	virtual void setFallbackReplacementChar(uint32_t ch) { mFallbackReplacementChar = ch; }

	/**
		@brief		デコードまたはエンコード先にマッピングできない文字が見つかった時に置換する文字の取得
	*/
	virtual uint32_t getFallbackReplacementChar() { return mFallbackReplacementChar; }

	/**
		@brief		バイトシーケンスを内部文字列へ変換するデコーダを作成する
		@details	作成したオブジェクトは、使い終えたら削除する必要があります。
	*/
	virtual TextDecoder* createDecoder() const = 0;

	/**
		@brief		内部文字列をバイトシーケンスへ変換するエンコーダを作成する
		@details	作成したオブジェクトは、使い終えたら削除する必要があります。
	*/
	virtual TextEncoder* createEncoder() const = 0;

	/**
		@brief		このエンコードを識別するための前文文字列 (BOM) を取得する
		@return		NULL で終わるバイト列。BOM を持たないエンコードの場合は NULL
	*/
	virtual byte_t* getPreamble() const = 0;

	/**
		@brief		指定されたバッファの先頭バイト列が、前文文字列 (BOM) と一致するかを確認します。
		@return		一致した場合は前文文字列のバイト数を返します。そうでなければ 0 を返します。
	*/
	size_t checkPreamble(const void* buffer, size_t bufferSize) const;

	/**
		@brief		指定されたバッファをこのエンコーディングで表現した際の文字数を取得します。
		@return		文字数。マルチバイトコードやサロゲートペアを考慮した文字数です。
	*/
	virtual int getCharacterCount(const void* buffer, size_t bufferSize) const = 0;

	/**
		@brief		指定されたバッファの先頭がマルチバイトコードやサロゲートペアの先行文字である場合、追加で読むべき文字列長さを返します。
		@return		追加で読むべき文字列長さ。先行文字ではない場合は 0。
	*/
	virtual int getLeadExtraLength(const void* buffer, size_t bufferSize) const = 0;

protected:
	TextEncoding();
	virtual ~TextEncoding();

	uint32_t	mFallbackReplacementChar;

private:
	LN_DISALLOW_COPY_AND_ASSIGN(TextEncoding);
};

/**
	@brief		エンコード済みバイトシーケンスから内部文字コード (UTF16) への変換を行うクラス

	@details	TextDecoder は1つの文字列ストリームの変換中、任意の回数 convertToUTF16() を呼び出すことができます。
				その間、変換ステータスを保存します。
				つまり、バッファリングIO等で長大な文章を複数のメモリバッファに分割して読み取る場合に、
				バッファの境界でマルチバイト文字が分かれてしまっても、継続して変換を行うことができます。

				ただし、このステータス保存は canRemain() が true を返す場合のみ使用可能です。
				false を返す場合はあらかじめ全てのソース文字列を1つの連続したメモリバッファに読み込み、
				convertToUTF16() に渡す必要があります。

				参考：.NET TextDecoder.convert()
				http://msdn.microsoft.com/ja-jp/library/twte1yfs(v=vs.110).aspx
*/
class LN_API TextDecoder
{
public:

	struct DecodeResult
	{
		size_t usedByteCount;		// 消費バイト数 (足りなければ変換エラーがある)
		size_t outputByteCount;		// 変換結果のバイト数を格納する
		size_t outputCharCount;		// 変換結果の文字数を格納する
	};

	/**
		@brief		1 文字の最小バイト数を取得します。
	*/
	virtual int getMinByteCount() = 0;

	/**
		@brief		1 文字の最大バイト数を取得します。
	*/
	virtual int getMaxByteCount() = 0;

	/**
		@brief		TextDecoder が変換状態を保持できるかを確認します。
	*/
	virtual bool canRemain() = 0;

	/**
		@brief		デコード先にマッピングできない文字が見つかった時に置換する文字の設定
	*/
	virtual void setFallbackReplacementChar(uint32_t ch) { mFallbackReplacementChar = ch; }

	/**
		@brief		デコード先にマッピングできない文字が見つかった時に置換する文字の取得
	*/
	virtual uint32_t getFallbackReplacementChar() { return mFallbackReplacementChar; }

	/**
		@brief		このエンコーディングで表現される文字列バッファを、UTF16 に変換する
		@param[in]	input				: このエンコーディングで表現される文字列バッファ
		@param[in]	inputByteSize		: input バッファのバイト数 (NULL 文字は含まない)
		@param[out]	output				: 変換結果の格納先バッファ
		@param[in]	outputElementSize	: output バッファのサイズ (文字数単位)
		@param[out]	outResult			: 変換結果を格納する変数
	*/
	virtual bool convertToUTF16(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, DecodeResult* outResult) = 0;

	/**
		@brief	一連の convertToUTF16() の呼び出しの中で、変換できない文字を規定文字に変換した文字数を取得する
	*/
	virtual int usedDefaultCharCount() = 0;

	/**
		@brief	最後の convertToUTF16() で、バッファ末尾でマルチバイト文字が途切れていなければ true を返す
	*/
	virtual bool completed() = 0;

	/**
		@brief	変換ステータスをクリアし、初期状態に戻す
	*/
	virtual void reset() = 0;

	virtual ~TextDecoder();

protected:
	TextDecoder();

	uint32_t	mFallbackReplacementChar;

private:
	LN_DISALLOW_COPY_AND_ASSIGN(TextDecoder);
};

/**
	@brief		内部文字コード (UTF16) をエンコードするクラス
	@details	TextDecoder 同様、canRemain() が true の場合は変換ステータスを保存します。
*/
class LN_API TextEncoder
{
public:
	struct EncodeResult
	{
		size_t usedElementCount;	// 消費要素数 (足りなければ変換エラーがある)
		size_t outputByteCount;		// 変換結果のバイト数を格納する
		size_t outputCharCount;		// 変換結果の文字数を格納する
	};

	/**
		@brief		1 文字の最小バイト数を取得します。
	*/
	virtual int getMinByteCount() = 0;

	/**
		@brief		1 文字の最大バイト数を取得します。
	*/
	virtual int getMaxByteCount() = 0;

	/**
		@brief	TextEncoder が変換状態を保持できるかを確認します。
	*/
	virtual bool canRemain() = 0;

	/**
		@brief		エンコード先にマッピングできない文字が見つかった時に置換する文字の設定
	*/
	virtual void setFallbackReplacementChar(uint32_t ch) { mFallbackReplacementChar = ch; }

	/**
		@brief		エンコード先にマッピングできない文字が見つかった時に置換する文字の取得
	*/
	virtual uint32_t getFallbackReplacementChar() { return mFallbackReplacementChar; }

	/**
		@brief		UTF16 文字列を、このエンコーディングで表現される文字列に変換する
		@param[in]	input				: このエンコーディングで表現される文字列バッファ
		@param[in]	inputElementSize	: input のサイズ (要素数単位。NULL 文字は含まない)
		@param[out]	output				: 変換結果の格納先バッファ
		@param[in]	outputByteSize		: output のバイト数
		@param[out]	outResult			: 変換結果を格納する変数
	*/
	virtual bool convertFromUTF16(const UTF16* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, EncodeResult* outResult) = 0;

	/**
		@brief	一連の convertToUTF16() の呼び出しの中で、変換できない文字を規定文字に変換した文字数を取得する
	*/
	virtual int usedDefaultCharCount() = 0;

	/**
		@brief	最後の convertToUTF16() で、バッファ末尾でマルチバイト文字が途切れていなければ true を返す
	*/
	virtual bool completed() = 0;

	/**
		@brief	変換ステータスをクリアし、初期状態に戻す
	*/
	virtual void reset() = 0;

	virtual ~TextEncoder();

protected:
	TextEncoder();

protected:
	uint32_t	mFallbackReplacementChar;

private:
	LN_DISALLOW_COPY_AND_ASSIGN(TextEncoder);
};

} // namespace ln
