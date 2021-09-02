// Copyright (c) 2018+ lriki. Distributed under the MIT license.
#pragma once
#include <locale>

namespace ln {

#if defined(_MSC_VER)
typedef _locale_t NativeLocale_t;
#else
typedef locale_t NativeLocale_t;
#endif

/** ロケールを表します。 */
class Locale
{
public:
    /** デフォルトのロケールを使用してオブジェクトを構築します。 */
    Locale();

    /** ロケール名を指定してオブジェクトを構築します。 */
    explicit Locale(const Char* name);

    Locale(const Locale& locale);
    Locale& operator=(const Locale& locale);
    ~Locale();

    /** 現在のデフォルトのロケールを取得します。 */
    static const Locale& getDefault();

    /** "C" ロケールを取得します。 */
    static const Locale& getC();

    const std::locale& stdLocale() const;
    NativeLocale_t nativeLocale() const;

private:
    void release();

    std::locale m_stdLocale;
    NativeLocale_t m_nativeLocale;
#ifdef LN_OS_WIN32
    std::wstring m_nativeName;
#else
    std::string m_nativeName;
#endif
};

} // namespace ln
