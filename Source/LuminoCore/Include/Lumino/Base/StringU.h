
#pragma once
#include <atomic>
#include "Common.h"
#include "StringBuilder.h"

LN_NAMESPACE_BEGIN
namespace detail { class UStringCore; }
using UChar = char16_t;

namespace detail
{

template<int N>
bool getLSB(unsigned char byte)
{
	return byte & (1u << N);
}

template<int N>
void setLSB(unsigned char& byte, bool bit)
{
	if (bit) {
		byte |= 1u << N;
	}
	else {
		byte &= ~(1u << N);
	}
}

enum class UStringRefSource
{
	ByUChar,
	ByChar,
};

}

class UStringRef;

/**
	@brief		文字列を表すクラスです。

	@note		[Experimental] null と空文字は区別しない。空文字扱い。null を表現したければ optional と組み合わせる。
				
*/
class UString
{
public:
	UString();
	~UString();
	UString(const UString& str);
	UString(UString&& str) LN_NOEXCEPT;
	UString& operator=(const UString& str);
	UString& operator=(UString&& str) LN_NOEXCEPT;

	UString(const UString& str, int begin);
	UString(const UString& str, int begin, int length);
	UString(const UChar* str);
	UString(const UChar* str, int length);
	UString(int count, UChar ch);
	UString(const char* str);
	UString(const UStringRef& str);

	/** 文字列が空であるかを確認します。 */
	bool isEmpty() const;

	/** C 言語としての文字列表現を取得します。 */
	const UChar* c_str() const;

	/** 文字列の長さを取得します。 */
	int getLength() const;

	/** メモリを再確保せずに格納できる最大の要素数を取得する。 */
	int getCapacity() const;

	/** 文字列をクリアします。 */
	void clear();

	/** 文字列の長さを変更します。 */
	void resize(int newLength);
	void resize(int newLength, UChar ch);

	/** サイズ変更の予定を指示します。 */
	void reserve(int size);
	
	/**
		@brief		指定した文字列がこの文字列内に存在するかを判断します。
		@param[in]	str		: 検索文字列
		@param[in]	cs		: 大文字と小文字の区別設定
		@return		文字列が存在すれば true。str が空文字列である場合は必ず true となります。
	*/
	bool contains(const UStringRef& str, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;
	bool contains(UChar ch, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;					/**< @overload contains */

	/**
		@brief		文字列を検索し、見つかった最初の文字のインデックスを返します。
		@param[in]	str			: 検索文字列
		@param[in]	startIndex	: 検索を開始するインデックス (省略した場合は先頭から)
		@param[in]	cs			: 大文字と小文字の区別設定
		@return		見つからなかった場合は -1。str が空文字列である場合は 0。
	*/
	int indexOf(const UStringRef& str, int startIndex = 0, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;
	int indexOf(UChar ch, int startIndex = 0, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;

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
	int lastIndexOf(const UStringRef& str, int startIndex = -1, int count = -1, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;
	int lastIndexOf(UChar ch, int startIndex = -1, int count = -1, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;	/**< @overload lastIndexOf */

	/**
		@brief		この文字列の先頭が、指定した文字列と一致するかを判断します。
		@param[in]	str			: 検索文字列
		@details	str が空文字の場合は必ず true が返ります。
	*/
	bool startsWith(const UStringRef& str, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;
	bool startsWith(UChar ch, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;				/**< @overload startsWith */

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
	bool endsWith(const UStringRef& str, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;
	bool endsWith(UChar ch, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;					/**< @overload endsWith */

	/**
		@brief		文字列の部分文字列を抽出します。
		@param[in]	str		: 対象の文字列
		@param[in]	start	: 開始文字インデックス
		@param[in]	count	: 文字数 (-1 の場合、末尾まで抽出する)
		@return		抽出された文字列
		@code
					String s("abcdef");
					s.Mid(2, 3)		=> "cde";
		@endcode
	*/
	UStringRef substring(int start, int count = -1) const;

	/** 文字列の先頭と末尾の空白を全て削除します。 */
	UString trim() const;

	/** 小文字を大文字に変換します。(ロケールの影響を受けません) */
	UString toUpper() const;

	/** 大文字を小文字に変換します。(ロケールの影響を受けません) */
	UString toLower() const;

	/** 先頭の文字を大文字、以降を小文字に変換します。(ロケールの影響を受けません) */
	UString toTitleCase() const;

	/**
		@brief		この文字列から指定した文字をすべて取り除いた新しい文字列を返します。
		@param[in]	str		: 削除する文字列
		@param[in]	cs		: 大文字と小文字の区別設定
	*/
	UString remove(const UStringRef& str, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;
	UString remove(UChar ch, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;
	
	/**
		@brief		文字列の置換を行います。
		@param[in]	from	: 置換される文字列
		@param[in]	to		: from を置換する文字列
		@return		置換結果の文字列
		@details	from に一致するすべての文字列を to に置換します。
	*/
	UString replace(const UStringRef& from, const UStringRef& to, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;
	
	/**
		@brief		文字列をデリミタで分割します。
		@param[in]	delim	: デリミタ文字列
		@param[in]	option	: 分割方法
		@return		分割結果の文字列配列
		@detail		分割が発生しない場合は文字列全体を持つ要素数1の配列を返します。
					
					~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
					auto tokens = String("a,b,c").split(",");		// => ["a", "b", "c"]
					auto tokens = String("a").split(",");			// => ["a"]
					auto tokens = String(",").split(",");			// => ["", ""]
					auto tokens = String("a::b").split("::");		// => ["a", "b"]
					~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	*/
	List<UString> split(const UStringRef& delim, StringSplitOptions option = StringSplitOptions::None) const;

	std::string toStdString() const;
	std::wstring toStdWString() const;


	/** 指定した文字列を連結します。 */
	static UString concat(const UStringRef& str1, const UStringRef& str2);

	template<typename... TArgs>
	static UString format(const UStringRef& format, TArgs&&... args);
	template<typename... TArgs>
	static UString format(const Locale& locale, const UStringRef& format, TArgs&&... args);	/**< @overload format */

	
	/**
		@brief		この文字列と、指定した文字列を比較します。
		@param[in]	str1		: 比較文字列
		@param[in]	index1		: str1 内の部分文字列の開始位置
		@param[in]	str2		: 比較文字列
		@param[in]	index2		: str2 内の部分文字列の開始位置
		@param[in]	length		: 比較する文字数 (-1 の場合、GetLength() の値を使用します)
		@param[in]	cs			: 大文字と小文字の区別設定
		@return		str1 が str2 より小さい → 0 より小さい値
					str1 と str2 が等しい   → 0
					str1 が str2 より大きい → 0 より大きい値
	*/
	static int compare(const UString& str1, const UString& str2, CaseSensitivity cs = CaseSensitivity::CaseSensitive);
	static int compare(const UStringRef& str1, int index1, const UStringRef& str2, int index2, int length = -1, CaseSensitivity cs = CaseSensitivity::CaseSensitive);

	static UString fromCString(const char* str, int length = -1);
	static UString fromCString(const wchar_t* str, int length = -1);

	/** @name STL interface */
	/** @{ */
	//typedef UChar* iterator;
	//typedef const UChar* const_iterator;

	//iterator begin();
	//const_iterator begin() const;
	//iterator end();
	//const_iterator end() const;

	/** @{ */



	/// 現在の環境で定義されている改行文字列を取得する
	static const UString& getNewLine();

	/// 空文字列を取得する
	static const UString& getEmpty();

	int getByteCount() const { return getLength() * sizeof(UChar); }

	bool isSSO() const LN_NOEXCEPT { return !detail::getLSB<0>(static_cast<uint8_t>(m_data.sso.length)); }
	bool isNonSSO() const LN_NOEXCEPT { return detail::getLSB<0>(static_cast<uint8_t>(m_data.sso.length)); }

	UString& operator=(const UStringRef& rhs);
	UString& operator=(const UChar* rhs);
	UString& operator=(UChar ch);

	UString& operator+=(const UString& rhs);
	UString& operator+=(const UStringRef& rhs);
	UString& operator+=(const UChar* rhs);
	UString& operator+=(UChar rhs);

private:
	static std::size_t const SSOCapacity = 15;//31;//sizeof(uint32_t) * 4 / sizeof(UChar) - 1;

	// resource management
	void init() LN_NOEXCEPT;
	void release() LN_NOEXCEPT;
	void copy(const UString& str);
	void move(UString&& str) LN_NOEXCEPT;
	void reserveBuffer(int length);
	UChar* lockBuffer(int requestSize);
	void unlockBuffer(int confirmedSize);
	UChar* getBuffer();

	// sso operation
	void setSSOLength(int len);
	int getSSOLength() const;
	void setSSO();
	void setNonSSO();

	// utils
	void assign(const UChar* str);
	void assign(const UChar* str, int length);
	void assign(int count, UChar ch);
	void assign(const UStringRef& str);
	template<typename TChar> void assignFromCStr(const TChar* str, int length = -1);
	void append(const UChar* str, int length);

	static ByteBuffer convertTo(const UString& str, const Encoding* encoding, bool* outUsedDefaultChar = nullptr);

	union Data
	{
		detail::UStringCore*	core;

		struct SSO
		{
			UChar		buffer[SSOCapacity];
			UChar		length;	// ---xxxxy	: x=size y:flag(0=sso,1=non sso)
		} sso;
	} m_data;
};

/**
	@brief		
*/
class UStringHelper
{
public:
	static size_t strlen(const UChar* str);
	static int compare(const UChar* str1, const UChar* str2);

	template<typename TChar>
	static void toStringInt8(int8_t v, TChar* outStr, int size);
};

/**
	@brief		
*/
class UStringRef
{
public:
	UStringRef()
	{
		m_type = detail::UStringRefSource::ByUChar;
		m_u.str = nullptr;
		m_u.length = 0;
	}

	UStringRef(const UString& str)
	{
		m_type = detail::UStringRefSource::ByUChar;
		m_u.str = str.c_str();
		m_u.length = str.getLength();
	}

	//template<std::size_t N>
	//UStringRef(const UChar (&str)[N])
	//{
	//	m_type = detail::UStringType::UChar;
	//	m_u.str = str;
	//	m_u.length = N;
	//	if (m_u.str[N - 1] == '\0')
	//	{
	//		m_u.length--;
	//	}
	//}

	UStringRef(const UChar* str)
	{
		m_type = detail::UStringRefSource::ByUChar;
		m_u.str = str;
		m_u.length = UStringHelper::strlen(str);
	}

	UStringRef(const UChar* str, int length)
	{
		m_type = detail::UStringRefSource::ByUChar;
		m_u.str = str;
		m_u.length = length;
	}

	UStringRef(const UChar* begin, const UChar* end)
	{
		m_type = detail::UStringRefSource::ByUChar;
		m_u.str = begin;
		m_u.length = end - begin;
	}

	//UStringRef(const char* str)
	//{
	//	m_type = detail::UStringRefSource::ByChar;
	//	m_c.str = UString(str);
	//}

	UStringRef(const UStringRef& str)
		: UStringRef()
	{
		copy(str);
	}

	UStringRef& operator=(const UStringRef& str)
	{
		copy(str);
	}

	~UStringRef()
	{
		clear();
	}

	int getLength() const
	{
		return (m_type == detail::UStringRefSource::ByUChar) ? m_u.length : m_c.str.getLength();
	}

	const UChar* data() const
	{
		return (m_type == detail::UStringRefSource::ByUChar) ? m_u.str : m_c.str.c_str();
	}

	const UChar* end() const
	{
		return data() + getLength();
	}

private:
	void clear()
	{
		switch (m_type)
		{
		case detail::UStringRefSource::ByUChar:
			break;
		case detail::UStringRefSource::ByChar:
			m_c.str.~UString();
			break;
		default:
			break;
		}
		m_type = detail::UStringRefSource::ByUChar;
		m_u.str = nullptr;
		m_u.length = 0;
	}

	void copy(const UStringRef& str)
	{
		clear();
		m_type = str.m_type;
		switch (m_type)
		{
		case detail::UStringRefSource::ByUChar:
			m_u.str = str.m_u.str;
			m_u.length = str.m_u.length;
			break;
		case detail::UStringRefSource::ByChar:
			m_c.str = str.m_c.str;
			break;
		default:
			break;
		}
	}

	detail::UStringRefSource	m_type;
	union
	{
		struct UCharData
		{
			const UChar*		str;
			int					length;
		} m_u;

		struct CharData
		{
			UString	str;
		} m_c;
	};
};




namespace fmt {

template<typename TChar>
class GenericFormatStringRef
{
public:
	GenericFormatStringRef();
	GenericFormatStringRef(const TChar* begin, const TChar* end);
	GenericFormatStringRef(const GenericFormatStringRef& str);

	bool isEmpty() const { return m_length == 0; }
	int getLength() const { return m_length; }
	const TChar* begin() const { return m_str; }
	const TChar* end() const { return m_str + m_length; }

	const TChar& operator[](int index) const { return *(m_str + index); }

private:
	const TChar*	m_str;
	int				m_length;
};

template<typename TChar>
class GenericFormatStringBuilder
{
public:
	GenericFormatStringBuilder();

	void clear();
	void appendChar(TChar ch);
	void appendChar(TChar ch, int count);
	void appendString(const TChar* str);
	void appendString(const TChar* str, int length);
	void appendString(const UString& str);
	const TChar* c_str() const;
	int getLength() const;

private:
	void appendIntenal(const TChar* str, int length);

	ByteBuffer	m_buffer;
	size_t		m_bufferUsed;
};

template<typename TChar>
class GenericStringFormatter
{
public:
	GenericStringFormatter()
		: m_error()
		, m_errorPos(0)
	{}
	~GenericStringFormatter() {}

	void reportError(const char* message, int pos) { m_error = message; m_errorPos = pos; }
	bool hasError() const { return !m_error.empty(); }

public:	// TODO
	const std::locale* m_locale;
	GenericFormatStringBuilder<TChar>	m_sb;
	GenericFormatStringRef<TChar>	m_formatString;
	GenericFormatStringRef<TChar>	m_precision;

private:
	std::string	m_error;
	int			m_errorPos;
};

template<typename Formatter>
void formatArg(Formatter&, ...)
{
	static_assert(false, "[Lumino format string error] Cannot format argument. Please overload formatArg.");
}

} // namespace fmt



namespace detail {

class UStringCore
{
public:

	UStringCore()
		: m_refCount(1)
		, m_str(nullptr)
		, m_capacity(0)
		, m_length(0)
	{}

	~UStringCore()
	{
		delete m_str;
	}

	bool isShared() const LN_NOEXCEPT { return (m_refCount > 1); }
	void addRef() { ++m_refCount; }
	void release()
	{
		--m_refCount;
		if (m_refCount == 0)
		{
			delete this;
		}
	}

	UChar* get() LN_NOEXCEPT { return m_str; }
	const UChar* get() const LN_NOEXCEPT { return m_str; }
	int getLength() const LN_NOEXCEPT { return m_length; }
	int getCapacity() const { return m_capacity; }
	void reserve(int length)
	{
		assert(length >= 0);
		int size = length + 1;
		if (m_capacity < size)
		{
			UChar* oldStr = m_str;
			int oldLen = m_length;

			m_str = LN_NEW UChar[size];
			m_capacity = size;

			if (oldStr != nullptr)
			{
				memcpy(m_str, oldStr, std::min(length, oldLen) * sizeof(UChar));
				delete oldStr;
			}
		}
	}
	void fixLength(int length)
	{
		m_str[length] = '\0';
		m_length = length;
	}
	void resize(int length)
	{
		reserve(length);
		fixLength(length);
	}
	void clear()
	{
		if (m_str != nullptr)
		{
			m_str[0] = '\0';
		}
		m_length = 0;
	}

private:
	std::atomic<int>	m_refCount;
	UChar*				m_str;
	int					m_capacity;
	int					m_length;
};

} // namespace detail




/**
	@brief		パス文字列を表すクラス
	@details	単純にパスセパレータで区切られた文字列を扱います。
				パスがフォルダを指すかファイルを指すかは区別しません。
				また、ファイルパスとして使用できない文字が含まれているかは確認しません。
				パス文字数の上限も確認しません。<br>

				セパレータは環境依存です。Windows では '\\'、UNIX 系では '/' です。
				※UNIX 系では '\\' をファイル名として使用可能です。

				このクラスに含まれる operator < 等の比較系の機能は、動作中のファイルシステムによって
				大文字小文字の区別を決定します。
*/
class Path
{
public:
	static const UChar Separator;
	static const UChar AltSeparator;
	static const UChar VolumeSeparator;

public:
	Path() {}
	Path(const Path& obj);

	Path(const UChar* path) { assign(path); }
	Path(const UString& path) { assign(path); }
	Path(const UStringRef& path) { assign(path); }
	Path(const Path& basePath, const UChar* relativePath) { assignUnderBasePath(basePath, UStringRef(relativePath)); }
	Path(const Path& basePath, const UStringRef& relativePath) { assignUnderBasePath(basePath, relativePath); }
	Path(const Path& basePath, const Path& relativePath) { assignUnderBasePath(basePath, relativePath); }

	// operators
	Path& operator = (const UString& str) { assign(str.c_str()); return (*this); }
	Path& operator = (const UChar* str) { assign(str); return (*this); }
	//bool operator < (const Path& right) const;
	//bool operator < (const UChar* right) const;


public:

	/**
		@brief		パス文字列を割り当てる
		@param[in]	path		: パス文字列
	*/
	void assign(const UStringRef& path);

	/**
		@brief		ベースパスと相対パスを連結して、パスを作成する
		@param[in]	basePath		: パス文字列
		@param[in]	relativePath	: パス文字列
		@details	relativePath がフルパスの場合は basePath を無視します。
	*/
	// TODO: 絶対パスにしてほしくない
	void assignUnderBasePath(const Path& basePath, const UStringRef& relativePath);
	void assignUnderBasePath(const Path& basePath, const Path& relativePath);

	/**
		@brief		現在のパスに別のパス文字列を連結します。
		@param[in]	path		: パス文字列
		@details	現在のパスの末尾にセパレータが無い場合は付加し、文字列を連結します。
					path が絶対パスであれば、現在のパスを置き換えます。
	*/
	void append(const UStringRef& path);
	void append(const Path& path) { append(UStringRef(path.m_path)); }

	/// 空文字列を設定する
	void clear() { m_path.clear(); }

	/// パスが空であるかを確認する
	bool isEmpty() const { return m_path.isEmpty(); }

	/** パス文字列の長さを返します。*/
	int getLength() const { return m_path.getLength(); }

	/** C言語形式の文字列ポインタを返します。*/
	const UChar* c_str() const { return m_path.c_str(); }

	const UString& getString() const { return m_path; }

	/** パス文字列の中から拡張子を含むファイル名の部分を返します。 (空パスの場合は空文字列を返す) */
	UString getFileName() const;

#if 0
	UStringRef getFileNameWithoutExtension() const;

	/**
		@brief		ファイルの拡張子を取得します。
		@param[in]	withDot		: true の場合、結果は '.' を含み、false の場合は含まない
		@details	拡張子を持たない場合は空文字列を返します。
		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		PathName("file.txt").GetExtension()			// => ".txt"
		PathName("file.txt").GetExtension(false)	// => "txt"
		PathName("file.tmp.txt").GetExtension()		// => ".txt"
		PathName("file").GetExtension()				// => ""
		PathName("").GetExtension()					// => ""
		PathName(".").GetExtension()				// => ""
		~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	*/
	UStringRef getExtension(bool withDot = true) const LN_NOEXCEPT;

	/** パス文字列の長さを返します。*/
	int getLength() const { return m_path.getLength(); }

	/** C言語形式の文字列ポインタを返します。*/
	const UChar* c_str() const { return m_path.c_str(); }
	
	/// パス文字列を返す
	const UString& getString() const { return m_path; }	// TODO: ToString()

	//GenericString<TCHAR> toString() const;

	///// パス文字列を返す (末尾に必ずセパレータをひとつ付加する)
	//const GenericString<TChar> getStrEndSeparator() const;

	/**
		@brief		このパスから拡張子を取り除いたパスを返します。
		@code
					PathName path1("C:/dir/file.txt");
					PathName path2 = path1.GetWithoutExtension();	// => "C:/dir/file"

					"file.txt"			=> "file"
					"file"				=> "file"
					""					=> ""
					"C:/dir.sub/file"	=> "C:/dir.sub/file"
					"dir/.git"			=> "dir/"
					".git"				=> ""
		@endcode
	*/
	Path<TChar> getWithoutExtension() const;

	/**
		@brief		このパスの拡張子を変更した新しいパスを返します。
		@param[in]	newExt	: 新しい拡張子 (先頭の . は省略しても良い)
		@code
					PathName path1("C:/dir/file.txt");
					PathName path2 = path1.GetWithoutExtension(_T(".dat"));	// => "C:/dir/file.dat"

					PathName path1("file");
					PathName path2 = path1.GetWithoutExtension(_T(".dat"));	// => "file.dat"
		@endcode
	*/
	Path<TChar> changeExtension(const TChar* newExt) const;


	/// 絶対パスであるかを確認する
	bool isAbsolute() const;

	/// ルートディレクトリであるかを確認する
	bool isRoot() const;

	/** パスの示す先がディレクトリであるかを確認します。*/
	bool isDirectory() const;

	/// 指定された拡張子を持っているかを確認する (ext の . の有無は問わない)
	bool checkExt(const TChar* ext) const;	// TODO: obsolete

	bool equalExtension(const TChar* ext) const { return checkExt(ext); }

	/**
		@brief		親ディレクトリの PathName を返す
		@details	現在のパスがルートパス ('C:\' や '/') の場合、現在のパスを返します。
					それ以外に親ディレクトリを特定できない場合は空文字を返します。
					以下の例を参考にしてください。
		@code
					"C:/dir1/file1.txt"	→ "C:/dir1"
					"C:/dir1"			→ "C:/"
					"C:/"				→ "C:/"
					"C:"				→ "C:"
					"/"					→ "/"		(Unix)
					"file1.txt"			→ ""
					".."				→ ""
					"."					→ ""
					""					→ ""
		@endcode
	*/
	Path getParent() const;

	/**
		@brief		パスを単純化し、フルパスにしたものを返す
		@details	パスが相対パスの場合はカレントディレクトリを基準に、フルパスを求めます。
					また、Windows 環境では / は \ に置き換えられます。
	*/
	Path canonicalizePath() const;

	/// ローカルの char 文字列表現として返す
	std::string toLocalChar() const;

	
	/** 
		@brief		このパスの指す先がファイルとして存在しているかを確認します。
	*/
	bool existsFile() const;

	/** 
		@brief		このパスの指す先がディレクトリとして存在しているかを確認します。
	*/
	// TODO :↑に同じようなのがある…
	bool existsDirectory() const;

	/**
		@brief		このパスの指す先がディレクトリ内に、指定した名前のファイルが存在するかを確認します。
	*/
	bool existsFileInDirectory(const StringRefT& relPath) const LN_NOEXCEPT;

	/** 
		@brief		このパスから指定したパスへの相対パスを取得します。
		@details	target とこのパスは絶対パスである必要があります。
					同じパスである場合は . が返ります。
		@attention	双方のパスはディレクトリパスである必要があります。
	*/
	Path<TChar> makeRelative(const Path<TChar>& target) const;
	// TODO: おしりに / はつかなくていい。

	/**
		@brief		このパスと、別のパス文字列が等しいかをチェックする
		@param[in]	path	: 比較対象のパス文字列
		@details	Separator と AltSeparator は等価とみなします。
					また、大文字小文字を区別しません。
		@note		環境による大文字小文字の区別について…<br>
					区別の有無は、OSではなくファイルシステム依存。
					http://ja.wikipedia.org/wiki/%E3%83%95%E3%82%A1%E3%82%A4%E3%83%AB%E3%82%B7%E3%82%B9%E3%83%86%E3%83%A0#.E6.A9.9F.E8.83.BD
					これをプログラムから調べる有効な手段が見つからなかった…。
					ちなみに、Adobe等クロスプラットフォームで動く製品を出してるところも、この辺りは十分に対応しきれていない様子。
					http://helpx.adobe.com/jp/x-productkb/global/cpsid_83180.html
					http://www.clip-studio.com/clip_site/support/faq/detail/svc/52/tid/37429
	*/
	bool equals(const TChar* path) const { return PathTraits::equals(m_path.c_str(), path); }
	/// @overload Equals
	bool equals(const Path& path) const { return PathTraits::equals(m_path.c_str(), path.c_str()); }
	/// @overload Equals
	bool equals(const UString& path) const { return PathTraits::equals(m_path.c_str(), path.c_str()); }
	/// @overload Equals
	bool operator == (const Path& path) const { return equals(path); }
#endif

	/**
		このパスが示すディレクトリを作成します。
		@details	このパスへの全てのディレクトリを作成します。既に存在する場合は作成しません。
	*/
	//void createDirectory() const;
	//void LN_AFX_FUNCNAME(createDirectory)() const;

public:

	///**
	//	@brief		カレントディレクトリのパスを取得します。
	//*/
	//static Path getCurrentDirectory();

	///** アプリケーションを開始した実行ファイルのパスを取得します。*/
	//static Path getExecutablePath();

	///**
	//	@brief		システムの特別なフォルダのパスを取得します。
	//	@param[in]	specialFolder	: フォルダの種類
	//	@param[in]	childDir		: specialFolder が示すパスの子フォルダとして結合するパス
	//	@param[in]	option			: 取得オプション
	//	@exception	ArgumentException	childDir が絶対パスです。
	//*/
	//static Path getSpecialFolderPath(SpecialFolder specialFolder, const TChar* childDir = NULL, SpecialFolderOption option = SpecialFolderOption::Create);

	///**	
	//	@brief		あるフォルダ内でユニークなファイルパス(絶対パス)を生成して返す
	//	@param[in]	directory	: フォルダパス
	//	@param[in]	filePrefix	: ファイル名の接頭辞 または NULL
	//	@param[in]	extName		: ファイルの拡張子 (プレフィックス。.を含めること) または NULL
	//	@details	生成されるファイルパスは "<directory>/<filePrefix><ID><extName>" となります。
	//				ID は時間情報をキーとして生成、<br>
	//				filePrefix、extName が NULL の場合は空文字扱いで結合されます。
	//				例えば両方 NULL にすると、番号だけのファイル名になります。
	//*/
	//static Path getUniqueFilePathInDirectory(const Path& directory, const TChar* filePrefix, const TChar* extName);

	///// (こちらはファイル名だけを返す)
	//static UString getUniqueFileNameInDirectory(const Path& directory, const TChar* filePrefix, const TChar* extName);

private:
	UString	m_path;

};


//==============================================================================
// String
//==============================================================================

inline const UChar* UString::c_str() const
{
	return (isSSO()) ? m_data.sso.buffer : ((m_data.core) ? m_data.core->get() : u"");
}

inline int UString::getLength() const
{
	return (isSSO()) ? getSSOLength() : ((m_data.core) ? m_data.core->getLength() : 0);
}

inline int UString::getCapacity() const
{
	//return (isSSO() || !m_data.core) ? SSOCapacity : m_data.core->getCapacity();
	return (isSSO()) ? SSOCapacity : ((m_data.core) ? m_data.core->getCapacity() : 0);
}

//inline UString::iterator UString::begin() { return (isSSO()) ? m_data.sso.buffer : ((m_data.core) ? m_data.core->get() : u""); }
//inline UString::const_iterator UString::begin() const { return c_str(); }
//inline UString::iterator UString::end() { return begin() + getLength(); }
//inline UString::const_iterator UString::end() const { return begin() + getLength(); }

template<typename... TArgs>
inline UString UString::format(const UStringRef& format, TArgs&&... args)
{
	auto argList = ln::fmt::detail::makeArgList<UChar>(std::forward<TArgs>(args)...);
	fmt::GenericFormatStringBuilder<UChar> sb;
	if (fmt::detail::formatInternal<UChar>(Locale::getC(), &sb, format.data(), format.getLength(), argList))
	{
		return UString(sb.c_str(), sb.getLength());
	}
	else
	{
		return UString();
	}
}

template<typename... TArgs>
inline UString UString::format(const Locale& locale, const UStringRef& format, TArgs&&... args)
{
	auto argList = ln::fmt::detail::makeArgList<UChar>(std::forward<TArgs>(args)...);
	fmt::GenericFormatStringBuilder<UChar> sb;
	if (fmt::detail::formatInternal<UChar>(locale, &sb, format.data(), format.getLength(), argList))
	{
		return UString(sb.c_str(), sb.getLength());
	}
	else
	{
		return UString();
	}
}

inline UString& UString::operator=(const UStringRef& rhs) { assign(rhs); return *this; }
inline UString& UString::operator=(const UChar* rhs) { assign(rhs); return *this; }
inline UString& UString::operator=(UChar ch) { assign(&ch, 1); return *this; }

inline UString& UString::operator+=(const UString& rhs) { append(rhs.c_str(), rhs.getLength()); return *this; }
inline UString& UString::operator+=(const UStringRef& rhs) { append(rhs.data(), rhs.getLength()); return *this; }
inline UString& UString::operator+=(const UChar* rhs) { append(rhs, UStringHelper::strlen(rhs)); return *this; }
inline UString& UString::operator+=(UChar rhs) { append(&rhs, 1); return *this; }

inline UString operator+(const UStringRef& lhs, const UStringRef& rhs) { return UString::concat(lhs, rhs); }

inline bool operator==(const UChar* lhs, const UString& rhs) { return UStringHelper::compare(lhs, rhs.c_str()) == 0; }
inline bool operator==(const UString& lhs, const UString& rhs) { return UString::compare(lhs, 0, rhs, 0) == 0; }
inline bool operator==(const UString& lhs, const UChar* rhs) { return UStringHelper::compare(lhs.c_str(), rhs) == 0; }
inline bool operator==(const UString& lhs, const UStringRef& rhs) { return UString::compare(lhs, 0, rhs, 0) == 0; }
inline bool operator!=(const UChar* lhs, const UString& rhs) { return !operator==(lhs, rhs); }
inline bool operator!=(const UString& lhs, const UString& rhs) { return !operator==(lhs, rhs); }
inline bool operator!=(const UString& lhs, const UChar* rhs) { return !operator==(lhs, rhs); }

inline bool operator<(const UString& lhs, const UString& rhs) { return UString::compare(lhs, 0, rhs, 0, std::max(lhs.getLength(), rhs.getLength()), CaseSensitivity::CaseSensitive) < 0; }
inline bool operator<(const UChar* lhs, const UString& rhs) { return UString::compare(lhs, 0, rhs, 0, -1, CaseSensitivity::CaseSensitive) < 0; }
inline bool operator<(const UString& lhs, const UChar* rhs) { return UString::compare(lhs, 0, rhs, 0, -1, CaseSensitivity::CaseSensitive) < 0; }
inline bool operator>(const UString& lhs, const UString& rhs) { return UString::compare(lhs, 0, rhs, 0, std::max(lhs.getLength(), rhs.getLength()), CaseSensitivity::CaseSensitive) > 0; }
inline bool operator>(const UChar* lhs, const UString& rhs) { return UString::compare(lhs, 0, rhs, 0, -1, CaseSensitivity::CaseSensitive) > 0; }
inline bool operator>(const UString& lhs, const UChar* rhs) { return UString::compare(lhs, 0, rhs, 0, -1, CaseSensitivity::CaseSensitive) > 0; }

inline bool operator<=(const UString& lhs, const UString& rhs) { return !operator>(lhs, rhs); }
inline bool operator<=(const UChar* lhs, const UString& rhs) { return !operator>(lhs, rhs); }
inline bool operator<=(const UString& lhs, const UChar* rhs) { return !operator>(lhs, rhs); }
inline bool operator>=(const UString& lhs, const UString& rhs) { return !operator<(lhs, rhs); }
inline bool operator>=(const UChar* lhs, const UString& rhs) { return !operator<(lhs, rhs); }
inline bool operator>=(const UString& lhs, const UChar* rhs) { return !operator<(lhs, rhs); }


//==============================================================================
// StringRef
//==============================================================================
inline bool operator==(const UStringRef& lhs, const UStringRef& rhs) { return UString::compare(lhs, 0, rhs, 0, std::max(lhs.getLength(), rhs.getLength())) == 0; }
inline bool operator==(const UChar* lhs, const UStringRef& rhs) { return UString::compare(UStringRef(lhs), 0, rhs, 0, -1) == 0; }
inline bool operator==(const UStringRef& lhs, const UChar* rhs) { return UString::compare(lhs, 0, UStringRef(rhs), 0, -1) == 0; }
inline bool operator!=(const UStringRef& lhs, const UStringRef& rhs) { return !operator==(lhs, rhs); }
inline bool operator!=(const UChar* lhs, const UStringRef& rhs) { return !operator==(lhs, rhs); }
inline bool operator!=(const UStringRef& lhs, const UChar* rhs) { return !operator==(lhs, rhs); }

#include "StringFormat.inl"

LN_NAMESPACE_END

// for unordered_map key
namespace std {
template <> struct hash<ln::UString> { std::size_t operator () (const ln::UString& key) const; };
}
