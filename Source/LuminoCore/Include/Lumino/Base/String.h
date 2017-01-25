/**
	@file	String.h
*/

#pragma once
#include <vector>
#include <string>
#include <atomic>
#include "Common.h"
#include "List.h"
#include "ByteBuffer.h"

LN_NAMESPACE_BEGIN
class Locale;
class Encoding;
template<typename TChar> class GenericCharRef;
template<typename TChar> class GenericStringRef;
template<typename TChar> class GenericStringArray;
template<typename TChar> class GenericPathName;

namespace detail { template<typename TChar> class GenericStringCore; }
namespace detail { class StringHelper; }
namespace tr { class Variant; }


/**
	@brief		文字列を表すクラスです。

	@details	このクラスは任意の長さの文字配列を扱うユーティリティクラスです。<br>
				C文字列からの構築、連結と比較、抽出、検索と置換など、文字列に対する一般的な操作を備えています。
				操作中は必要に応じて文字列の長さを拡張します。<br>
				内部では、メモリ使用量を削減し、データの不必要なコピーを避けるためにコピーオンライト(COW)の共有を行います。


	@section	文字列型について

	GenericString は型引数として char または wchar_t を受け取ります。
	StringA と StringW はそれぞれ GenericString<char> と GenericString<wchar_t> のインスタンスです。	<br>
	String は、UNICODE または LN_UNICODE シンボルのどちらが定義されている場合は GenericString<wchar_t>、
	それ以外は GenericString<char> としてサポートされます。	

	String を使用する場合、併せて TCHAR 型および _T マクロを使用するべきです。
	これらは Windows プログラミングの国際対応で用いられるもので、本ライブラリでは Windows 以外の環境でも
	UNICODE または LN_UNICODE シンボルの定義に合わせて char または wchar_t 用に定義されます。


	@section	エンコーディング（文字コード）について

	GenericString とそのインスタンスである String、StringA、StringW は、いずれもエンコーディングを規定しません。
	規定されるのは型のみで、エンコーディングは環境に依存します。<br>
	- char 型
		- Windows		… ユーザーロケールに依存するマルチバイトエンコーディング (日本語 OSであれば Shift_JIS)
		- Linux, Mac	… UTF-8
	- wchar_t 型
		- Windows		… UTF-16
		- Linux, Mac	… UTF-32

	例えば VisualStudio (Windows) で LN_UNICODE シンボルを定義している場合、
	String は wchar_t 型であり、エンコーディングは UTF-16 として扱われます。

	現在の環境のエンコーディングは Encoding::GetTCharEncoding() により取得することができます。


	@section	比較について

	このクラスを通じて2つの文字列を比較する場合、NULL と 空文字列は同一視されます。<br>
	また、比較時にロケールは考慮しません。ロケールを考慮した比較を行う場合、Locale クラスの関数を使用してください。<br>

	なお、NULL を代入したり NULL で初期化した場合、インスタンスは空文字列として初期化されます。
	

*/
template<typename TChar>
class GenericString
{
public:
	typedef typename GenericStringTraits<TChar>::XCHAR XCHAR;
	typedef typename GenericStringTraits<TChar>::YCHAR YCHAR;

	typedef GenericString<TChar>	StringT;
	typedef GenericStringRef<TChar>	StringRefT;
	typedef TChar					CharType;
    typedef std::size_t size_type;	// for GCC

public:
	GenericString();
	~GenericString();

	// TChar (同一の文字型は assign だけにし、高速化を図る)
	GenericString(const GenericString& str);
	GenericString(const GenericString& str, int length);
	GenericString(const GenericString& str, int begin, int length);
	GenericString(const StringRefT& str);
	GenericString(const TChar* str);
	GenericString(const TChar* str, int length);
	GenericString(const TChar* str, int begin, int length);
	GenericString(TChar ch);
	GenericString(int count, TChar ch);
	GenericString(const GenericPathName<TChar>& path);
	GenericString(GenericString&& str) LN_NOEXCEPT;

	// other char code convertion
	explicit GenericString(const GenericString<YCHAR>& str);
	explicit GenericString(const GenericString<YCHAR>& str, int length);
	explicit GenericString(const GenericString<YCHAR>& str, int begin, int length);
	explicit GenericString(const YCHAR* str);
	explicit GenericString(const YCHAR* str, int length);
	

	/// @name Operators
	/// @{
	GenericString& operator=(const GenericString& right);
	GenericString& operator=(const GenericStringRef<TChar>& right);
	GenericString& operator=(const std::basic_string<TChar>& right);
	GenericString& operator=(const TChar* right);
	GenericString& operator=(const GenericPathName<TChar>& right);
	GenericString& operator=(GenericString&& right) LN_NOEXCEPT;

	GenericString& operator+=(const GenericString& right);
	GenericString& operator+=(const TChar* ptr);
	GenericString& operator+=(TChar ch);

	bool operator==(const GenericString& right) const			{ return Equals(right); }		///< @see Equals
	bool operator==(const GenericStringRef<TChar>& right) const	{ return Equals(right); }		///< @see Equals
	bool operator==(const TChar* right) const					{ return Equals(right); }		///< @see Equals
	bool operator!=(const GenericString& right) const			{ return !operator==(right); }	///< @see Equals
	bool operator!=(const GenericStringRef<TChar>& right) const	{ return !operator==(right); }	///< @see Equals
	bool operator!=(const TChar* right) const					{ return !operator==(right); }	///< @see Equals

	bool operator<(const GenericString& right) const;
	bool operator<(const GenericStringRef<TChar>& right) const;
	bool operator<(const TChar* right) const;
	bool operator>(const GenericString& right) const;
	bool operator>(const GenericStringRef<TChar>& right) const;
	bool operator>(const TChar* right) const;
	bool operator<=(const GenericString& right) const			{ return !operator>(right); }
	bool operator<=(const GenericStringRef<TChar>& right) const	{ return !operator>(right); }
	bool operator<=(const TChar* right) const					{ return !operator>(right); }
	bool operator>=(const GenericString& right) const			{ return !operator<(right); }
	bool operator>=(const GenericStringRef<TChar>& right) const	{ return !operator<(right); }
	bool operator>=(const TChar* right) const					{ return !operator<(right); }

	GenericCharRef<TChar> operator[](int index);
	const TChar& operator[](int index) const;

	//operator const TChar*() const;
	/// @}

public:

	/// C言語形式の文字列ポインタを返す
	const TChar* c_str() const;
	

	/// 終端 \0 までの文字数を返す (マルチバイト文字は考慮しない。CString::GetLength と同様の関数です)
	int GetLength() const;

	/// 文字列が空の時にtrueを返す
	bool IsEmpty() const;

	/**
		@brief		この文字列の末尾に指定された文字列を追加します。
		@param[in]	str		: 追加する文字列
		@param[in]	len		: 追加する文字数 (-1 を指定すると \0 まで)
	*/
	void Append(const GenericString& str, int len = -1);
	void Append(const TChar* str, int len = -1);			///< @overload Append
	void Append(TChar ch);									///< @overload Append

	/**
		@brief		指定したエンコーディングを使用し、文字列を変換して設定する
		@param[in]	buffer				: 変換元文字列が格納されているバッファ
		@param[in]	byteCount			: buffer のバイト数 (-1 で \0 まで)
		@param[in]	encoding			: 変換元文字エンコーディング
		@param[out]	usedDefaultChar		: 変換不可文字をデフォルト文字 ('?') に変換したかどうか
	*/
	void ConvertFrom(const void* buffer, int byteCount, const Encoding* encoding, bool* usedDefaultChar = NULL);

	/**
		@brief		指定したエンコーディングを使用し、変換した文字列バッファを取得する
		@param[in]	encoding			: 変換先文字エンコーディング
		@param[out]	usedDefaultChar		: 変換不可文字をデフォルト文字 ('?') に変換したかどうか
		@return		バッファ終端には encoding に合わせた \0 終端文字が付加されます。
	*/
	ByteBuffer ConvertTo(const Encoding* encoding, bool* usedDefaultChar = NULL) const;

	/**
		@brief		空文字列を設定する
	*/
	void Clear();

	/**
		@brief		部分文字列を取得する
		@param[in]	startIndex	: 文字列の 0 から始まる開始文字位置
		@param[in]	length		: 部分文字列の文字数 (省略した場合は末尾まで)
	*/
	StringT SubString(int startIndex = 0, int length = -1) const;

	/**
		@brief		文字列の先頭と末尾の空白を全て削除する
		@details	空白文字はロケールに依存します。日本語環境であれば全角スペースも空白扱いです。
	*/
	StringT Trim() const;

	/**
		@brief		この文字列から指定した文字をすべて取り除いた新しい文字列を返します。
		@param[in]	ch		: 削除する文字
		@param[in]	cs		: 大文字と小文字の区別設定
	*/
	StringT Remove(TChar ch, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;

	/**
		@brief		文字列の置換を行う
		@param[in]	from	: 置換される文字列
		@param[in]	to		: 出現するすべての from を置換する文字列
		@return		置換結果の文字列
		@details	from に一致するすべての文字列を to に置換します。
					~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
					String str = "abc";
					str = str.Replace("bc", "d");	// => "ad"
					~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	*/
	StringT Replace(const StringRefT& from, const StringRefT& to) const;

	/**
		@brief		指定した文字列がこの文字列内に存在するかを判断します。
		@param[in]	str		: 検索文字列
		@param[in]	cs		: 大文字と小文字の区別設定
		@return		文字列が存在すれば true。str が空文字列である場合は必ず true となります。
	*/
	bool Contains(const TChar* str, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;
	bool Contains(TChar ch,         CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;			/**< @overload Contains */

	/**
		@brief		文字列を検索し、見つかった最初の文字のインデックスを返す
		@param[in]	str			: 検索文字列
		@param[in]	startIndex	: 検索を開始するインデックス (省略した場合は先頭から)
		@param[in]	cs		: 大文字と小文字の区別設定
		@return		見つからなかった場合は -1。str が空文字列である場合は 0。
	*/
	int IndexOf(const StringRefT& str, int startIndex = 0, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;
	int IndexOf(TChar ch,              int startIndex = 0, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;	/**< @overload IndexOf */

	/**
		@brief		文字列を検索し、最後に見つかったインデックスを返します。
		@param[in]	str			: 検索文字列
		@param[in]	startIndex	: 検索を開始するインデックス (-1 を指定すると、文字列の末尾から検索を開始する)
		@param[in]	count		: 検索する文字数 (-1 を指定すると、文字列の先頭まで検索する)
		@param[in]	cs			: 大文字と小文字の区別設定
		@return		見つかった文字列の開始インデックス。見つからなかった場合は -1。
		@details	startIndex の位置から文字列の先頭に向かう count 文字分の領域から str を検索します。
		@code
					String str = "abcdef";
					str.LastIndexOf("de");			// => 3
					str.LastIndexOf("bc", 2);		// => 1
					str.LastIndexOf("cd", 2);		// => -1	(検索範囲 "abc" の中に "cd" は存在しない)
					str.LastIndexOf("cd", 4, 3);	// => 2		(検索範囲 "cde" の中に "cd" は存在する)
					str.LastIndexOf("bc", 4, 3);	// => -1	(検索範囲 "cde" の中に "bc" は存在しない)
		@endcode
	*/
	int LastIndexOf(const TChar* str, int startIndex = -1, int count = -1, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;
	int LastIndexOf(TChar ch,         int startIndex = -1, int count = -1, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;	/**< @overload LastIndexOf */
	
	/**
		@brief		この文字列の先頭が、指定した文字列と一致するかを判断します。
		@param[in]	str			: 検索文字列
		@details	str が空文字の場合は必ず true が返ります。
	*/
	bool StartsWith(const TChar* str, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;
	bool StartsWith(TChar ch,         CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;	/**< @overload StartsWith */

	/**
		@brief		この文字列の末尾が、指定した文字列と一致するかを判断します。
		@param[in]	str			: 検索文字列
		@details	str が空文字の場合は必ず true が返ります。
		@code
					str = "file.txt";
					if (str.EndsWith(".txt")) {
						// 一致した
					}
		@endcode
	*/
	bool EndsWith(const TChar* str, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;
	bool EndsWith(TChar ch,         CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;	/**< @overload EndsWith */
	
	/**
		@brief		文字列が同一かを判断します。
		@param[in]	str		: この文字列と比較する文字列
		@details	大文字/小文字を区別し、ロケールに依存しない比較を行います。
					また、str が NULL の場合は空文字とみなして比較を行います。
	*/
	bool Equals(const GenericString& str) const;
	bool Equals(const GenericStringRef<TChar>& str) const;	/**< @overload Equals */
	bool Equals(const TChar* str) const;					/**< @overload Equals */

	/**
		@brief		この文字列と、指定した文字列を比較します。
		@param[in]	str			: 比較文字列
		@param[in]	count		: 比較する文字数 (-1 の場合、GetLength() の値を使用します)
		@param[in]	cs			: 大文字と小文字の区別設定
		@return		str1 が str2 より小さい → 0 より小さい値
					str1 と str2 が等しい   → 0
					str1 が str2 より大きい → 0 より大きい値
	*/
	int Compare(const TChar* str, int count = -1, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;

	/**
		@brief		文字列の左側(先頭)から指定した文字数を抽出します。
		@param[in]	str		: 対象の文字列
		@param[in]	count	: 文字数
		@return		抽出された文字列
		@code
					String s(_T("abcdef"));
					s.Left(2)		=> _T("ab");
		@endcode
	*/
	GenericString<TChar> Left(int count) const;

	/**
		@brief		文字列の右側(末尾)から指定した文字数を抽出します。
		@param[in]	str		: 対象の文字列
		@param[in]	count	: 文字数
		@return		抽出された文字列
		@code
					String s(_T("abcdef"));
					s.Right(2)		=> _T("ef");
		@endcode
	*/
	GenericString<TChar> Right(int count) const;

	/**
		@brief		文字列の部分文字列を抽出します。
		@param[in]	str		: 対象の文字列
		@param[in]	start	: 開始文字インデックス
		@param[in]	count	: 文字数 (-1 の場合、末尾まで抽出する)
		@return		抽出された文字列
		@code
					String s(_T("abcdef"));
					s.Mid(2, 3)		=> _T("cde");
		@endcode
	*/
	GenericString<TChar> Mid(int start, int count = -1) const;

	/**
		@brief		文字列をデリミタで分割する
		@param[in]	delim	: デリミタ文字列
		@param[in]	option	: 分割方法
		@return		分割結果の文字列配列
		@detail		分割が発生しない場合は文字列全体を持つ要素数1の配列を返します。
					
					~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
					auto tokens = String("a,b,c").Split(",");		// => ["a", "b", "c"]
					auto tokens = String("a").Split(",");			// => ["a"]
					auto tokens = String(",").Split(",");			// => ["", ""]
					auto tokens = String("a::b").Split("::");		// => ["a", "b"]
					~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	*/
	GenericStringArray<TChar> Split(const TChar* delim, StringSplitOptions option = StringSplitOptions::None) const;

	/**
		@brief		文字列を構成するバイト数を取得する (終端 '\0' は含まない)
	*/
	int GetByteCount() const { return GetLength() * sizeof(TChar); }

	/** 小文字を大文字に変換します。(ロケールの影響を受けません) */
	GenericString<TChar> ToUpper() const;

	/** 大文字を小文字に変換します。(ロケールの影響を受けません) */
	GenericString<TChar> ToLower() const;

	/** 先頭の文字を大文字、以降を小文字に変換します。(ロケールの影響を受けません) */
	GenericString<TChar> ToTitleCase() const;

	/**
		@brief		この文字列を整数値に変換します。
		@param[in]	base		: 基数 (0、2、8、10、16 のいずれかであること)
		@return		変換結果の数値
		@details	次の書式に従い、文字列を数値に変換します。<br>
					[whitespace] [{+ | – }] [0 [{ x | X }]] [digits | letters]		<br>
					16 進数値のアルファベットは大文字と小文字を区別しません。		<br><br>
					
					基数に 0 を指定すると、文字列の先頭文字から基数を自動判別します。<br>
					"0x" または "0X" であれば 16 進数、"0" であれば 8 進数、それ以外であれば 10 進数です。
					基数に 8 または 16 が指定されている際、文字列の先頭は "0" または "0x" である必要はありません。

		@exception	InvalidFormatException	指定された基数に対して有効な桁を示す数字以外の文字が含まれていました。または、書式の前後に空白以外の文字が存在しました。
		@exception	OverflowException		数値に変換する際にオーバーフローが発生しました。
	*/
	int8_t		ToInt8(int base = 0) const;
	int16_t		ToInt16(int base = 0) const;	///< @copydoc ToInt8
	int32_t		ToInt32(int base = 0) const;	///< @copydoc ToInt8
	int64_t		ToInt64(int base = 0) const;	///< @copydoc ToInt8
	uint8_t		ToUInt8(int base = 0) const;	///< @copydoc ToInt8
	uint16_t	ToUInt16(int base = 0) const;	///< @copydoc ToInt8
	uint32_t	ToUInt32(int base = 0) const;	///< @copydoc ToInt8
	uint64_t	ToUInt64(int base = 0) const;	///< @copydoc ToInt8

	/**
		@brief		この文字列を整数値に変換し、成否を返します。
		@param[in]	outValue	: 結果を格納する変数のポインタ (NULL を指定すると成否のみを返す)
		@param[in]	base		: 基数 (0、2、8、10、16 のいずれかであること)
		@return		正常に変換された場合は true。それ以外の場合は false。
		@details	例外が発生しない点を除けば ToInt8 等と同様です。
					大量のループの内部等、例外によるパフォーマンスへの影響が懸念される場合に使用してください。
		@see		ToInt8
	*/
	bool		TryToInt8(int8_t* outValue, int base = 0) const;
	bool		TryToInt16(int16_t* outValue, int base = 0) const;		///< @copydoc TryToInt8
	bool		TryToInt32(int32_t* outValue, int base = 0) const;		///< @copydoc TryToInt8
	bool		TryToInt64(int64_t* outValue, int base = 0) const;		///< @copydoc TryToInt8
	bool		TryToUInt8(uint8_t* outValue, int base = 0) const;		///< @copydoc TryToInt8
	bool		TryToUInt16(uint16_t* outValue, int base = 0) const;	///< @copydoc TryToInt8
	bool		TryToUInt32(uint32_t* outValue, int base = 0) const;	///< @copydoc TryToInt8
	bool		TryToUInt64(uint64_t* outValue, int base = 0) const;	///< @copydoc TryToInt8

	GenericString<char>	ToStringA() const;
	GenericString<wchar_t> ToStringW() const;

	/**
		@brief		指定した char 配列から文字列を作成します。
		@param[in]	str		: コピーする文字列
		@param[in]	length	: コピーする文字数 (省略した場合は終端 \0 まで)
		@return		作成された文字列
	*/
	static GenericString FromNativeCharString(const char* str, int length = -1);
	
	/**
		@brief		指定した wchar_t 配列から文字列を作成します。
		@param[in]	str		: コピーする文字列
		@param[in]	length	: コピーする文字数 (省略した場合は終端 \0 まで)
		@return		作成された文字列
	*/
	static GenericString FromNativeCharString(const wchar_t* str, int length = -1);


	/**
		@brief		ネイティブ型文字列を割り当てます。
		@param[in]	str				: 設定する文字列
		@param[in]	begin			: コピー範囲の開始インデックス (省略した場合は先頭から)
		@param[in]	length			: コピーする文字数 (省略した場合は終端 \0 まで)
		@param[in]	usedDefaultChar	: 変換不可文字をデフォルト文字 ('?') に変換したかどうか
		@details	この関数は char 用、wchar_t 用それぞれオーバーロードされます。
					型引数 TChar に対して必要であれば、
					それぞれ Encoding::GetSystemMultiByteEncoding()、Encoding::GetWideCharEncoding() で取得できるエンコーディングを使用して
					文字コードをの変換を行います。
					TChar と str の型が同じ場合は文字コードの変換を行いません。
	*/
	void AssignCStr(const char* str, int begin, int length, bool* usedDefaultChar = NULL);
	void AssignCStr(const char* str, int length, bool* usedDefaultChar = NULL);					/**< @overload AssignCStr */
	void AssignCStr(const char* str, bool* usedDefaultChar = NULL);								/**< @overload AssignCStr */
	void AssignCStr(const wchar_t* str, int begin, int length, bool* usedDefaultChar = NULL);	/**< @overload AssignCStr */
	void AssignCStr(const wchar_t* str, int length, bool* usedDefaultChar = NULL);				/**< @overload AssignCStr */
	void AssignCStr(const wchar_t* str, bool* usedDefaultChar = NULL);							/**< @overload AssignCStr */

public:
	/// 現在の環境で定義されている改行文字列を取得する
	static const StringT& GetNewLine();

	/// 空文字列を取得する
	static const StringT& GetEmpty();

	/**
		@brief		書式文字列と可変長引数リストから文字列を生成します。
		@param[in]	format		: 書式文字列 (printf の書式指定構文)
		@param[in]	...			: 引数リスト

		@details	指定できる書式はC言語標準の printf 等の書式です。
					~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
					String str1 = String::SPrintf(_T("%d"), 100);			// => "100"
					~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

					各書式指定子の詳細な仕様は環境に依存します。
					特にこれらの中には32ビットシステムと64ビットシステムとの間で移植性がはっきりしないものがありますので、
					そういったシステムで利用する場合はオーバーラン等に注意してください。

					こういった移植性や後述する引数リストにクラスの実体を指定できてしまう問題を回避するため、
					Format() を使用することを推奨します。

		@attention	引数リストに指定できるのは環境がサポートしているプリミティブ型だけです。
					String クラス等のクラスや構造体を指定した場合は未定義動作となります。
					~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
					String str1 = String::SPrintf(_T("%d"), 100);			// => "100"
					String str2 = String::SPrintf(_T("%ss"), str1);			// => NG (未定義動作)
					String str2 = String::SPrintf(_T("%ss"), str1.c_str());	// => "100s"
					~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	*/
	static GenericString SPrintf(const GenericString& format, ...);
	static GenericString SPrintf(const TChar* format, ...);				/**< @overload SPrintf */
	
	/**
		@brief		複合書式文字列と可変長引数リストから文字列を生成します。
		@param[in]	locale		: ロケール
		@param[in]	format		: 書式文字列
		@param[in]	...			: 引数リスト

		@details	この関数は書式として、.NET Framework で使用されている複合書式文字列を受け取ります。
					~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
					String name = "file";
					int index = 5;
					fileName = String::Format("{0}_{1}.txt", name, index);
					~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

					各書式指定項目の構文は次の通りです。
					~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
					{index[,alignment][:formatString][precision]}
					~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
					- index			: 引数リストに指定された値の番号。
					- alignment		: フィールドの幅。書式設定された文字列よりも長い場合空白で埋められ、値が正の場合は右揃え、負の場合は左揃えになります。
					- formatString	: 書式指定文字列。以下のセクションを参照してください。
					- precision		: 精度指定子。formatString によって意味が変わります。

		@section	10 進数 ("D") 書式指定子

					数値を 10 進数文字列に変換します。入力は整数型のみサポートします。

					精度指定子は変換後の文字列の最小桁数です。
					出力がこの桁数未満の場合は0埋めを行います。
					~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
					String::Format("{0:D}", 12345));			// => "12345"
					String::Format("{0:d}", -12345));			// => "-12345"
					String::Format("{0:D8}", 12345));			// => "00012345"
					~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		@section	16 進数 ("X") 書式指定子

					数値を 16 進数文字列に変換します。入力は整数型のみサポートします。
					書式指定子が大文字か小文字かによって出力される文字列の大文字か小文字が決まります。

					精度指定子は変換後の文字列の最小桁数です。
					出力がこの桁数未満の場合は0埋めを行います。
					~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
					String::Format("{0:x}", 0x2045e);			// => "2045e"
					String::Format("{0:X}", 0x2045e);			// => "2045E"
					String::Format("{0:X8}", 0x2045e);			// => "0002045E"
					String::Format("0x{0:X}", 255);				// => "0xFF"
					~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		
		@section	固定小数点 ("F") 書式指定子

					実数を固定小数点の文字列に変換します。

					精度指定子は小数部の桁数です。
					~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
					String::Format("{0:F}", 25.1879));					// => "25.1879"
					String::Format("{0:f}", 25.1879));					// => "25.1879"
					String::Format("{0:F2}", 25.1879));					// => "25.19"
					String::Format(Locale("fr"), "{0:F2}", 25.1879));	// => "25,187900"
					~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		@section	指数 ("E") 書式指定子

					実数を指数表現の文字列に変換します。
					書式指定子が大文字か小文字かによって出力される文字列の大文字か小文字が決まります。

					精度指定子は小数部の桁数です。
					~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
					String::Format("{0:e}", 12345.6789);				// => "1.234568e+004"
					String::Format("{0:E10}", 12345.6789);				// => "1.2345678900E+004"
					String::Format("{0:e4}", 12345.6789);				// => "1.2346e+004"
					String::Format(Locale("fr"), "{0:E}", 12345.6789);	// => "1,234568E+004"
					~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		@section	{} のエスケープ

					左中かっこ ({) および右中かっこ (}) は、書式指定項目の開始および終了として解釈されます。
					したがって、左中かっこおよび右中かっこを文字として表示するためには、エスケープ シーケンスを使用する必要があります。
					左中かっこを 1 つ ("{") 表示するには、左中かっこ 2 つ ("{{") を固定テキストに指定します。
					~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
					String::Format("{{0}}");					// => "{0}"
					String::Format("{{{0}}}", 1);				// => "{1}"
					~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

		@section	型安全性

					引数リストに指定された値の型が受け入れられない場合、static_assert によってコンパイル時に不正を検出します。
					また、型のサイズは引数リストに指定された値の型から求めるため printf 書式の移植性の問題もありません。

					引数リストに入力できる値の型は以下の通りです。
					- 文字型 (char, wchar_t)
					- 8～64bit の整数型
					- 実数型 (float, double)
					- bool 型
					- 文字配列 (char*, char[], wchar_t*, wchar_t[])
					- std::string クラス (std::basic_string<T>)
					- String クラス (GenericString<T>)
	*/
	template<typename... TArgs>
	static GenericString Format(const GenericStringRef<TChar>& format, const TArgs&... args);
	template<typename... TArgs>
	static GenericString Format(const Locale& locale, const GenericStringRef<TChar>& format, const TArgs&... args);	/**< @overload Format */

	/**
		@brief		String に格納されているデータへのポインタを返します。
		@details	返されたポインタは、文字列を構成する文字にアクセスして変更するために使用することができます。
					格納されているデータが複数の String から参照されている場合はデータをコピーし、新しいデータのポインタを返します。
	*/
	TChar* GetData();

private:
	friend class detail::StringHelper;
	friend class tr::Variant;
	template<typename T> friend class GenericCharRef;
	template<typename T> friend class GenericStringRef;
	
	void Detach() LN_NOEXCEPT;
	void Attach(detail::GenericStringCore<TChar>* core);
	void AssignTString(const TChar* str, int len);
	void AssignTString(int count, TChar ch);
	void Realloc();
	TChar& InternalGetAt(int index);
	const TChar& InternalGetAt(int index) const;

	Encoding* GetThisTypeEncoding() const;
	detail::GenericStringCore<TChar>* GetCore() const { return m_string; }

	detail::GenericStringCore<TChar>*	m_string;

#ifdef LN_GenericString_Extensions
	LN_GenericString_Extensions;
#endif
};



template<typename TChar>
class GenericCharRef
{
private:
	template<typename T> friend class GenericString;

	GenericString<TChar>& m_str;
	int m_idx;

	inline GenericCharRef(GenericString<TChar>& str, int idx)
		: m_str(str), m_idx(idx)
	{}

public:

	inline operator TChar() const
	{
		LN_THROW(m_idx < static_cast<int>(m_str.m_string->size()), InvalidOperationException);
		return m_str.m_string->at(m_idx);
	}

	inline GenericCharRef& operator=(TChar ch)
	{
		m_str.Realloc();
		m_str.m_string->at(m_idx) = ch;
		return *this;
	}
	inline GenericCharRef& operator=(const GenericCharRef& ch) { return operator=(static_cast<TChar>(ch)); }
	inline GenericCharRef& operator=(unsigned short ch) { return operator=(static_cast<TChar>(ch)); }
	inline GenericCharRef& operator=(short ch) { return operator=(static_cast<TChar>(ch)); }
	inline GenericCharRef& operator=(unsigned int ch) { return operator=(static_cast<TChar>(ch)); }
	inline GenericCharRef& operator=(int ch) { return operator=(static_cast<TChar>(ch)); }
};


template<typename TChar>
inline GenericString<TChar> operator+(const GenericString<TChar>& left, const GenericString<TChar>& right)
{
	GenericString<TChar> str;
	//str.reserve(left.size() + right.size());	// TODO
	str += left;
	str += right;
	return str;
}
template<typename TChar>
inline GenericString<TChar> operator+(const GenericString<TChar>& left, const TChar* right)
{
	GenericString<TChar> str;
	str += left;
	str += right;
	return str;
}
template<typename TChar>
inline GenericString<TChar> operator+(const TChar* left, const GenericString<TChar>& right)
{
	GenericString<TChar> str;
	str += left;
	str += right;
	return str;
}
template<typename TChar>
inline GenericString<TChar> operator+(const GenericString<TChar>& left, TChar right)
{
	GenericString<TChar> str;
	str += left;
	str += right;
	return str;
}
template<typename TChar>
inline GenericString<TChar> operator+(TChar left, const GenericString<TChar>& right)
{
	GenericString<TChar> str;
	str += left;
	str += right;
	return str;
}
template<typename TChar>
inline bool operator==(const TChar* left, const GenericString<TChar>& right)
{
	return right.Equals(left);
}

template<typename TChar>
GenericCharRef<TChar> GenericString<TChar>::operator[](int index)
{
	return GenericCharRef<TChar>(*this, index);
}


#ifdef LN_DOXYGEN
/// @see GenericString
class String {};
/// @see GenericString
class StringA {};	
/// @see GenericString
class StringW {};		
#else
typedef GenericString<TCHAR>	String;
typedef GenericString<char>		StringA;
typedef GenericString<wchar_t>	StringW;
#endif

namespace detail {

#if defined(LN_DEBUG)
extern int g_testGenericStringCoreAllocCount;
#endif

template<typename TChar>
class GenericStringCore
	: public std::basic_string<TChar, std::char_traits<TChar>, detail::StlAllocator<TChar> >
{
public:
	GenericStringCore()
		: m_refCount(1)
	{
#if defined(LN_DEBUG)
		++g_testGenericStringCoreAllocCount;
#endif
	}
	~GenericStringCore() {}

	static GenericStringCore* GetSharedEmpty() { return &m_sharedEmpty; }

#ifdef LN_INTERNAL_COW_THREAD_SAFE
	// ※ m_sharedEmpty の参照カウントは操作しない。String を初期化しただけでオーバーヘッドが出るのを避けるため。
	inline bool IsShared() const { return IsSharedEmpty() || (m_refCount.load() > 1); }
	inline bool IsSharedEmpty() const { return this == GetSharedEmpty(); }
	inline void AddRef()
	{
		if (IsSharedEmpty()) {
			return;
		}
		m_refCount.fetch_add(1, std::memory_order_relaxed);/*m_refCount.Increment();*/
	}
	inline void Release()
	{
		if (IsSharedEmpty()) {
			return;
		}

		int before = m_refCount.fetch_sub(1, std::memory_order_relaxed);
		/*m_refCount.Decrement();*/
		//if (m_refCount.Get() <= 0)
		if (before <= 1)
		{
			//if (this != GetSharedEmpty()) {		// グローバル変数として定義された String からの解放済み delete 対策
				delete this;
			//}
		}
	}
public:
	std::atomic<int>	m_refCount;
#else
	inline bool IsShared() const { return (m_refCount > 1); }
	inline void AddRef() { ++m_refCount; }
	inline void Release()
	{
		--m_refCount;
		if (m_refCount <= 0)
		{
			if (this != GetSharedEmpty()) {		// グローバル変数として定義された String からの解放済み delete 対策
				delete this;
			}
		}
	}
public:
	int		m_refCount;
#endif

	static GenericStringCore	m_sharedEmpty;
};

class StringHelper
{
public:
	template<typename TChar>
	static GenericStringCore<TChar>* GetStringCore(const GenericString<TChar>& str) { return str.m_string; }

	template<typename TChar>
	static void AttachStringCore(String* str, GenericStringCore<TChar>* core) { str->Attach(core); }
};

} // namespace detail

LN_NAMESPACE_END


namespace std {

// for unordered_map key
template <> struct hash<ln::StringA> { std::size_t operator () (const ln::StringA& key) const; };
template <> struct hash<ln::StringW> { std::size_t operator () (const ln::StringW& key) const; };

} // namespace std

#include "StringHelper.h"
#include "StringRef.h"
