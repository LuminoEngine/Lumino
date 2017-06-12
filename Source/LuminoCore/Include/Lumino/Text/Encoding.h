
#pragma once

LN_NAMESPACE_BEGIN

class ByteBuffer;
class Decoder;
class Encoder;

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

	SJIS,		/** 日本語 (シフト JIS) -- cp932(MS932) Windows-31J ※MS実装 */
	EUCJP,		/** 日本語 (EUC-JP) */
	GB2312,		/** 簡体字中国語 (GB2312) -- cp936(MS936) */
	EUCKR,		/** 韓国語 (EUC-KR)(=KSX1001) -- cp949(MS949) */
	BIG5,		/** 繁体字中国語 (Big5) -- cp950(MS950) */

	TERMINATOR,	/* (terminator) */
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
	@attention	このクラスの各 static メソッドが返す Encoding オブジェクトはグローバルなインスタンスです。返されたオブジェクトは削除してはなりません。
*/
class Encoding
{
public:

	/**
		@brief		環境依存のマルチバイト文字コードのエンコーディングを取得します。
		@details	char 型のエンコーディングとみなすことができます。
					Windows 環境では CP_THREAD_ACP が示すコードページエンコーディングです。それ以外の環境では UTF-8 となります。
	*/
	static Encoding* GetSystemMultiByteEncoding();

	/**
		@brief		環境依存のワイド文字のエンコーディングを取得します。
		@details	wchar_t 型のエンコーディングとみなすことができます。
					wchar_t のサイズによって UTF-16 または UTF-32 となります。
	*/
	static Encoding* GetWideCharEncoding();

	/**
		@brief		TCHAR 型文字のエンコーディングを取得します。
	*/
	static Encoding* GetTCharEncoding();

	/**
		@brief		UTF-8 (BOM 無し)エンコーディングを取得します。
	*/
	static Encoding* GetUTF8Encoding();

	/**
		@brief		UTF-16 (Little) エンコーディングを取得します。
	*/
	static Encoding* GetUTF16Encoding();

	/**
		@brief		UTF-16 (Little) エンコーディングを取得します。
	*/
	static Encoding* GetUTF32Encoding();

	/**
		@brief		種類を指定してエンコーディングを取得します。
	*/
	static Encoding* getEncoding(EncodingType type);

	/**
		@brief		テンプレート引数によってエンコーディングを取得します。
		@details	指定できる型は char または wchar_t です。
					それぞれ GetSystemMultiByteEncoding()、GetWideCharEncoding() を返します。
	*/
	template<typename TChar>
	static Encoding* GetEncodingTemplate();

#ifdef LN_OS_WIN32
	/**
		@brief		現在のスレッドロケールに基づくデフォルトコードページの Encoding を取得します。
	*/
	static Encoding* GetWin32DefaultCodePageEncoding();
#endif

	/**
		@brief		文字コードを from から to へ fromByteCount バイト分だけ変換する際に必要な、変換先バッファの最大バイト数を取得します。
	*/
	static size_t GetConversionRequiredByteCount(Encoding* from, Encoding* to, size_t fromByteCount);

	/**
		@brief		文字コードを変換する (不正シーケンスがあったら例外)
		@param[in]	src				: 変換元
		@param[in]	srcByteCount	: 変換元のバイト数
		@param[in]	srcEncoding		: 変換元のエンコーディング
		@param[in]	targetEncoding	: 変換先のエンコーディング
		@param[out]	result			: 変換の結果情報
		@return		\0終端文字は付加されません。GetSize() により使用バイト数を確認できます。
					使用後、Release() で開放する必要があります。
	*/
	static ByteBuffer Convert(
		const void* src, size_t srcByteCount, const Encoding* srcEncoding,
		const Encoding* targetEncoding,
		const EncodingConversionOptions& options,
		EncodingConversionResult* result);

	/**
		@brief		Encoder と Decoder を指定して文字コードを変換する (不正シーケンスがあったら例外)
		@param[in]	src				: 変換元
		@param[in]	srcByteCount	: 変換元のバイト数
		@param[in]	decoder			: src を中間フォーマットに変換するデコーダ
		@param[in]	encoder			: 中間フォーマットをターゲットに変換するエンコーダ
		@param[out]	result			: 変換の結果情報
		@details	指定された Encoder と Decoder は、変換ステータスを保持できる場合は保持します。
		@return		\0終端文字は付加されません。GetSize() により使用バイト数を確認できます。
					使用後、Release() で開放する必要があります。
	*/
	static ByteBuffer Convert(
		const void* src, size_t srcByteCount, Decoder* decoder,
		Encoder* encoder,
		const EncodingConversionOptions& options,
		EncodingConversionResult* result);

	/**
		@brief		文字コードを変換します。
		@param[in]	src				: 変換元
		@param[in]	srcByteCount	: src のバイト数
		@param[in]	srcEncoding		: src のエンコーディング
		@param[in]	dest			: 変換結果を格納するバッファ
		@param[in]	destByteCount	: dest のバイト数
		@param[in]	destEncoding	: dest のエンコーディング
		@param[out]	result			: 変換の結果情報
		@details	\0終端文字は付加されません。result.BytesUsed により使用バイト数を確認できます。
	*/
	static void Convert(
		const void* src, size_t srcByteCount, Encoding* srcEncoding,
		void* dest, size_t destByteCount, Encoding* destEncoding,
		EncodingConversionResult* result);

	/**
		@brief		文字コードを変換します。
		@param[in]	src				: 変換元
		@param[in]	srcByteCount	: src のバイト数
		@param[in]	srcDecoder		: src に対するデコーダ
		@param[in]	dest			: 変換結果を格納するバッファ
		@param[in]	destByteCount	: dest のバイト数
		@param[in]	destEncoder		: dest に対するエンコーダ
		@param[out]	result			: 変換の結果情報
		@details	指定された Encoder と Decoder は、変換ステータスを保持できる場合は保持します。
					\0終端文字は付加されません。result.BytesUsed により使用バイト数を確認できます。
	*/
	static void Convert(
		const void* src, size_t srcByteCount, Decoder* srcDecoder,
		void* dest, size_t destByteCount, Encoder* destEncoder,
		EncodingConversionResult* result);

public:

	/**
		@brief		このエンコーディングの名前を取得します。
		@details	名前は IANA 文字セット名です。(http://www.iana.org/assignments/character-sets/character-sets.xhtml)
					Windows コードページによるエンコーディングを使用している場合、"cp437" のようなエイリアス名になることがあります。
	*/
	virtual const TCHAR* GetName() const = 0;

	/**
		@brief		1 文字の最小バイト数を取得する
	*/
	virtual int GetMinByteCount() const = 0;

	/**
		@brief		1 文字の最大バイト数を取得する
	*/
	virtual int GetMaxByteCount() const = 0;
	
	/**
		@brief		デコードまたはエンコード先にマッピングできない文字が見つかった時に置換する文字の設定
	*/
	virtual void SetFallbackReplacementChar(uint32_t ch) { mFallbackReplacementChar = ch; }

	/**
		@brief		デコードまたはエンコード先にマッピングできない文字が見つかった時に置換する文字の取得
	*/
	virtual uint32_t GetFallbackReplacementChar() { return mFallbackReplacementChar; }

	/**
		@brief		バイトシーケンスを内部文字列へ変換するデコーダを作成する
		@details	作成したオブジェクトは、使い終えたら削除する必要があります。
	*/
	virtual Decoder* CreateDecoder() const = 0;

	/**
		@brief		内部文字列をバイトシーケンスへ変換するエンコーダを作成する
		@details	作成したオブジェクトは、使い終えたら削除する必要があります。
	*/
	virtual Encoder* CreateEncoder() const = 0;

	/**
		@brief		このエンコードを識別するための前文文字列 (BOM) を取得する
		@return		NULL で終わるバイト列。BOM を持たないエンコードの場合は NULL
	*/
	virtual byte_t* GetPreamble() const = 0;

	/**
		@brief		指定されたバッファの先頭バイト列が、前文文字列 (BOM) と一致するかを確認します。
		@return		一致した場合は前文文字列のバイト数を返します。そうでなければ 0 を返します。
	*/
	size_t CheckPreamble(const void* buffer, size_t bufferSize) const;

	/**
		@brief		指定されたバッファをこのエンコーディングで表現した際の文字数を取得します。
		@return		文字数。マルチバイトコードやサロゲートペアを考慮した文字数です。
	*/
	virtual int GetCharacterCount(const void* buffer, size_t bufferSize) const = 0;

	/**
		@brief		指定されたバッファの先頭がマルチバイトコードやサロゲートペアの先行文字である場合、追加で読むべき文字列長さを返します。
		@return		追加で読むべき文字列長さ。先行文字ではない場合は 0。
	*/
	virtual int GetLeadExtraLength(const void* buffer, size_t bufferSize) const = 0;

protected:
	Encoding() : mFallbackReplacementChar((uint32_t)'?') {};
	virtual ~Encoding() {};

	uint32_t	mFallbackReplacementChar;

private:
	LN_DISALLOW_COPY_AND_ASSIGN(Encoding);
};




/*===============================================================================
	以下は内部用クラス。新しい文字コードに対応する場合は拡張する必要があるが、
	基本的には意識しなくてOK。
===============================================================================*/

/**
	@brief		エンコード済みバイトシーケンスから内部文字コード (UTF16) への変換を行うクラス

	@details	Decoder は1つの文字列ストリームの変換中、任意の回数 ConvertToUTF16() を呼び出すことができます。
				その間、変換ステータスを保存します。
				つまり、バッファリングIO等で長大な文章を複数のメモリバッファに分割して読み取る場合に、
				バッファの境界でマルチバイト文字が分かれてしまっても、継続して変換を行うことができます。

				ただし、このステータス保存は CanRemain() が true を返す場合のみ使用可能です。
				false を返す場合はあらかじめ全てのソース文字列を1つの連続したメモリバッファに読み込み、
				ConvertToUTF16() に渡す必要があります。

				参考：.NET Decoder.Convert()
				http://msdn.microsoft.com/ja-jp/library/twte1yfs(v=vs.110).aspx
*/
class Decoder
{
public:

	/**
		@brief		1 文字の最小バイト数を取得します。
	*/
	virtual int GetMinByteCount() = 0;

	/**
		@brief		1 文字の最大バイト数を取得します。
	*/
	virtual int GetMaxByteCount() = 0;

	/**
		@brief		Decoder が変換状態を保持できるかを確認します。
	*/
	virtual bool CanRemain() = 0;

	/**
		@brief		デコード先にマッピングできない文字が見つかった時に置換する文字の設定
	*/
	virtual void SetFallbackReplacementChar(uint32_t ch) { mFallbackReplacementChar = ch; }

	/**
		@brief		デコード先にマッピングできない文字が見つかった時に置換する文字の取得
	*/
	virtual uint32_t GetFallbackReplacementChar() { return mFallbackReplacementChar; }

	/**
		@brief		このエンコーディングで表現される文字列バッファを、UTF16 に変換する
		@param[in]	input				: このエンコーディングで表現される文字列バッファ
		@param[in]	inputByteSize		: input バッファのバイト数 (NULL 文字は含まない)
		@param[out]	output				: 変換結果の格納先バッファ
		@param[in]	outputElementSize	: output バッファのサイズ (文字数単位)
		@param[out]	outBytesUsed		: 変換結果のバイト数を格納する変数
		@param[out]	outCharsUsed		: 変換結果の文字数を格納する変数
	*/
	virtual void ConvertToUTF16(const byte_t* input, size_t inputByteSize, UTF16* output, size_t outputElementSize, size_t* outBytesUsed, size_t* outCharsUsed) = 0;

	/**
		@brief	一連の ConvertToUTF16() の呼び出しの中で、変換できない文字を規定文字に変換した文字数を取得する
	*/
	virtual int UsedDefaultCharCount() = 0;	// TODO: Get～

	/**
		@brief	最後の ConvertToUTF16() で、バッファ末尾でマルチバイト文字が途切れていなければ true を返す
	*/
	virtual bool Completed() = 0;

	/**
		@brief	変換ステータスをクリアし、初期状態に戻す
	*/
	virtual void Reset() = 0;

	virtual ~Decoder() {}

protected:
	Decoder() : mFallbackReplacementChar(0) {}

	uint32_t	mFallbackReplacementChar;

private:
	LN_DISALLOW_COPY_AND_ASSIGN(Decoder);
};

/**
	@brief		内部文字コード (UTF16) をエンコードするクラス
	@details	Decoder 同様、CanRemain() が true の場合は変換ステータスを保存します。
*/
class Encoder
{
public:

	/**
		@brief		1 文字の最小バイト数を取得します。
	*/
	virtual int GetMinByteCount() = 0;

	/**
		@brief		1 文字の最大バイト数を取得します。
	*/
	virtual int GetMaxByteCount() = 0;

	/**
		@brief	Encoder が変換状態を保持できるかを確認します。
	*/
	virtual bool CanRemain() = 0;

	/**
		@brief		エンコード先にマッピングできない文字が見つかった時に置換する文字の設定
	*/
	virtual void SetFallbackReplacementChar(uint32_t ch) { mFallbackReplacementChar = ch; }

	/**
		@brief		エンコード先にマッピングできない文字が見つかった時に置換する文字の取得
	*/
	virtual uint32_t GetFallbackReplacementChar() { return mFallbackReplacementChar; }

	/**
		@brief		UTF16 文字列を、このエンコーディングで表現される文字列に変換する
		@param[in]	input				: このエンコーディングで表現される文字列バッファ
		@param[in]	inputElementSize	: input のサイズ (要素数単位。NULL 文字は含まない)
		@param[out]	output				: 変換結果の格納先バッファ
		@param[in]	outputByteSize		: output のバイト数
		@param[out]	outBytesUsed		: 変換結果のバイト数を格納する変数
		@param[out]	outCharsUsed		: 変換結果の文字数を格納する変数
	*/
	virtual void ConvertFromUTF16(const UTF16* input, size_t inputElementSize, byte_t* output, size_t outputByteSize, size_t* outBytesUsed, size_t* outCharsUsed) = 0;

	/**
		@brief	一連の ConvertToUTF16() の呼び出しの中で、変換できない文字を規定文字に変換した文字数を取得する
	*/
	virtual int UsedDefaultCharCount() = 0;

	/**
		@brief	最後の ConvertToUTF16() で、バッファ末尾でマルチバイト文字が途切れていなければ true を返す
	*/
	virtual bool Completed() = 0;

	/**
		@brief	変換ステータスをクリアし、初期状態に戻す
	*/
	virtual void Reset() = 0;

	virtual ~Encoder() {}

protected:
	Encoder() : mFallbackReplacementChar(0) {}

protected:
	uint32_t	mFallbackReplacementChar;

private:
	LN_DISALLOW_COPY_AND_ASSIGN(Encoder);
};

LN_NAMESPACE_END
