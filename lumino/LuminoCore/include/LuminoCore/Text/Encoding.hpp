// Copyright (c) 2018+ lriki. Distributed under the MIT license.
#pragma once
#include <vector>
#include "../Base/RefObject.hpp"

namespace ln {
class TextDecoder;
class TextEncoder;
class String;

/** エンコーディングの種類 */
LN_ENUM()
enum class EncodingType
{
    /** 不明な文字エンコーディング (判別失敗。またはバイナリファイル) */
    Unknown = 0,

    /** ASCII (拡張アスキーは含まない http://www.asciitable.com/) */
    ASCII,

    /** UTF-8 */
    UTF8,

    /** UTF-8 (BOM 無し) */
    UTF8N,

	/** 日本語 (シフト JIS) -- cp932(MS932) Windows-31J ※MS実装 */
	SJIS,
};

/** 文字コード変換のオプションを指定するための構造体 */
struct EncodingConversionOptions
{
    bool NullTerminated; /** 変換結果の終端に \0 文字を付加する */
};

/** 文字コード変換の結果を受け取るための構造体 */
struct EncodingConversionResult
{
    size_t BytesUsed;     /** 変換後の有効バイト数 */
    size_t CharsUsed;     /** 変換後の有効文字数(マルチバイト文字を考慮した文字数) */
    bool UsedDefaultChar; /** 変換不可文字をデフォルト文字('?') に変換したかどうか */
};

/** 内部文字コードへのデコード結果を受け取るための構造体 */
struct TextDecodeResult
{
    /** 消費バイト数 (入力バイト数に対して足りなければ変換エラーがある) */
    size_t usedByteCount;

    /** 変換結果のバイト数 */
    size_t outputByteCount;

    /** 変換結果の文字数 */
    size_t outputCharCount;
};

/** 内部文字コードからのエンコード結果を受け取るための構造体 */
struct TextEncodeResult
{
    /** 消費要素数数 (入力要素数に対して足りなければ変換エラーがある) */
    size_t usedElementCount;

    /** 変換結果のバイト数 */
    size_t outputByteCount;

    /** 変換結果の文字数 */
    size_t outputCharCount;
};

/**
 * 文字エンコーディングを表します。
 */
class LN_API TextEncoding : public RefObject
{
public:

	// TODO: 以下の const Ref<TextEncoding>& 戻り値は Rawポインタでかまわない

    /**
     * 環境依存のマルチバイト文字コードのエンコーディングを取得します。
     *
     * Windows 環境では CP_THREAD_ACP が示すコードページエンコーディングです。それ以外の環境では UTF-8 となります。        
     */
    static const Ref<TextEncoding>& systemMultiByteEncoding();

    /**
     * 環境依存のワイド文字のエンコーディングを取得します。
     *
     * wchar_t のサイズによって UTF-16 または UTF-32 となります。
     */
    static const Ref<TextEncoding>& wideCharEncoding();

    /** 互換性のために残されています */
    static const Ref<TextEncoding>& tcharEncoding();

    /**
     * UTF-8 エンコーディングを取得します。
     *
     * BOM (Byte order mark) は付きません。
     */
    static const Ref<TextEncoding>& utf8Encoding();

    /**
     * UTF-16 エンコーディングを取得します。
     *
     * このエンコーディングは ln::String や ln::Char のエンコーディングです。
     * エンディアンはプログラムの動作環境に依存します。
     * BOM (Byte order mark) は付きません。
     */
    static const Ref<TextEncoding>& utf16Encoding();

    /**
     * UTF-32 エンコーディングを取得します。
     *
     * エンディアンはプログラムの動作環境に依存します。
     * BOM (Byte order mark) は付きません。
     */
    static const Ref<TextEncoding>& utf32Encoding();

    /**
     * エンコーディングの種類を指定して TextEncoding オブジェクトを取得します。
     */
    static const Ref<TextEncoding>& getEncoding(EncodingType type);

    /**
     * テンプレート引数によって TextEncoding オブジェクトを取得します。
     */
    template<typename TChar>
    static const Ref<TextEncoding>& getEncodingTemplate();

    /**
     * エンコーディングを from から to へ fromByteCount バイト分だけ変換する際に必要な、変換先バッファの最大バイト数を取得します。
     */
    static size_t getConversionRequiredByteCount(TextEncoding* from, TextEncoding* to, size_t fromByteCount);

    /**
     * バイトシーケンスがどのようなエンコーディングで表されているかを簡易的に判別します。
     *
     * 判別できるエンコーディングは ASCII と UTF-8 です。それ以外の場合は systemMultiByteEncoding() で返されるエンコーディングとみなされます。
     */
    static const Ref<TextEncoding>& detectEncodingSimple(const char* str, int length, bool strict);

public:
    /** エンコード済みバイトシーケンスを String へ変換します。 */
    String decode(const byte_t* bytes, int length, int* outUsedDefaultCharCount = nullptr);

    /** String をバイトシーケンスへ変換します。 */
    std::vector<byte_t> encode(const String& str, int* outUsedDefaultCharCount = nullptr);

    /** Char 配列をバイトシーケンスへ変換します。 */
    std::vector<byte_t> encode(const Char* str, int length, int* outUsedDefaultCharCount = nullptr);

    /**
     * このエンコーディングの名前を取得します。
     *
     * 名前は IANA 文字セット名です。(http://www.iana.org/assignments/character-sets/character-sets.xhtml)
     * Windows コードページによるエンコーディングを使用している場合、"cp437" のようなエイリアス名になることがあります。    
     */
    virtual const String& name() const = 0;

    /** 1 文字の最小バイト数を取得します。 */
    virtual int minByteCount() const = 0;

    /** 1 文字の最大バイト数を取得します。 */
    virtual int maxByteCount() const = 0;

    /**
     * バイトシーケンスを内部文字列へ変換するデコーダを作成します。
     *
     * 作成したオブジェクトは、使い終えたら削除する必要があります。    
     */
    virtual TextDecoder* createDecoder() = 0;

    /**
     * 内部文字列をバイトシーケンスへ変換するエンコーダを作成する
     *
     * 作成したオブジェクトは、使い終えたら削除する必要があります。
     */
    virtual TextEncoder* createEncoder() = 0;

    /**
     * このエンコードを識別するための BOM (Byte order mark) を取得します。
     *
     * @return 0 で終わるバイト列。BOM を持たないエンコードの場合は nullptr
     */
    virtual byte_t* preamble() const = 0;

    /**
     * 指定されたバッファをこのエンコーディングで表現した際の文字数を取得します。
     *
     * @return 文字数。マルチバイトコードやサロゲートペアを考慮した文字数です。
     */
    virtual int getCharacterCount(const void* buffer, size_t bufferSize) const = 0;

    /**
     * 指定されたバッファの先頭がマルチバイトコードやサロゲートペアの先行文字である場合、追加で読むべき文字列長さを返します。
     *
     * @return 追加で読むべき文字列長さ。先行文字ではない場合は 0。
     */
    virtual int getLeadExtraLength(const void* buffer, size_t bufferSize) const = 0;

    /**
     * 指定されたバッファの先頭バイト列が、BOM と一致するかを確認します。
     *
     * @return 一致した場合は BOM のバイト数を返します。そうでなければ 0 を返します。
     */
    size_t checkPreamble(const void* buffer, size_t bufferSize) const;

    /** デコードまたはエンコード先にマッピングできない文字が見つかった時に置換する文字を設定します。 */
    void setFallbackReplacementChar(uint32_t ch) { m_fallbackReplacementChar = ch; }

    /** デコードまたはエンコード先にマッピングできない文字が見つかった時に置換する文字を取得します。 */
    uint32_t fallbackReplacementChar() const { return m_fallbackReplacementChar; }

    virtual bool convertToUTF16Stateless(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, TextDecodeResult* outResult);

protected:
    TextEncoding();
    virtual ~TextEncoding();

private:
    uint32_t m_fallbackReplacementChar;
};

/**
 * エンコード済みバイトシーケンスから内部文字コード (UTF16) への変換を行うクラス
 * 
 * TextDecoder は1つの文字列ストリームの変換中、任意の回数 convertToUTF16() を呼び出すことができます。
 * その間、変換ステータスを保存します。
 * つまり、バッファリングIO等で長大な文章を複数のメモリバッファに分割して読み取る場合に、
 * バッファの境界でマルチバイト文字が分かれてしまっても、継続して変換を行うことができます。
 * 
 * ただし、このステータス保存は canRemain() が true を返す場合のみ使用可能です。
 * false を返す場合はあらかじめ全てのソース文字列を1つの連続したメモリバッファに読み込み、convertToUTF16() に渡す必要があります。
 * 
 * 参考：.NET TextDecoder.convert()
 * http://msdn.microsoft.com/ja-jp/library/twte1yfs(v=vs.110).aspx
 */
class LN_API TextDecoder
{
public:
    /** この Decoder に対応する TextEncoding を取得します。 */
    TextEncoding* encoding() const { return m_encoding; }

    /** 変換状態を保持できるかを確認します。 */
    virtual bool canRemain() = 0;

    /**
     * このエンコーディングで表現される文字列バッファを、UTF16 に変換します。
     * 
     * @param[in]   input               : このエンコーディングで表現される文字列バッファ
     * @param[in]   inputByteSize       : input バッファのバイト数 (NULL 文字は含まない)
     * @param[out]  output              : 変換結果の格納先バッファ
     * @param[in]   outputElementSize   : output バッファのサイズ (文字数単位)
     * @param[out]  outResult           : 変換結果を格納する変数
     */
    virtual bool convertToUTF16(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, TextDecodeResult* outResult) = 0;

    /** 一連の convertToUTF16() の呼び出しの中で、変換できない文字を規定文字に変換した文字数を取得します。 */
    virtual int usedDefaultCharCount() = 0;

    /** 最後の convertToUTF16() で、バッファ末尾でマルチバイト文字が途切れていなければ true を返します。 */
    virtual bool completed() = 0;

    /** 変換ステータスをクリアし、初期状態に戻します。 */
    virtual void reset() = 0;

    virtual ~TextDecoder();

protected:
    TextDecoder(TextEncoding* encoding);

private:
    LN_DISALLOW_COPY_AND_ASSIGN(TextDecoder);
    Ref<TextEncoding> m_encoding;
};

/**
 * 内部文字コード (UTF16) をエンコードするクラス
 * 
 * TextDecoder 同様、canRemain() が true の場合は変換ステータスを保存します。
 */
class LN_API TextEncoder
{
public:
    /** この Encoder に対応する TextEncoding を取得します。 */
    TextEncoding* encoding() const { return m_encoding; }

    /** 変換状態を保持できるかを確認します。 */
    virtual bool canRemain() = 0;

    /**
     * UTF16 文字列を、このエンコーディングで表現される文字列に変換します。
     * 
     * @param[in]   input               : このエンコーディングで表現される文字列バッファ
     * @param[in]   inputElementSize    : input のサイズ (要素数単位。NULL 文字は含まない)
     * @param[out]  output              : 変換結果の格納先バッファ
     * @param[in]   outputByteSize      : output のバイト数
     * @param[out]  outResult           : 変換結果を格納する変数
     */
    virtual bool convertFromUTF16(const UTF16* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, TextEncodeResult* outResult) = 0;

    /** 一連の convertToUTF16() の呼び出しの中で、変換できない文字を規定文字に変換した文字数を取得します。 */
    virtual int usedDefaultCharCount() = 0;

    /**  最後の convertToUTF16() で、バッファ末尾でマルチバイト文字が途切れていなければ true を返します。 */
    virtual bool completed() = 0;

    /** 変換ステータスをクリアし、初期状態に戻します。 */
    virtual void reset() = 0;

    virtual ~TextEncoder();

protected:
    TextEncoder(TextEncoding* encoding);

private:
    LN_DISALLOW_COPY_AND_ASSIGN(TextEncoder);
    Ref<TextEncoding> m_encoding;
};

} // namespace ln
