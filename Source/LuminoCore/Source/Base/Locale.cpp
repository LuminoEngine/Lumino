/*
	■ CLDR(Common Locale Data Repository)
	http://cldr.unicode.org/
	ライセンスは apache-license みたい。

	・twitter-cldr-rb
	https://github.com/twitter/twitter-cldr-rb


	■ GNU C ロケール名　書式
	http://www.gnu.org/software/libc/manual/html_node/Locale-Names.html#Locale-Names

	■ MSDN ロケール名　書式
	https://msdn.microsoft.com/ja-jp/library/hzz3tw78.aspx


	■ 各OSの標準ライブラリの locale 事情
		ロケールを考慮した文字列生成を、スレッドセーフで実装したいのだが・・・。

		共通:
			setlocale は「プログラム」のロケールを変更する、とある。
			スレッド単位と決め付けることは出来ないので、基本的に使わない方針で。

		Windows:
			_printf_l のように、_l が付いている関数はロケールを引数で指定できる。

		Max:
			(実際に動かしてはいないが) Windows と同じく、printf_l のような関数が用意されているようだ。
			ただし、Windows と違って先頭に _ が付かない。

		Linux:
			これも種類によって差が大きい。
			FreeBSD は Max と同様らしい。
			Ubuntu は _l が付く関数は無い。変わりに、uselocale を使うとスレッドのロケールを変更できる。
			(__locale_raii でググるといろいろ出てくる)
		
*/

#include "../Internal.h"
#if defined(LN_OS_WIN32)
#include <windows.h>
#endif
#include <locale.h>
#include <Lumino/Base/Locale.h>

LN_NAMESPACE_BEGIN

//==============================================================================
// Locale
//==============================================================================


#if defined(LN_OS_WIN32)
//---------------------------------------------
class ScopedLocaleRAII
{
public:
	ScopedLocaleRAII(NativeLocale_t loc) {}
};

//---------------------------------------------
static NativeLocale_t CreateNativeLocale(const wchar_t* locale)
{
	return _wcreate_locale(LC_ALL, locale);
}

//---------------------------------------------
static void GetNativeDefaultLocale(NativeLocale_t* outLocale, StringW* outName)
{
	// setlocale を使用した方法は Windows でも可能だが、取得できるのは必ずシステムロケールになってしまう。
	// Qt 等のほかのフレームワークでもユーザーロケールを優先しているし、
	// Office 等のメジャーなツールもユーザーロケールでUIを表示している。
	// これらに合わせておく。
	WCHAR strNameBuffer[LOCALE_NAME_MAX_LENGTH];
	LCID  lcid = ::GetUserDefaultLCID();
	if (LCIDToLocaleName(lcid, strNameBuffer, LOCALE_NAME_MAX_LENGTH, 0) == 0)
	{
		// エラーが発生したら変わりに "C" を使う
		*outLocale = _wcreate_locale(LC_ALL, L"C");
		*outName = L"C";
		return;
	}
	*outLocale = _wcreate_locale(LC_ALL, strNameBuffer);
	*outName = strNameBuffer;
}

#else
//---------------------------------------------
class ScopedLocaleRAII
{
	locale_t m_old;
public:
	ScopedLocaleRAII(NativeLocale_t loc)
	{
		m_old = uselocale(loc);
	}
	~ScopedLocaleRAII()
	{
		uselocale(m_old);
	}
};
//---------------------------------------------
static NativeLocale_t CreateNativeLocale(const char* locale)
{
	return newlocale(LC_ALL_MASK, locale, NULL);
}
//---------------------------------------------
static void GetNativeDefaultLocale(NativeLocale_t* outLocale, StringA* outName)
{
	//// How Programs Set the Locale
	//// http://www.gnu.org/software/libc/manual/html_node/Setting-the-Locale.html
	//StringA oldLocale = setlocale(LC_ALL, NULL);
	//char* newLocale = setlocale(LC_ALL, "");
	//setlocale(LC_ALL, oldLocale.c_str());

	*outLocale = newlocale(LC_ALL_MASK, "", NULL);
	*outName = "";
}
#endif

static void FreeNativeLocale(NativeLocale_t locale)
{
#ifdef LN_OS_WIN32
	_free_locale(locale);
#else
	freelocale(locale);
#endif
}

//------------------------------------------------------------------------------
Locale::Locale()
	: m_nativeLocale(0)
	, m_nativeName()
{
	GetNativeDefaultLocale(&m_nativeLocale, &m_nativeName);
	StringA name = m_nativeName.ToStringA();
	m_stdLocale = std::locale(name.c_str());
}

//------------------------------------------------------------------------------
Locale::Locale(const TCHAR* name)
: m_nativeLocale(0)
#if defined(LN_OS_WIN32)
	, m_nativeName(StringW::FromNativeCharString(name))
#else
    , m_nativeName(StringA::FromNativeCharString(name))
#endif
{
	m_nativeLocale = CreateNativeLocale(m_nativeName.c_str());
	StringA t = m_nativeName.ToStringA();
	m_stdLocale = std::locale(t.c_str());
}

//------------------------------------------------------------------------------
Locale::Locale(const Locale& locale)
	: m_nativeLocale(0)
	, m_nativeName(locale.m_nativeName)
{
	m_nativeLocale = CreateNativeLocale(m_nativeName.c_str());
	m_stdLocale = locale.m_stdLocale;
}

//------------------------------------------------------------------------------
Locale& Locale::operator=(const Locale& locale)
{
	m_nativeName = locale.m_nativeName;
	m_nativeLocale = CreateNativeLocale(m_nativeName.c_str());
	m_stdLocale = locale.m_stdLocale;
	return (*this);
}

//------------------------------------------------------------------------------
Locale::~Locale()
{
	Release();
}

//------------------------------------------------------------------------------
void Locale::Release()
{
	FreeNativeLocale(m_nativeLocale);
}

//------------------------------------------------------------------------------
const std::locale& Locale::GetStdLocale() const
{
	return m_stdLocale;
}

//------------------------------------------------------------------------------
NativeLocale_t Locale::GetNativeLocale() const
{
	return m_nativeLocale;
}

//------------------------------------------------------------------------------
const Locale& Locale::GetDefault()
{
	static Locale locale;
	return locale;
}

//------------------------------------------------------------------------------
const Locale& Locale::GetC()
{
	static Locale locale;
	static bool init = false;
	if (!init)
	{
		locale.m_stdLocale = std::locale("C");
#ifdef LN_OS_WIN32
		locale.m_nativeLocale = CreateNativeLocale(L"C");
		locale.m_nativeName = L"C";
#else
		locale.m_nativeLocale = CreateNativeLocale("C");
		locale.m_nativeName = "C";
#endif
		init = true;
	}
	return locale;
}

//==============================================================================
// GenericLocalizer
//==============================================================================
//------------------------------------------------------------------------------
template<typename TChar>
GenericLocalizer<TChar>::GenericLocalizer(const Locale& locale)
	: m_locale(locale)
{
}

//------------------------------------------------------------------------------
template<typename TChar>
int GenericLocalizer<TChar>::Format(TChar* outBuf, int outBufLength, const TChar* format, ...)
{

}

LN_NAMESPACE_END
