
#include "../Internal.h"
#include <sstream>
#include <Lumino/Base/ByteBuffer.h>
#include <Lumino/Text/Encoding.h>
#include <Lumino/Base/RefObject.h>
#include <Lumino/Base/String.h>
#include <Lumino/Base/StringHelper.h>
#include <Lumino/Base/Locale.h>
#include <Lumino/Base/Environment.h>
#include <Lumino/Base/StringArray.h>
#include <Lumino/Base/Hash.h>
#include <Lumino/IO/PathName.h>

#if 1
#else
/*
basic_string クラス
https://msdn.microsoft.com/ja-jp/library/syxtdd4f.aspx
CString の使用
https://msdn.microsoft.com/ja-jp/library/ms174288.aspx
String クラス
https://msdn.microsoft.com/ja-jp/library/system.string%28v=vs.110%29.aspx
QString Class
http://doc.qt.io/qt-5/qstring.html#details
wxString の概要
http://murank.github.io/wxwidgetsjp/2.9.4/overview_string.html
wxWidgets における Unicode 対応
http://murank.github.io/wxwidgetsjp/2.9.4/overview_unicode.html#overview_unicode_encodings
Tchar.h における汎用テキストのマッピング
https://msdn.microsoft.com/ja-jp/library/c426s321.aspx

[2015/1/21] char*, wchar_t* へのキャスト演算子はサポートしない。
	普段は便利でも、落とし穴に落ちたときの原因がすごくわかりづらいため。
	・Variant に入れるときとか、String と TCHAR でオーバーロード組む必要があるときに邪魔。 
	・CStringと三項演算子の問題。
	http://www.g-ishihara.com/mfc_st_02.htm

[2015/8/3] VS2013 では InterlockedIncrement() を使った参照カウント操作は std::string のコピーよりも高いスコアを出した。
	他の環境でもよいスコアが出ればスレッドセーフ化も検討するかもしれない。

	1000文字の代入を 100000 回行った平均時間は以下のとおり。
	- String (Atomic無し)	: 2ms
	- String (Atomic有り)	: 3ms
	- wstring (VS2013)		: 10ms

[2015/6/8] 共有空文字列
	
	空文字列を表すのに、いちいち char[1] を new したりしない。
	空文字列は共通の GenericStringCore::m_sharedEmpty を参照する。

	この空文字列は参照カウントを持つ。
	グローバルなインスタンスなのでプログラム終了時に解放される。

	とすると一見わざわざ GenericStringCore のインスタンスを共有する必要は無く、
	グローバルな「char g_empty[1] = "";」を共有すれば良いように見える。

	しかしそうすると、String クラスは文字列操作時、
	「今参照しているのは g_empty か？GenericStringCore か？」を区別しなければならず、冗長になる。

	空文字列もそれ以外も全て GenericStringCore として扱うことで、余計なエラーチェックが必要なくなる。


[2015/2/21] 内部文字コードは固定しない

	QString や NSString は内部文字コードを固定しているが、このライブラリの String は固定しない。
	これは、このライブラリが何かの大規模なフレームワークを作るわけではく、
	ほかのライブラリやフレームワークをサポートする目的もあるため。

	String は GenericString<> の typedef で、Windows では _UNICODE により <char> または <wchar_t> を使用する。
	Unix 系は、今のところ <char> だけを使う。(UTF-8)
	これは、Unix では wchar 系の正常動作が十分に期待できないため。
	例えば、Ubuntu では wsprintf 使うと、日本語が全て消えてしまう。
	さらに、wchar_t をサポートしているシステムAPIは無く、必ず UTF-8 に変換してからシステムAPIに渡す必要がある。
	Unix系はもう OS レベルで wchar_t を見限っていると考えた方がよい。Mac ですら NSString を UTF-16 にしている。

	完全に実装するならば printf も自分で実装する必要がある。
	ゆくゆくはタイプセーフな printf (https://github.com/c42f/tinyformat) を作ろうかと考えてはいるが、
	直近ではその重要性は低い。

	最終的には、どのようなパターンを考えても一長一短があり、内部的な理由からは決定しきれない。
	となるとユーザーが使ったときに、コードを何も変更しなくても別 OS でコンパイルできるようになることが最低条件となる。
	その次に来るのが、ユーザーが使っている他のフレームワークやライブラリと統合し易いかどうか。そしてパフォーマンス。

	FreeType を使うときは UTF-32 文字列を使えた方が良いし、CEGUI を使うときは UTF-8 文字列を使えた方が良い。
	Windows ⇔ Unix 間でファイルをやり取りするときのフォーマットは UTF-16 がベターだろう。
	Ruby と連携するときは UTF-8 だし、Java や C# とは UTF-16。
	MFC(WinAPI)と使うなら char と wchar_t をマクロで切り替えられるように。
	そして、例えば内部文字コードを固定すると IndexOf() に指定したリテラルをイチイチ文字コード変換しなければならず、パフォーマンスに影響がかなり出る。

	内部文字コードを固定しても、外部ライブラリと連携するために変換関数を作ればいいだけの話ではあるのだけど、
	このライブラリのメイン用途 (ゲームエンジン) としてはパフォーマンス解決のための手段は用意しておくべき。

	以下、メモ。

	・UTF-8を内部コードとする場合
		+ 新しい Unix 系であれば OS の内部コードが UTF-8 となっているため、それほど問題はない。
		- Windows では printf や API を呼び出すか時に THREAD_ACP に変換する必要がある。
		- Windows では 文字列リテラルは UTF-8 にコンパイルされないため、日本語文字を含む char* で String::IndexOf できない。
		  日本語をハードコーディングすることは稀だが、それでもする場合は 0 ではない。

	・UTF-16を内部コードとする場合
		+ 日本語を扱うとき、メモリ効率は UTF 系の中で一番良いし、当然 Unicode の文字を全て扱える。
		+ Windows では ≒ wchar_t と考えて wchar 系の標準ライブラリが使える。
		- ただし、Unix 計では ≠ wchar_t なので標準ライブラリに相当する関数をほぼすべて自分で実装する必要がある。

	・wchar_t を内部コードとする場合
		+ UTF-16 と比べて_T() のリテラルで、文字列検索とかし易い。
		- Unix では使い物にならないと考えた方がよい。wsprintf とか日本語が消える。UTF-16と同じく、API呼ぶときは UTF-8への変換必須。

	・ノータッチ(MFC ライク)
		+ 文字列リテラルで検索とか、printf の可変引数に指定できる。
		- wprintf とか使い物にならないのは一緒。

	参考：http://www.nubaria.com/en/blog/?p=289
*/

LN_NAMESPACE_BEGIN

//==============================================================================
// GenericStringCore
//==============================================================================
namespace detail {

#if defined(LN_DEBUG)
// UnitTest 用の変数。とりあえず Atomic じゃなくていい。
int g_testGenericStringCoreAllocCount = 0;
#endif

} // namespace detail

template<typename TChar>
typename detail::GenericStringCore<TChar> detail::GenericStringCore<TChar>::m_sharedEmpty;

//==============================================================================
// GenericString
//==============================================================================

static const int MaxFormatLength = 1024;

//------------------------------------------------------------------------------
template<typename TChar>
GenericString<TChar>::GenericString()
	: m_string(detail::GenericStringCore<TChar>::getSharedEmpty())
{
}

//------------------------------------------------------------------------------
template<typename TChar>
GenericString<TChar>::~GenericString()
{
	LN_SAFE_RELEASE(m_string);
}

//------------------------------------------------------------------------------
// TChar コンストラクタ系
//------------------------------------------------------------------------------
template<typename TChar>
GenericString<TChar>::GenericString(const GenericString& str)
	: m_string(nullptr)
{
	attach(str.m_string);
}
template<typename TChar>
GenericString<TChar>::GenericString(const GenericString& str, int length)
	: m_string(nullptr)
{
	assignTString(str.m_string->c_str(), length);
}
template<typename TChar>
GenericString<TChar>::GenericString(const GenericString& str, int begin, int length)
	: m_string(nullptr)
{
	assignTString(str.m_string->c_str() + begin, length);	// str+begin にしないと、暗黙的コンストラクタの呼び出しが発生してしまう
}
template<typename TChar>
GenericString<TChar>::GenericString(const StringRefT& str)
	: m_string(nullptr)
{
	if (str.m_string != nullptr)
		attach(str.m_string);
	else
		assignTString(str.getBegin(), str.getLength());
}
template<typename TChar>
GenericString<TChar>::GenericString(const TChar* str)
	: m_string(nullptr)
{
	assignTString(str, -1);
}
template<typename TChar>
GenericString<TChar>::GenericString(const TChar* str, int length)
	: m_string(nullptr)
{
	assignTString(str, length);
}
template<typename TChar>
GenericString<TChar>::GenericString(const TChar* str, int begin, int length)
	: m_string(nullptr)
{
	assignTString(str + begin, length);
}
template<typename TChar>
GenericString<TChar>::GenericString(TChar ch)
	: GenericString(&ch, 1)
{
}

template<typename TChar>
GenericString<TChar>::GenericString(int count, TChar ch)
	: m_string(nullptr)
{
	assignTString(count, ch);
}

template<typename TChar>
GenericString<TChar>::GenericString(GenericString&& str) LN_NOEXCEPT
	: m_string(nullptr)
{
	*this = std::move(str);
}


//------------------------------------------------------------------------------
// YCHAR コンストラクタ系
//------------------------------------------------------------------------------
template<typename TChar>
GenericString<TChar>::GenericString(const GenericString<YCHAR>& str)
	: m_string(nullptr)
{
	assignCStr(str.c_str(), str.getLength());
}
template<typename TChar>
GenericString<TChar>::GenericString(const GenericString<YCHAR>& str, int length)
	: m_string(nullptr)
{
	assignCStr(str.c_str(), length);
}
template<typename TChar>
GenericString<TChar>::GenericString(const GenericString<YCHAR>& str, int begin, int length)
	: m_string(nullptr)
{
	assignCStr(str.c_str(), begin, length);
}
template<typename TChar>
GenericString<TChar>::GenericString(const YCHAR* str)
	: m_string(nullptr)
{
	assignCStr(str);
}
template<typename TChar>
GenericString<TChar>::GenericString(const YCHAR* str, int length)
	: m_string(nullptr)
{
	assignCStr(str, length);
}

//template<typename TChar>
//GenericString<TChar>::GenericString(const GenericString<YCHAR>& str)
//	: m_ref(nullptr)
//	, m_string(nullptr)
//{
//	AssignCStr(str.c_str());
//}
//template<typename TChar>
//GenericString<TChar>::GenericString(const GenericString<YCHAR>& str, int length)
//	: m_ref(nullptr)
//	, m_string(nullptr)
//{
//	AssignCStr(str.c_str(), length);
//}
//template<typename TChar>
//GenericString<TChar>::GenericString(const GenericString<YCHAR>& str, int begin, int length)
//	: m_ref(nullptr)
//	, m_string(nullptr)
//{
//	AssignCStr(str.c_str(), begin, length);
//}
//template<typename TChar>
//GenericString<TChar>::GenericString(const YCHAR* str)
//	: m_ref(nullptr)
//	, m_string(nullptr)
//{
//	AssignCStr(str);
//}
//template<typename TChar>
//GenericString<TChar>::GenericString(const YCHAR* str, int length)
//	: m_ref(nullptr)
//	, m_string(nullptr)
//{
//	AssignCStr(str, length);
//}
//template<typename TChar>
//GenericString<TChar>::GenericString(const YCHAR* str, int begin, int length)
//	: m_ref(nullptr)
//	, m_string(nullptr)
//{
//	AssignCStr(str, begin, length);
//}

//------------------------------------------------------------------------------
// operator=
//------------------------------------------------------------------------------
template<typename TChar>
GenericString<TChar>& GenericString<TChar>::operator=(const GenericString& right)
{
	attach(right.m_string);
	return (*this);
}
template<typename TChar>
GenericString<TChar>& GenericString<TChar>::operator=(const GenericStringRef<TChar>& right)
{
	assignTString(right.getBegin(), right.getLength());
	return (*this);
}
template<typename TChar>
GenericString<TChar>& GenericString<TChar>::operator=(const std::basic_string<TChar>& right)
{
	assignTString(right.c_str(), -1);
	return (*this);
}
template<typename TChar>
GenericString<TChar>& GenericString<TChar>::operator=(const TChar* right)
{
	assignTString(right, -1);
	return (*this);
}
template<typename TChar>
GenericString<TChar>& GenericString<TChar>::operator=(GenericString&& right) LN_NOEXCEPT
{
	if (m_string != right.m_string)
	{
		LN_SAFE_RELEASE(m_string);
		m_string = right.m_string;
		right.m_string = nullptr;
	}
	return *this;
}

//------------------------------------------------------------------------------
// operator+=
//------------------------------------------------------------------------------
template<typename TChar>
GenericString<TChar>& GenericString<TChar>::operator+=(const GenericString<TChar>& right)
{
	append(right.m_string->c_str(), right.getLength());
	return (*this);
}
//template<typename TChar>
//GenericString<TChar>& GenericString<TChar>::operator+=(const std::basic_string<TChar>& right)
//{
//	Append(right.c_str(), right.size());
//	return (*this);
//}
template<typename TChar>
GenericString<TChar>& GenericString<TChar>::operator+=(const TChar* ptr)
{
	append(ptr, -1);
	return (*this);
}
template<typename TChar>
GenericString<TChar>& GenericString<TChar>::operator+=(TChar ch)
{
	append(&ch, 1);
	return (*this);
}

//------------------------------------------------------------------------------
// operator<
//------------------------------------------------------------------------------
template<typename TChar>
bool GenericString<TChar>::operator < (const GenericString& right) const
{
	return compare(right.c_str(), right.getLength(), CaseSensitivity::CaseSensitive) < 0;
}
template<typename TChar>
bool GenericString<TChar>::operator < (const GenericStringRef<TChar>& right) const
{
	return compare(right.getBegin(), right.getLength(), CaseSensitivity::CaseSensitive) < 0;
}
template<typename TChar>
bool GenericString<TChar>::operator < (const TChar* right) const
{
	return compare(right, -1, CaseSensitivity::CaseSensitive) < 0;
}

//------------------------------------------------------------------------------
// operator>
//------------------------------------------------------------------------------
template<typename TChar>
bool GenericString<TChar>::operator > (const GenericString& right) const
{
	return compare(right.c_str(), -1, CaseSensitivity::CaseSensitive) > 0;
}
template<typename TChar>
bool GenericString<TChar>::operator > (const GenericStringRef<TChar>& right) const
{
	return compare(right.getBegin(), right.getLength(), CaseSensitivity::CaseSensitive) > 0;
}
template<typename TChar>
bool GenericString<TChar>::operator > (const TChar* right) const
{
	return compare(right, -1, CaseSensitivity::CaseSensitive) > 0;
}

//------------------------------------------------------------------------------
// operator[]
//------------------------------------------------------------------------------
//template<typename TChar>
//TChar& GenericString<TChar>::operator[](int index)
//{
//	return InternalGetAt(index);
//}
template<typename TChar>
const TChar& GenericString<TChar>::operator[](int index) const
{
	return internalGetAt(index);
}

//------------------------------------------------------------------------------
// operator  cast
//------------------------------------------------------------------------------
//template<typename TChar>
//GenericString<TChar>::operator const TChar*() const
//{
//	return c_str();
//}

//------------------------------------------------------------------------------
template<typename TChar>
const TChar* GenericString<TChar>::c_str() const
{
	return m_string->c_str();
}

//------------------------------------------------------------------------------
template<typename TChar>
int GenericString<TChar>::getLength() const
{
	return (int)m_string->size();
}

//------------------------------------------------------------------------------
template<typename TChar>
bool GenericString<TChar>::isEmpty() const
{
	return m_string->empty();
}

//------------------------------------------------------------------------------
template<typename TChar>
void GenericString<TChar>::append(const GenericString& str, int len)
{
	append(str.m_string->c_str(), str.getLength());
}
//template<typename TChar>
//void GenericString<TChar>::Append(const std::basic_string<TChar>& str, int len)
//{
//	Append(str.c_str(), str.size());
//}
template<typename TChar>
void GenericString<TChar>::append(const TChar* str, int len)
{
	if (str == nullptr || len == 0) {
		return;		// 空文字列なので何もしない
	}
	realloc();	// 共有参照を切る
	m_string->append(str, (len < 0) ? StringTraits::tcslen(str) : len);
}
template<typename TChar>
void GenericString<TChar>::append(TChar ch)
{
	append(&ch, 1);
}

//------------------------------------------------------------------------------
template<typename TChar>
void GenericString<TChar>::assignCStr(const char* str, int begin, int length, bool* outUsedDefaultChar)
{
	if (str == nullptr) {
		clear();
		return;
	}
	//LN_THROW(str != nullptr, ArgumentException);	// std::string の assign は nullptr が渡されたときの動作は未定義。VS2013 では制御が返ってこなくなった

	if (length <= -1) {
		length = INT_MAX;
	}
	int len = std::min((int)strlen(str), length);

	// サイズ 0 なら空文字列にするだけ
	if (len == 0) {
		clear();
	}
	else {
		convertFrom(str + begin, len, Encoding::getSystemMultiByteEncoding());
	}
}
template<typename TChar>
void GenericString<TChar>::assignCStr(const char* str, int length, bool* usedDefaultChar)
{
	assignCStr(str, 0, length, usedDefaultChar);
}
template<typename TChar>
void GenericString<TChar>::assignCStr(const char* str, bool* usedDefaultChar)
{
	assignCStr(str, 0, -1, usedDefaultChar);
}

//------------------------------------------------------------------------------
template<typename TChar>
void GenericString<TChar>::assignCStr(const wchar_t* str, int begin, int length, bool* outUsedDefaultChar)
{
	if (str == nullptr) {
		clear();
		return;
	}
	//LN_THROW(str != nullptr, ArgumentException);	// std::string の assign は nullptr が渡されたときの動作は未定義。VS2013 では制御が返ってこなくなった

	if (length <= -1) {
		length = (int)wcslen(str);
		//length = INT_MAX;
	}
	int len = length * sizeof(wchar_t);

	// サイズ 0 なら空文字列にするだけ
	if (len == 0) {
		clear();
	}
	else {
		convertFrom(str + begin, len, Encoding::getWideCharEncoding());
	}
}
template<typename TChar>
void GenericString<TChar>::assignCStr(const wchar_t* str, int length, bool* usedDefaultChar)
{
	assignCStr(str, 0, length, usedDefaultChar);
}
template<typename TChar>
void GenericString<TChar>::assignCStr(const wchar_t* str, bool* usedDefaultChar)
{
	assignCStr(str, 0, -1, usedDefaultChar);
}

//------------------------------------------------------------------------------
template<typename TChar>
void GenericString<TChar>::convertFrom(const void* buffer, int byteCount, const Encoding* encoding, bool* outUsedDefaultChar)
{
	LN_THROW(encoding, ArgumentException);

	Encoding* thisTypeEncoding = getThisTypeEncoding();

	// 全く同じエンコーディングなら変換の必要は無い
	if (thisTypeEncoding == encoding) 
	{
		byteCount = (byteCount < 0) ? strlen((const char*)buffer) : byteCount;
		assignTString((const TChar*)buffer, byteCount / sizeof(TChar));
	}
	else
	{
		EncodingConversionOptions options;
		options.NullTerminated = false;

		EncodingConversionResult result;
		const ByteBuffer tmpBuffer = Encoding::convert(buffer, byteCount, encoding, thisTypeEncoding, options, &result);
		if (outUsedDefaultChar != nullptr) {
			*outUsedDefaultChar = result.UsedDefaultChar;
		}

		assignTString((const TChar*)tmpBuffer.getData(), result.BytesUsed / sizeof(TChar));
	}
}

//------------------------------------------------------------------------------
template<typename TChar>
ByteBuffer GenericString<TChar>::convertTo(const Encoding* encoding, bool* outUsedDefaultChar) const
{
	EncodingConversionOptions options;
	options.NullTerminated = true;

	EncodingConversionResult result;
	const ByteBuffer buf = Encoding::convert(c_str(), getByteCount(), getThisTypeEncoding(), encoding, options, &result);
	if (outUsedDefaultChar != nullptr) {
		*outUsedDefaultChar = result.UsedDefaultChar;
	}

	return buf;
}

//------------------------------------------------------------------------------
template<typename TChar>
void GenericString<TChar>::clear()
{
	attach(detail::GenericStringCore<TChar>::getSharedEmpty());
}

//------------------------------------------------------------------------------
template<typename TChar>
GenericString<TChar> GenericString<TChar>::subString(int startIndex, int length) const
{
	return StringT(*this, startIndex, length);
}

//------------------------------------------------------------------------------
template<typename TChar>
GenericString<TChar> GenericString<TChar>::trim() const
{
	const TChar* begin;
	int length;
	StringTraits::trim(c_str(), getLength(), &begin, &length);
	return GenericString<TChar>(begin, length);
}

//------------------------------------------------------------------------------
template<typename TChar>
struct CmpCaseSensitive
{
	TChar ch;
	bool operator()(TChar value) { return ch == value; }
};

template<typename TChar>
struct CmpCaseInsensitive
{
	TChar ch;
	bool operator()(TChar value) { return StringTraits::toUpper(ch) == StringTraits::toUpper(value); }
};

template<typename TChar>
GenericString<TChar> GenericString<TChar>::remove(TChar ch, CaseSensitivity cs) const
{
	GenericString<TChar> newStr(*this);
	if (newStr.isEmpty()) {
		// 空文字列なら処理を行う必要は無い。というか、SharedEmpty に対して erase とかしてはいけない。
		return newStr;
	}

	// 大文字と小文字を区別する
	detail::GenericStringCore<TChar>& ss = *newStr.m_string;
	if (cs == CaseSensitivity::CaseSensitive)
	{
		CmpCaseSensitive<TChar> cmp;
		cmp.ch = ch;
		ss.erase(std::remove_if(ss.begin(), ss.end(), cmp), ss.end());
	}
	// 大文字と小文字を区別しない
	else //if (cs == CaseSensitivity::CaseSensitive)
	{
		CmpCaseInsensitive<TChar> cmp;
		cmp.ch = ch;
		ss.erase(std::remove_if(ss.begin(), ss.end(), cmp), ss.end());
	}

	return newStr;
}

//------------------------------------------------------------------------------
template<typename TChar>
GenericString<TChar> GenericString<TChar>::replace(const StringRefT& from, const StringRefT& to) const
{
	GenericString<TChar> newStr;
	newStr.assignTString(c_str(), getLength());
	if (newStr.isEmpty()) {
		// 空文字列なら処理を行う必要は無い。というか、SharedEmpty に対して replace とかしてはいけない。
		return newStr;
	}

	size_type pos = 0;
	size_t fromLength = from.getLength();
	size_t toLength = to.getLength();

	while (pos = newStr.m_string->find(from.getBegin(), pos, fromLength), pos != std::basic_string<TChar>::npos)
	{
		newStr.m_string->replace(pos, fromLength, to.getBegin(), toLength);
		pos += toLength;
	}

	return newStr;
}


//------------------------------------------------------------------------------
template<typename TChar>
bool GenericString<TChar>::contains(const TChar* str, CaseSensitivity cs) const
{
	return indexOf(str, 0, cs) >= 0;
}

//------------------------------------------------------------------------------
template<typename TChar>
bool GenericString<TChar>::contains(TChar ch, CaseSensitivity cs) const
{
	TChar str[2] = { ch, 0x00 };
	return indexOf(str, 0, cs) >= 0;
}

//------------------------------------------------------------------------------
template<typename TChar>
int GenericString<TChar>::indexOf(const StringRefT& str, int startIndex, CaseSensitivity cs) const
{
	return StringTraits::indexOf(c_str(), getLength(), str.getBegin(), str.getLength(), startIndex, cs);
}

//------------------------------------------------------------------------------
template<typename TChar>
int GenericString<TChar>::indexOf(TChar ch, int startIndex, CaseSensitivity cs) const
{
	TChar str[2] = { ch, 0x00 };
	return StringTraits::indexOf(c_str(), getLength(), str, (int)StringTraits::tcslen(str), startIndex, cs);
}

//------------------------------------------------------------------------------
template<typename TChar>
int GenericString<TChar>::lastIndexOf(const TChar* str, int startIndex, int count, CaseSensitivity cs) const
{
	return StringTraits::lastIndexOf(c_str(), getLength(), str, StringTraits::tcslen(str), startIndex, count, cs);
}
template<typename TChar>
int GenericString<TChar>::lastIndexOf(TChar ch, int startIndex, int count, CaseSensitivity cs) const
{
	return StringTraits::lastIndexOf(c_str(), getLength(), &ch, 1, startIndex, count, cs);
}

//------------------------------------------------------------------------------
template<typename TChar>
bool GenericString<TChar>::startsWith(const TChar* str, CaseSensitivity cs) const
{
	return StringTraits::startsWith(c_str(), getLength(), str, StringTraits::tcslen(str), cs);
}
template<typename TChar>
bool GenericString<TChar>::startsWith(TChar ch, CaseSensitivity cs ) const
{
	return StringTraits::endsWith(c_str(), getLength(), &ch, 1, cs);
}

//------------------------------------------------------------------------------
template<typename TChar>
bool GenericString<TChar>::endsWith(const TChar* str, CaseSensitivity cs) const
{
	return StringTraits::endsWith(c_str(), getLength(), str, StringTraits::tcslen(str), cs);
}
template<typename TChar>
bool GenericString<TChar>::endsWith(TChar ch, CaseSensitivity cs) const
{
	return StringTraits::endsWith(c_str(), getLength(), &ch, 1, cs);
}

//------------------------------------------------------------------------------
template<typename TChar>
bool GenericString<TChar>::equals(const GenericString& str) const
{
	if (getLength() != str.getLength()) {
		return false;
	}
	return compare(str.c_str(), str.getLength(), CaseSensitivity::CaseSensitive) == 0;
}
template<typename TChar>
bool GenericString<TChar>::equals(const GenericStringRef<TChar>& str) const
{
	return compare(str.getBegin(), str.getLength(), CaseSensitivity::CaseSensitive) == 0;
}
template<typename TChar>
bool GenericString<TChar>::equals(const TChar* str) const
{
	return compare((str) ? str : getEmpty().c_str(), -1, CaseSensitivity::CaseSensitive) == 0;
}

//------------------------------------------------------------------------------
template<typename TChar>
int GenericString<TChar>::compare(const TChar* str, int count, CaseSensitivity cs) const
{
	return StringTraits::compare(c_str(), getLength(), str, count, -1/*std::max(GetLength(), count)*/, cs);
}

//------------------------------------------------------------------------------
template<typename TChar>
GenericString<TChar> GenericString<TChar>::toUpper() const
{
	GenericString<TChar> newStr(c_str(), getLength());
	std::transform(newStr.m_string->begin(), newStr.m_string->end(), newStr.m_string->begin(), StringTraits::toUpper<TChar>);
	return newStr;
}

//------------------------------------------------------------------------------
template<typename TChar>
GenericString<TChar> GenericString<TChar>::toLower() const
{
	GenericString<TChar> newStr(c_str(), getLength());
	std::transform(newStr.m_string->begin(), newStr.m_string->end(), newStr.m_string->begin(), StringTraits::toLower<TChar>);
	return newStr;
}

//------------------------------------------------------------------------------
template<typename TChar>
GenericString<TChar> GenericString<TChar>::toTitleCase() const
{
	GenericString<TChar> newStr(c_str(), getLength());
	std::transform(newStr.m_string->begin(), newStr.m_string->end(), newStr.m_string->begin(), StringTraits::toLower<TChar>);
	if (newStr.m_string->size() > 0) (*newStr.m_string)[0] = StringTraits::toUpper<TChar>((*newStr.m_string)[0]);
	return newStr;
}

//------------------------------------------------------------------------------
#define TO_INT_DEF(type, func) \
	const TChar* str; \
	const TChar* end; \
	int len; \
	NumberConversionResult res; \
	StringTraits::trim(c_str(), getLength(), &str, &len); \
	type num = StringTraits::func(str, len, base, &end, &res); \
	if (res == NumberConversionResult::ArgsError)	{ LN_THROW(0, ArgumentException); } \
	if (res == NumberConversionResult::FormatError)	{ LN_THROW(0, InvalidFormatException); } \
	if (res == NumberConversionResult::Overflow)		{ LN_THROW(0, OverflowException); } \
	LN_THROW(end == str + len, InvalidFormatException); \
	return num;

template<typename TChar>
int8_t GenericString<TChar>::toInt8(int base) const { TO_INT_DEF(int8_t, toInt8); }
template<typename TChar>
int16_t GenericString<TChar>::toInt16(int base) const { TO_INT_DEF(int16_t, toInt16); }
template<typename TChar>
int32_t GenericString<TChar>::toInt32(int base) const { TO_INT_DEF(int32_t, toInt32); }
template<typename TChar>
int64_t GenericString<TChar>::toInt64(int base) const { TO_INT_DEF(int64_t, toInt64); }
template<typename TChar>
uint8_t GenericString<TChar>::toUInt8(int base) const { TO_INT_DEF(uint8_t, toUInt8); }
template<typename TChar>
uint16_t GenericString<TChar>::toUInt16(int base) const { TO_INT_DEF(uint16_t, toUInt16); }
template<typename TChar>
uint32_t GenericString<TChar>::toUInt32(int base) const { TO_INT_DEF(uint32_t, toUInt32); }
template<typename TChar>
uint64_t GenericString<TChar>::toUInt64(int base) const { TO_INT_DEF(uint64_t, toUInt64); }

#undef TO_INT_DEF

//------------------------------------------------------------------------------
#define TRY_TO_INT_DEF(type, func) \
	const TChar* str; \
	const TChar* end; \
	int len; \
	NumberConversionResult res; \
	StringTraits::trim(c_str(), getLength(), &str, &len); \
	type num = StringTraits::func(str, len, base, &end, &res); \
	if (end != str + len) { return false; } \
	if (res != NumberConversionResult::Success) { return false; } \
	if (outValue != nullptr) { *outValue = num; } \
	return true;

template<typename TChar>
bool GenericString<TChar>::tryToInt8(int8_t* outValue, int base) const { TRY_TO_INT_DEF(int8_t, toInt8); }
template<typename TChar>
bool GenericString<TChar>::tryToInt16(int16_t* outValue, int base) const { TRY_TO_INT_DEF(int16_t, toInt16); }
template<typename TChar>
bool GenericString<TChar>::tryToInt32(int32_t* outValue, int base) const { TRY_TO_INT_DEF(int32_t, toInt32); }
template<typename TChar>
bool GenericString<TChar>::tryToInt64(int64_t* outValue, int base) const { TRY_TO_INT_DEF(int64_t, toInt64); }
template<typename TChar>
bool GenericString<TChar>::tryToUInt8(uint8_t* outValue, int base) const { TRY_TO_INT_DEF(uint8_t, toUInt8); }
template<typename TChar>
bool GenericString<TChar>::tryToUInt16(uint16_t* outValue, int base) const { TRY_TO_INT_DEF(uint16_t, toUInt16); }
template<typename TChar>
bool GenericString<TChar>::tryToUInt32(uint32_t* outValue, int base) const { TRY_TO_INT_DEF(uint32_t, toUInt32); }
template<typename TChar>
bool GenericString<TChar>::tryToUInt64(uint64_t* outValue, int base) const { TRY_TO_INT_DEF(uint64_t, toUInt64); }

#undef TRY_TO_INT_DEF


//------------------------------------------------------------------------------
template<>
GenericString<char>	GenericString<char>::toStringA() const
{
	return *this;
}
template<>
GenericString<char>	GenericString<wchar_t>::toStringA() const
{
	GenericString<char> str;
	str.assignCStr(this->c_str());
	return str;
}

//------------------------------------------------------------------------------
template<>
GenericString<wchar_t> GenericString<char>::toStringW() const
{
	GenericString<wchar_t> str;
	str.assignCStr(this->c_str());
	return str;
}
template<>
GenericString<wchar_t>	GenericString<wchar_t>::toStringW() const
{
	return *this;
}

//------------------------------------------------------------------------------
template<typename TChar>
GenericString<TChar> GenericString<TChar>::fromNativeCharString(const char* str, int length)
{
	GenericString<TChar> out;
	out.assignCStr(str, (length < 0) ? StringTraits::tcslen(str) : length);
	return out;
}
template<typename TChar>
GenericString<TChar> GenericString<TChar>::fromNativeCharString(const wchar_t* str, int length)
{
	GenericString<TChar> out;
	out.assignCStr(str, (length < 0) ? StringTraits::tcslen(str) : length);
	return out;
}

//------------------------------------------------------------------------------
template<typename TChar>
const GenericString<TChar>& GenericString<TChar>::getNewLine()
{
	static GenericString<TChar> nl(Environment::getNewLine<TChar>());
	return nl;
}

//------------------------------------------------------------------------------
template<typename TChar>
const GenericString<TChar>& GenericString<TChar>::getEmpty()
{
	static GenericString<TChar> str;
	return str;
}

//------------------------------------------------------------------------------
template<typename TChar>
size_t GenericString<TChar>::getHashCode() const
{
	if (getLength() == 0) return 0;
	return ln::Hash::calcHash(c_str(), getLength());
}

//------------------------------------------------------------------------------
template<typename TChar>
TChar* GenericString<TChar>::getData()
{
	realloc();
	return &m_string->at(0);
}

//------------------------------------------------------------------------------
template<typename TChar>
GenericString<TChar> GenericString<TChar>::sprintf(const GenericString<TChar>& format, ...)
{
	// http://jumble-note.blogspot.jp/2012/09/c-vacopy.html
	const TChar* fmt = format.c_str();	// VS2015 エラー回避。一度変数に入れる。
	va_list args1, args2;
	va_start(args1, fmt);
	va_copy(args2, args1);
	int len = StringTraits::tvscprintf_l(format.c_str(), Locale::getC().getNativeLocale(), args1);	// 文字数を求める

	// 文字数が一定以内ならメモリ確保せずにスタックを使い、速度向上を図る
	if (len < MaxFormatLength)
	{
		TChar buf[MaxFormatLength + 1];
		memset(buf, 0, sizeof(buf));
		StringTraits::tvsnprintf_l(buf, MaxFormatLength + 1, format.c_str(), Locale::getDefault().getNativeLocale(), args2);
		va_end(args1);
		va_end(args2);
		return GenericString<TChar>(buf);
	}
	else
	{
		ByteBuffer buf(len + 1);
		StringTraits::tvsnprintf_l((TChar*)buf.getData(), len + 1, format.c_str(), Locale::getDefault().getNativeLocale(), args2);
		va_end(args1);
		va_end(args2);
		return GenericString<TChar>((TChar*)buf.getData());
	}
}
template<typename TChar>
GenericString<TChar> GenericString<TChar>::sprintf(const TChar* format, ...)
{
	va_list args1, args2;
	va_start(args1, format);
	va_copy(args2, args1);
	int len = StringTraits::tvscprintf_l(format, Locale::getC().getNativeLocale(), args1);	// 文字数を求める

	// 文字数が一定以内ならメモリ確保せずにスタックを使い、速度向上を図る
	if (len < MaxFormatLength)
	{
		TChar buf[MaxFormatLength + 1];
		memset(buf, 0, sizeof(buf));
		StringTraits::tvsnprintf_l(buf, MaxFormatLength + 1, format, Locale::getDefault().getNativeLocale(), args2);
		va_end(args1);
		va_end(args2);
		return GenericString<TChar>(buf);
	}
	else
	{
		ByteBuffer buf((len + 1) * sizeof(TChar));
		int ll = StringTraits::tvsnprintf_l((TChar*)buf.getData(), len + 1, format, Locale::getDefault().getNativeLocale(), args2);
		va_end(args1);
		va_end(args2);
		return GenericString<TChar>((TChar*)buf.getData(), len);
	}
}

//------------------------------------------------------------------------------
template<typename TChar>
void GenericString<TChar>::detach() LN_NOEXCEPT
{
	LN_SAFE_RELEASE(m_string);
}

//------------------------------------------------------------------------------
template<typename TChar>
void GenericString<TChar>::attach(detail::GenericStringCore<TChar>* core)
{
	LN_REFOBJ_SET(m_string, core);
}

//------------------------------------------------------------------------------
template<typename TChar>
void GenericString<TChar>::assignTString(const TChar* str, int len)
{
	LN_SAFE_RELEASE(m_string);
	if (str == nullptr || len == 0)
	{
		// 空の文字列になる場合は共有の空文字列を参照する
		attach(detail::GenericStringCore<TChar>::getSharedEmpty());
	}
	else 
	{
		m_string = LN_NEW detail::GenericStringCore<TChar>();	// 参照カウントは 1
		m_string->assign(str, (len < 0) ? StringTraits::tcslen(str) : len);
	}
}

//------------------------------------------------------------------------------
template<typename TChar>
void GenericString<TChar>::assignTString(int count, TChar ch)
{
	LN_SAFE_RELEASE(m_string);
	if (count == 0)
	{
		// 空の文字列になる場合は共有の空文字列を参照する
		attach(detail::GenericStringCore<TChar>::getSharedEmpty());
	}
	else
	{
		m_string = LN_NEW detail::GenericStringCore<TChar>();	// 参照カウントは 1
		m_string->assign((size_t)count, ch);
	}
}

//------------------------------------------------------------------------------
// ソース文字列の共有参照を切り、新しい GenericStringCore を確保する
//------------------------------------------------------------------------------
template<typename TChar>
void GenericString<TChar>::realloc()
{
	if (m_string->isShared())
	{
		detail::GenericStringCore<TChar>* old = m_string;
		m_string = LN_NEW detail::GenericStringCore<TChar>();	// 参照カウントは 1
		m_string->assign(old->c_str());
		LN_SAFE_RELEASE(old);
	}
	else {
		// 共有参照されていなければそのまま使い続ければよい
	}
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
template<typename TChar>
TChar& GenericString<TChar>::internalGetAt(int index)
{
	LN_ASSERT(0 <= index && index < getLength());
	return m_string->at(index);
}

//------------------------------------------------------------------------------
// 
//------------------------------------------------------------------------------
template<typename TChar>
const TChar& GenericString<TChar>::internalGetAt(int index) const
{
	LN_ASSERT(0 <= index && index < getLength());
	return m_string->at(index);
}

//------------------------------------------------------------------------------
template<typename TChar>
Encoding* GenericString<TChar>::getThisTypeEncoding() const
{
	if (sizeof(TChar) == sizeof(char))
	{
		// this のエンコーディングはシステム依存である
		return Encoding::getSystemMultiByteEncoding();
	}
	else if (sizeof(TChar) == sizeof(wchar_t))
	{
		// this のエンコーディングは wchar_t である
		return Encoding::getWideCharEncoding();
	}
	else
	{
		LN_NOTIMPLEMENTED();
	}
}

// テンプレートのインスタンス化
template class GenericString<char>;
template class GenericString<wchar_t>;

LN_NAMESPACE_END


namespace std {

#ifdef LN_USTRING
#else
// for unordered_map key
std::size_t hash<ln::StringA>::operator () (const ln::StringA& key) const
{
	return ln::Hash::calcHash(key.c_str(), key.getLength());
}

std::size_t hash<ln::StringW>::operator () (const ln::StringW& key) const
{
	return ln::Hash::calcHash(key.c_str(), key.getLength());
}
#endif

} // namespace std

#endif

