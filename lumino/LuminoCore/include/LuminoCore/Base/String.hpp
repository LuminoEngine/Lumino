
#pragma once
#include "Common.hpp"
#include "List.hpp"
#include "Buffer.hpp"
#include "StringHelper.hpp"

namespace ln {
class TextEncoding;
class Locale;
class CharRef;
class StringView;
class Path;
namespace detail {
class UStringCore;
struct StringLockContext;
}

/**
 * Unicode 文字列を表します。
 * 
 * String クラスは環境に依らず、UTF-32 コードで表現される文字の配列です。
 * 
 * 文字列はメモリ上に連続的に確保され Null 終端文字を格納します。
 */
class LN_API String
{
public:
    static const String Empty;

    using value_type = Char;
    size_t size() const { return length(); }

    String();
    ~String();
    String(const String& str);
    String(String&& str) LN_NOEXCEPT;
    String& operator=(const String& str);
    String& operator=(String&& str) LN_NOEXCEPT;

    String(const String& str, int begin);
    String(const String& str, int begin, int length);
    String(const Char* str);
    String(const Char* str, int length);
    String(const Char* begin, const Char* end);
    String(int count, Char ch);
    String(const StringView& str);
    String(const Path& path);

#ifdef LN_STRING_FUZZY_CONVERSION
    String(const char* str);
#ifdef LN_USTRING16
    String(const wchar_t* str);
#endif
#endif

    /** 文字列が空であるかを確認します。 */
    bool isEmpty() const LN_NOEXCEPT;

    /** C 言語としての文字列表現を取得します。 */
    const Char* c_str() const LN_NOEXCEPT;

    /** 文字列の長さを取得します。 */
    int length() const LN_NOEXCEPT;

    /** メモリを再確保せずに格納できる最大の要素数を取得します。 */
    int capacity() const LN_NOEXCEPT;

    /** 文字列をクリアします。 */
    void clear();

    /** 文字列の長さを変更します。 */
    void resize(int newLength);

    /** 文字列の長さを変更します。増大した領域を ch で置き換えます。 */
    void resize(int newLength, Char ch);

    /** サイズ変更の予定を指示します。 */
    void reserve(int size);

    /** 文字列の再代入します。 */
    void assign(const Char* str);
    void assign(const Char* str, int length); /**< @overload assign */
    void assign(int count, Char ch);          /**< @overload assign */
    void assign(const StringView& str);        /**< @overload assign */

    /** 指定された文字列を追加します。 */
    void append(const Char* str, int length);
    void append(const String& str); /**< @overload append */

    /**
     * 指定した文字列がこの文字列内に存在するかを判断します。
     * 
     * @param[in]    str        : 検索文字列
     * @param[in]    cs        : 大文字と小文字の区別設定
     * @return        文字列が存在すれば true。str が空文字列である場合は必ず true となります。
     */
    bool contains(const StringView& str, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;
    bool contains(Char ch, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const; /**< @overload contains */

    /**
     * 文字列を検索し、見つかった最初の文字のインデックスを返します。
     * 
     * @param[in]    str            : 検索文字列
     * @param[in]    startIndex    : 検索を開始するインデックス (省略した場合は先頭から)
     * @param[in]    cs            : 大文字と小文字の区別設定
     * @return        見つからなかった場合は -1。str が空文字列である場合は 0。
     */
    int indexOf(const StringView& str, int startIndex = 0, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;
    int indexOf(Char ch, int startIndex = 0, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;

    /**
     * 文字列を検索し、最後に見つかったインデックスを返します。
     * 
     * @param[in]    str        : 検索文字列
     * @param[in]    startIndex : 検索を開始するインデックス (-1 を指定すると、文字列の末尾から検索を開始する)
     * @param[in]    count      : 検索する文字数 (-1 を指定すると、文字列の先頭まで検索する)
     * @param[in]    cs         : 大文字と小文字の区別設定
     * @return        見つかった文字列の開始インデックス。見つからなかった場合は -1。
     * 
     * startIndex の位置から文字列の先頭に向かう count 文字分の領域から str を検索します。
     * ~~~
     * String str = "abcdef";
     * str.lastIndexOf("de");       // => 3
     * str.lastIndexOf("bc", 2);    // => 1
     * str.lastIndexOf("cd", 2);    // => -1    (検索範囲 "abc" の中に "cd" は存在しない)
     * str.lastIndexOf("cd", 4, 3); // => 2     (検索範囲 "cde" の中に "cd" は存在する)
     * str.lastIndexOf("bc", 4, 3); // => -1    (検索範囲 "cde" の中に "bc" は存在しない)
     * ~~~
     */
    int lastIndexOf(const StringView& str, int startIndex = -1, int count = -1, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;
    int lastIndexOf(Char ch, int startIndex = -1, int count = -1, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const; /**< @overload lastIndexOf */

    /**
     * この文字列の先頭が、指定した文字列と一致するかを判断します。
     * 
     * @param[in]    str            : 検索文字列
     * 
     * str が空文字の場合は必ず true が返ります。
     */
    bool startsWith(const StringView& str, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;
    bool startsWith(Char ch, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const; /**< @overload startsWith */

    /**
     * この文字列の末尾が、指定した文字列と一致するかを判断します。
     * 
     * @param[in]    str            : 検索文字列
     * 
     * str が空文字の場合は必ず true が返ります。
     * ~~~
     * str = "file.txt";
     * if (str.endsWith(".txt")) {
     *     // 一致した
     * }
     * ~~~
     */
    bool endsWith(const StringView& str, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;
    bool endsWith(Char ch, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const; /**< @overload endsWith */

    /**
     * 文字列の部分文字列を抽出します。
     * 
     * @param[in]   start   : 開始文字インデックス
     * @param[in]   count   : 文字数 (-1 の場合、末尾まで抽出する)
     * @return      抽出された文字列
     * 
     * ~~~
     * String s("abcdef");
     * s.substr(2, 3);      // => "cde";
     * ~~~
     */
    StringView substr(int start, int count = -1) const;

    /**
     * 文字列の左側(先頭)から指定した文字数を抽出します。
     * 
     * @param[in]    count    : 文字数
     * @return        抽出された文字列
     * 
     * ~~~
     * String s("abcdef");
     * s.left(2);           // => "ab";
     * ~~~
     */
    StringView left(int count) const;

    /**
     * 文字列の右側(末尾)から指定した文字数を抽出します。
     * 
     * @param[in]    count    : 文字数
     * @return        抽出された文字列
     * 
     * ~~~
     * String s("abcdef");
     * s.right(2);          // => "ef";
     * ~~~
     */
    StringView right(int count) const;

    /** 文字列の先頭と末尾の空白を全て削除した文字列を返します。 */
    String trim() const;

    /** 小文字を大文字に変換した文字列を返します。(ロケールの影響を受けません) */
    String toUpper() const;

    /** 大文字を小文字に変換した文字列を返します。(ロケールの影響を受けません) */
    String toLower() const;

    /** 先頭の文字を大文字、以降を小文字に変換した文字列を返します。(ロケールの影響を受けません) */
    String toTitleCase() const;

    /**
     * この文字列から指定した文字をすべて取り除いた新しい文字列を返します。
     * 
     * @param[in]   str     : 削除する文字列
     * @param[in]   cs      : 大文字と小文字の区別設定
     */
    String remove(const StringView& str, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;
    String remove(Char ch, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const; /**< @overload remove */

    /**
     * 文字列の置換を行います。
     * 
     * @param[in]   from    : 置換される文字列
     * @param[in]   to      : from を置換する文字列
     * @return      置換結果の文字列
     * 
     * from に一致するすべての文字列を to に置換します。
     */
    String replace(const StringView& from, const StringView& to, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;

    /**
     * 指定したインデックス位置に文字列を挿入します。
     *
     * @param[in]   startIndex : インデックス位置
     * @param[in]   value      : 挿入する文字列
     * @return     挿入結果の文字列
     */
    String insert(int startIndex, const StringView& value) const;

    /**
     * 文字列をデリミタで分割します。
     * 
     * @param[in]   delim   : デリミタ文字列
     * @param[in]   option  : 分割方法
     * @return      分割結果の文字列配列
     * 
     * 分割が発生しない場合は文字列全体を持つ要素数1の配列を返します。
     * ~~~
     * auto tokens = String("a,b,c").split(",");        // => ["a", "b", "c"]
     * auto tokens = String("a").split(",");            // => ["a"]
     * auto tokens = String(",").split(",");            // => ["", ""]
     * auto tokens = String("a::b").split("::");        // => ["a", "b"]
     * ~~~
     */
    List<String> split(const StringView& delim, StringSplitOptions option = StringSplitOptions::None) const;

    /**
     * この文字列を整数値に変換します。
     * 
     * @param[in]   base    : 基数 (0、2、8、10、16 のいずれかであること)
     * @return      変換結果の数値
     * 
     * 次の書式に従い、文字列を数値に変換します。
     * 
     * [whitespace] [{+ | – }] [0 [{ x | X }]] [digits | letters]
     * 
     * 16 進数値のアルファベットは大文字と小文字を区別しません。
     * 
     * 基数に 0 を指定すると、文字列の先頭文字から基数を自動判別します。
     * "0x" または "0X" であれば 16 進数、"0" であれば 8 進数、それ以外であれば 10 進数です。
     * 基数に 8 または 16 が指定されている際、文字列の先頭は "0" または "0x" である必要はありません。
     */
    int toInt(int base = 0) const;
    int8_t toInt8(int base = 0) const;
    int16_t toInt16(int base = 0) const;   /**< @copydoc toInt */
    int32_t toInt32(int base = 0) const;   /**< @copydoc toInt */
    int64_t toInt64(int base = 0) const;   /**< @copydoc toInt */
    uint8_t toUInt8(int base = 0) const;   /**< @copydoc toInt */
    uint16_t toUInt16(int base = 0) const; /**< @copydoc toInt */
    uint32_t toUInt32(int base = 0) const; /**< @copydoc toInt */
    uint64_t toUInt64(int base = 0) const; /**< @copydoc toInt */

    /**
     * この文字列を整数値に変換し、成否を返します。
     * 
     * @param[in]   outValue    : 結果を格納する変数のポインタ (nullptr を指定すると成否のみを返す)
     * @param[in]   base        : 基数 (0、2、8、10、16 のいずれかであること)
     * @return      正常に変換された場合は true。それ以外の場合は false。
     * @see         toInt
     */
    bool tryToInt(int* outValue, int base = 0) const;
    bool tryToInt8(int8_t* outValue, int base = 0) const;
    bool tryToInt16(int16_t* outValue, int base = 0) const;   /**< @copydoc tryToInt */
    bool tryToInt32(int32_t* outValue, int base = 0) const;   /**< @copydoc tryToInt */
    bool tryToInt64(int64_t* outValue, int base = 0) const;   /**< @copydoc tryToInt */
    bool tryToUInt8(uint8_t* outValue, int base = 0) const;   /**< @copydoc tryToInt */
    bool tryToUInt16(uint16_t* outValue, int base = 0) const; /**< @copydoc tryToInt */
    bool tryToUInt32(uint32_t* outValue, int base = 0) const; /**< @copydoc tryToInt */
    bool tryToUInt64(uint64_t* outValue, int base = 0) const; /**< @copydoc tryToInt */

    /** ローカルの std::string 型文字列へ変換します。 */
    std::string toStdString(TextEncoding* encoding = nullptr) const;

    /** ローカルの std::wstring 型文字列へ変換します。 */
    std::wstring toStdWString() const;

    /** 指定した文字列を連結します。 */
    static String concat(const StringView& str1, const StringView& str2);
    static String concat(const StringView& str1, const StringView& str2, const StringView& str3);                        /**< @overload concat */
    static String concat(const StringView& str1, const StringView& str2, const StringView& str3, const StringView& str4); /**< @overload concat */

    /** 指定した文字列リストを結合した 1 つの文字列を生成します。各要素の間には、指定した区切り記号が挿入されます。 */
    static String join(const List<String>& list, const StringView& delim);

    /**
     * 指定した2つの文字列を比較します。
     * 
     * @param[in]   str1        : 比較文字列
     * @param[in]   index1      : str1 内の部分文字列の開始位置
     * @param[in]   str2        : 比較文字列
     * @param[in]   index2      : str2 内の部分文字列の開始位置
     * @param[in]   length      : 比較する文字数 (-1 の場合、GetLength() の値を使用します)
     * @param[in]   cs          : 大文字と小文字の区別設定
     * 
     * @return
     *  - str1 が str2 より小さい → 0 より小さい値
     *  - str1 と str2 が等しい   → 0
     *  - str1 が str2 より大きい → 0 より大きい値
     */
    static int compare(const String& str1, const String& str2, CaseSensitivity cs = CaseSensitivity::CaseSensitive);
    static int compare(const StringView& str1, int index1, const StringView& str2, int index2, int length = -1, CaseSensitivity cs = CaseSensitivity::CaseSensitive); /**< @overload compare */

    /** ローカルの char 文字列を String へ変換します。 */
    static String fromCString(const char* str, int length = -1, TextEncoding* encoding = nullptr);

    /** ローカルの wchar_t 文字列を String へ変換します。 */
    static String fromCString(const wchar_t* str, int length = -1);

    /** ローカルの std::string 型文字列を String へ変換します。 */
    static String fromStdString(const std::string_view& str, TextEncoding* encoding = nullptr);

    /** ローカルの std::wstring 型文字列を String へ変換します。 */
    static String fromStdString(const std::wstring& str);

    std::string toUtf8() const;

    static String fromUtf8(const std::string_view& s);

    std::u16string toUtf16() const;

    static String fromUtf16(const std::u16string_view& s);

    std::filesystem::path toStdPath() const { return std::filesystem::path(c_str()); }

    //std::u16string toUtf16() const;

    //static String fromUtf16(const std::u16string& s);

    /**
     * 数値を文字列に変換します。
     * 
     * @param[in]   value       : 数値
     * @param[in]   format      : 書式
     * @param[in]   precision   : 小数の精度 (小数部の桁数)
     * 
     * - 'D' または 'd' : 10進数
     * - 'X' または 'x' : 16進数
     * - 'F' または 'f' : 小数
     * - 'E' または 'e' : 小数 (指数表記)
     */
    static String fromNumber(int32_t value, Char format = 'D');
    static String fromNumber(int64_t value, Char format = 'D');                   /**< @overload fromNumber */
    static String fromNumber(uint32_t value, Char format = 'D');                  /**< @overload fromNumber */
    static String fromNumber(uint64_t value, Char format = 'D');                  /**< @overload fromNumber */
    static String fromNumber(float value, Char format = 'F', int precision = 6);  /**< @overload fromNumber */
    static String fromNumber(double value, Char format = 'F', int precision = 6); /**< @overload fromNumber */

    /** 任意の位置の要素へアクセスします。 */
    CharRef operator[](int index);

    /** 任意の位置の要素へアクセスします。 */
    const Char& operator[](int index) const LN_NOEXCEPT;

    String& operator=(const StringView& rhs);
    String& operator=(const Char* rhs);
    String& operator=(Char ch);
    String& operator=(const Path& rhs);

    String& operator+=(const String& rhs);
    String& operator+=(const StringView& rhs);
    String& operator+=(const Char* rhs);
    String& operator+=(Char rhs);

    explicit operator bool() const LN_NOEXCEPT { return !isEmpty(); }

#ifdef LN_STRING_FUZZY_CONVERSION
    String& operator=(const char* rhs);
    String& operator+=(const char* rhs);
#endif

    /** 現在の環境で定義されている改行文字列を取得します。 */
    static const String& newLine();

    bool equals(const StringView rhs) const noexcept;

private:
    static std::size_t const SSOCapacity = 15;

    // resource management
    void init() LN_NOEXCEPT;
    void release() LN_NOEXCEPT;
    void copy(const String& str);
    void move(String&& str) LN_NOEXCEPT;
    Char* lockBuffer(size_t requestSize, detail::StringLockContext* context) noexcept;
    void unlockBuffer(int confirmedSize, detail::StringLockContext* context) noexcept;
    Char* getBuffer();
    const Char* getBuffer() const LN_NOEXCEPT;

    // sso operation
    void setSSOLength(size_t len);
    size_t getSSOLength() const LN_NOEXCEPT;
    void setSSO();
    void setNonSSO();
    bool isSSO() const LN_NOEXCEPT { return !detail::getLSB<0>(static_cast<uint8_t>(m_data.sso.length)); }
    bool isNonSSO() const LN_NOEXCEPT { return detail::getLSB<0>(static_cast<uint8_t>(m_data.sso.length)); }

    // utils
    void assignFromCStr(const char* str, int length = -1, bool* outUsedDefaultChar = nullptr, TextEncoding* encoding = nullptr);
    void assignFromCStr(const wchar_t* str, int length = -1, bool* outUsedDefaultChar = nullptr, TextEncoding* encoding = nullptr);
    void assignFromCStr(const char16_t* str, int length, bool* outUsedDefaultChar = nullptr);
    void assignFromCStrInternal(const byte_t* bytes, int bytesCount, int elementSize, bool* outUsedDefaultChar, TextEncoding* encoding);
    void setAt(int index, Char ch);
    int getByteCount() const { return length() * sizeof(Char); }
    uint32_t getHashCode() const;

    //static ByteBuffer convertTo(const String& str, const TextEncoding* encoding, bool* outUsedDefaultChar = nullptr);

    union Data
    {
        detail::UStringCore* core;

        struct SSO
        {
            Char buffer[SSOCapacity];
            Char length; // ---xxxxy    : x=size y:flag(0=sso,1=non sso)
        } sso;
    } m_data;

    friend class CharRef;
};

/** String のヘルパーです。 = 演算子による更新を参照元の String に伝えます。 */
class CharRef
{
public:
    operator Char() const
    {
        return m_str.c_str()[m_index];
    }

    CharRef& operator=(Char ch)
    {
        m_str.setAt(m_index, ch);
        return *this;
    }

private:
    CharRef(String& str, int index)
        : m_str(str)
        , m_index(index)
    {
    }

    String& m_str;
    int m_index;

    friend class String;
};

/** ある文字列に対する部分文字列の参照を保持します。 */
class StringView
{
public:
    /** 空の StringView を構築します。 */
    StringView() LN_NOEXCEPT;

    /** 別の StringView と同じ文字列を参照します。(コピーコンストラクタ) */
    StringView(const StringView& str) LN_NOEXCEPT;

    /** 指定された文字列の全体を参照します。 */
    StringView(const String& str);

    /** 文字列と長さを受け取り、その範囲を参照します。 */
    StringView(const String& str, int len);

    /** 文字列と開始インデックスと長さを受け取り、その範囲を参照します。 */
    StringView(const String& str, int startIndex, int len);

    /** 指定された文字列の全体を参照します。 */
    StringView(const Char* str);

    /** 文字列と長さを受け取り、その範囲を参照します。 */
    StringView(const Char* str, int len);

    /** 文字列の範囲を参照します。 */
    StringView(const Char* begin, const Char* end);

    /** 指定された Path 全体を参照します。 */
    StringView(const Path& path);

#ifdef LN_STRING_FUZZY_CONVERSION
    /** 指定された文字列の全体を参照します。 */
    StringRef(const char* str);
#ifdef LN_USTRING16
    /** 指定された文字列の全体を参照します。 */
    StringRef(const wchar_t* str);
#endif
#endif

    /** 文字列の長さを取得します。 */
    LN_CONSTEXPR int length() const LN_NOEXCEPT { return static_cast<int>(m_len); }

    /** 文字列が空かどうかを判定します。 */
    LN_CONSTEXPR bool isEmpty() const LN_NOEXCEPT { return (data() == nullptr || length() <= 0); }

    /** 文字配列を取得します。 */
    LN_CONSTEXPR const Char* data() const LN_NOEXCEPT { return m_str; }

    /** この文字列の末尾が、指定した文字列と一致するかを判断します。 */
    bool endsWith(const StringView& str, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;

    /** 文字列の部分文字列を抽出します。 */
    StringView substr(int start, int count) const;


    /** @copydoc String::indexOf */
    int indexOf(const StringView& str, int startIndex = 0, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;
    int indexOf(Char ch, int startIndex = 0, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;

    /** @copydoc String::lastIndexOf */
    int lastIndexOf(const StringView& str, int startIndex = -1, int count = -1, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;
    int lastIndexOf(Char ch, int startIndex = -1, int count = -1, CaseSensitivity cs = CaseSensitivity::CaseSensitive) const;

    int toInt(int base = 0) const;

    /** ローカルの std::string 型文字列へ変換します。 */
    std::string toStdString() const;

    /** ローカルの std::wstring 型文字列へ変換します。 */
    std::wstring toStdWString() const;

    std::string toUtf8() const;

    /** 任意の位置の要素にアクセスします。 */
    LN_CONSTEXPR const Char& operator[](int index) const { return *(data() + index); }

    ~StringView() { clear(); }
    StringView& operator=(const StringView& str);
    StringView& operator=(StringView&& str);

private:
    size_t getHashCode() const;
    void clear();

    const String* m_string;
    const Char* m_str;
    size_t m_len;
    bool m_localAlloc;
};

//namespace fmt {
//
//template<typename TChar>
//class GenericFormatStringRef
//{
//public:
//    GenericFormatStringRef();
//    GenericFormatStringRef(const TChar* begin, const TChar* end);
//    GenericFormatStringRef(const GenericFormatStringRef& str);
//
//    bool isEmpty() const { return m_length == 0; }
//    size_t length() const { return m_length; }
//    const TChar* begin() const { return m_str; }
//    const TChar* end() const { return m_str + m_length; }
//
//    const TChar& operator[](int index) const { return *(m_str + index); }
//
//private:
//    const TChar* m_str;
//    size_t m_length;
//};
//
//template<typename TChar>
//class GenericFormatStringBuilder
//{
//public:
//    GenericFormatStringBuilder();
//    GenericFormatStringBuilder(TChar* buffer, size_t bufferSize);
//
//    void clear();
//    void appendChar(TChar ch);
//    void appendChar(TChar ch, int count);
//    void appendString(const TChar* str);
//    void appendString(const TChar* str, size_t length);
//    void appendString(const String& str);
//    const TChar* c_str() const;
//    size_t length() const;
//    bool isFixedBufferOver() const { return m_fixedBufferOver; }
//
//private:
//    void appendIntenal(const TChar* str, size_t length);
//
//    ByteBuffer m_buffer;
//    size_t m_bufferUsed;
//    byte_t* m_fixedBuffer;
//    size_t m_fixedBufferSize;
//    bool m_fixedBufferOver;
//};
//
//template<typename TChar>
//class GenericStringFormatter
//{
//public:
//    GenericStringFormatter()
//        : m_error()
//        , m_errorPos(0)
//    {
//    }
//    ~GenericStringFormatter() {}
//
//    void reportError(const char* message, int pos)
//    {
//        m_error = message;
//        m_errorPos = pos;
//    }
//    bool hasError() const { return !m_error.empty(); }
//
//    void setLocale(const std::locale* locale) { m_locale = locale; }
//    const std::locale* locale() const { return m_locale; }
//
//    void setFormatString(const GenericFormatStringRef<TChar>& str) { m_formatString = str; }
//    const GenericFormatStringRef<TChar>& formatString() const { return m_formatString; }
//
//    void setPrecision(const GenericFormatStringRef<TChar>& str) { m_precision = str; }
//    const GenericFormatStringRef<TChar>& precision() const { return m_precision; }
//
//    GenericFormatStringBuilder<TChar>& getSB() { return m_sb; }
//
//private:
//    const std::locale* m_locale;
//    GenericFormatStringBuilder<TChar> m_sb;
//    GenericFormatStringRef<TChar> m_formatString;
//    GenericFormatStringRef<TChar> m_precision;
//
//    std::string m_error;
//    int m_errorPos;
//};
//
////template<typename T, typename Formatter>
////void formatArg(Formatter&, ...)
////{
////    assert(0);
////    //static_assert(false, "[Lumino format string error] Cannot format argument. Please overload formatArg.");
////}
//
//} // namespace fmt

namespace detail {

class UStringCore
{
public:
    UStringCore();
    ~UStringCore();
    bool isShared() const LN_NOEXCEPT;
    void retain();
    void release();

    Char* get() LN_NOEXCEPT { return m_str; }
    const Char* get() const LN_NOEXCEPT { return m_str; }
    int length() const LN_NOEXCEPT { return m_length; }
    int capacity() const { return m_capacity; }
    void reserve(int length);
    void fixLength(int length);
    void resize(int length);
    void clear();

private:
    std::atomic<int> m_refCount;
    Char* m_str;
    int m_capacity;
    int m_length;
};

} // namespace detail

//==============================================================================
// String

inline const Char* String::c_str() const LN_NOEXCEPT
{
    return (isSSO()) ? m_data.sso.buffer : ((m_data.core) ? m_data.core->get() : _TT(""));
}

inline int String::length() const LN_NOEXCEPT
{
    return static_cast<int>((isSSO()) ? getSSOLength() : ((m_data.core) ? m_data.core->length() : 0));
}

inline int String::capacity() const LN_NOEXCEPT
{
    return static_cast<int>((isSSO()) ? SSOCapacity : ((m_data.core) ? m_data.core->capacity() : 0));
}

inline CharRef String::operator[](int index)
{
	LN_CHECK(0 <= index && index < length());
    return CharRef(*this, index);
}

inline const Char& String::operator[](int index) const LN_NOEXCEPT
{
	LN_CHECK(0 <= index && index < length());
    return getBuffer()[index];
}

inline String& String::operator=(const StringView& rhs)
{
    assign(rhs);
    return *this;
}
inline String& String::operator=(const Char* rhs)
{
    assign(rhs);
    return *this;
}
inline String& String::operator=(Char ch)
{
    assign(&ch, 1);
    return *this;
}

inline String& String::operator+=(const String& rhs)
{
    append(rhs.c_str(), rhs.length());
    return *this;
}
inline String& String::operator+=(const StringView& rhs)
{
    append(rhs.data(), rhs.length());
    return *this;
}
inline String& String::operator+=(const Char* rhs)
{
    append(rhs, static_cast<int>(detail::UStringHelper::strlen(rhs)));
    return *this;
}
inline String& String::operator+=(Char rhs)
{
    append(&rhs, 1);
    return *this;
}

inline String operator+(const String& lhs, const String& rhs)
{
    return String::concat(lhs, rhs);
}
inline String operator+(const String& lhs, const Char* rhs)
{
    return String::concat(lhs, StringView(rhs));
}
inline String operator+(const Char* lhs, const String& rhs)
{
    return String::concat(StringView(lhs), rhs);
}
inline String operator+(const String& lhs, Char rhs)
{
    return String::concat(lhs, StringView(&rhs, 1));
}
inline String operator+(Char lhs, const String& rhs)
{
    return String::concat(StringView(&lhs, 1), rhs);
}

inline bool operator==(const Char* lhs, const String& rhs)
{
    return detail::UStringHelper::compare(lhs, rhs.c_str()) == 0;
}
inline bool operator==(const String& lhs, const String& rhs)
{
    return String::compare(lhs, 0, rhs, 0) == 0;
}
inline bool operator==(const String& lhs, const Char* rhs)
{
    return detail::UStringHelper::compare(lhs.c_str(), rhs) == 0;
}
inline bool operator==(const String& lhs, const StringView& rhs)
{
    return String::compare(lhs, 0, rhs, 0) == 0;
}
inline bool operator!=(const Char* lhs, const String& rhs)
{
    return !operator==(lhs, rhs);
}
inline bool operator!=(const String& lhs, const String& rhs)
{
    return !operator==(lhs, rhs);
}
inline bool operator!=(const String& lhs, const Char* rhs)
{
    return !operator==(lhs, rhs);
}

inline bool operator<(const String& lhs, const String& rhs)
{
    return String::compare(lhs, 0, rhs, 0, LN_MAX(lhs.length(), rhs.length()), CaseSensitivity::CaseSensitive) < 0;
}
inline bool operator<(const Char* lhs, const String& rhs)
{
    return String::compare(lhs, 0, rhs, 0, -1, CaseSensitivity::CaseSensitive) < 0;
}
inline bool operator<(const String& lhs, const Char* rhs)
{
    return String::compare(lhs, 0, rhs, 0, -1, CaseSensitivity::CaseSensitive) < 0;
}
inline bool operator>(const String& lhs, const String& rhs)
{
    return String::compare(lhs, 0, rhs, 0, LN_MAX(lhs.length(), rhs.length()), CaseSensitivity::CaseSensitive) > 0;
}
inline bool operator>(const Char* lhs, const String& rhs)
{
    return String::compare(lhs, 0, rhs, 0, -1, CaseSensitivity::CaseSensitive) > 0;
}
inline bool operator>(const String& lhs, const Char* rhs)
{
    return String::compare(lhs, 0, rhs, 0, -1, CaseSensitivity::CaseSensitive) > 0;
}

inline bool operator<=(const String& lhs, const String& rhs)
{
    return !operator>(lhs, rhs);
}
inline bool operator<=(const Char* lhs, const String& rhs)
{
    return !operator>(lhs, rhs);
}
inline bool operator<=(const String& lhs, const Char* rhs)
{
    return !operator>(lhs, rhs);
}
inline bool operator>=(const String& lhs, const String& rhs)
{
    return !operator<(lhs, rhs);
}
inline bool operator>=(const Char* lhs, const String& rhs)
{
    return !operator<(lhs, rhs);
}
inline bool operator>=(const String& lhs, const Char* rhs)
{
    return !operator<(lhs, rhs);
}

#ifdef LN_STRING_FUZZY_CONVERSION
inline String& String::operator=(const char* rhs)
{
    assignFromCStr(rhs);
    return *this;
}
inline String& String::operator+=(const char* rhs)
{
    String s(rhs);
    append(s.c_str(), s.length());
    return *this;
}

inline String operator+(const char* lhs, const String& rhs)
{
    return operator+(String(lhs), rhs);
}

inline bool operator==(const char* lhs, const String& rhs)
{
    return operator==(String(lhs), rhs);
}
inline bool operator==(const String& lhs, const char* rhs)
{
    return operator==(lhs, String(rhs));
}
inline bool operator!=(const char* lhs, const String& rhs)
{
    return operator!=(String(lhs), rhs);
}
inline bool operator!=(const String& lhs, const char* rhs)
{
    return operator!=(lhs, String(rhs));
}

inline bool operator<(const char* lhs, const String& rhs)
{
    return operator<(String(lhs), rhs);
}
inline bool operator<(const String& lhs, const char* rhs)
{
    return operator<(lhs, String(rhs));
}
inline bool operator>(const char* lhs, const String& rhs)
{
    return operator>(String(lhs), rhs);
}
inline bool operator>(const String& lhs, const char* rhs)
{
    return operator>(lhs, String(rhs));
}

inline bool operator<=(const char* lhs, const String& rhs)
{
    return operator<=(String(lhs), rhs);
}
inline bool operator<=(const String& lhs, const char* rhs)
{
    return operator<=(lhs, String(rhs));
}
inline bool operator>=(const char* lhs, const String& rhs)
{
    return operator>=(String(lhs), rhs);
}
inline bool operator>=(const String& lhs, const char* rhs)
{
    return operator>=(lhs, String(rhs));
}
#endif

inline std::ostream& operator<<(std::ostream& os, const String& str)
{
    os << str.toStdString();
    return os;
}
inline std::wostream& operator<<(std::wostream& os, const String& str)
{
    os << str.toStdWString();
    return os;
}

//==============================================================================
// StringView

inline String operator+(const Char* lhs, const StringView& rhs)
{
    return String::concat(lhs, rhs);
}

inline String operator+(const StringView& lhs, const StringView& rhs)
{
    return String::concat(lhs, rhs);
}

inline bool operator==(const StringView& lhs, const StringView& rhs)
{
    return String::compare(lhs, 0, rhs, 0, LN_MAX(lhs.length(), rhs.length())) == 0;
}
inline bool operator==(const Char* lhs, const StringView& rhs)
{
    return String::compare(StringView(lhs), 0, rhs, 0, -1) == 0;
}
inline bool operator==(const StringView& lhs, const Char* rhs)
{
    return String::compare(lhs, 0, StringView(rhs), 0, -1) == 0;
}
inline bool operator!=(const StringView& lhs, const StringView& rhs)
{
    return !operator==(lhs, rhs);
}
inline bool operator!=(const Char* lhs, const StringView& rhs)
{
    return !operator==(lhs, rhs);
}
inline bool operator!=(const StringView& lhs, const Char* rhs)
{
    return !operator==(lhs, rhs);
}

} // namespace ln

// for unordered_map key
namespace std {
template<>
struct hash<ln::String>
{
    std::size_t operator()(const ln::String& key) const;
};
} // namespace std

//#include "StringFormat.inl"


namespace fmt {

//template<typename TChar> struct formatter<::ln::String, TChar> {
//
//    template<typename ParseContext>
//    auto parse(ParseContext& ctx) ->  decltype(ctx.begin()) {
//        return ctx.begin();
//    }
//
//    template<typename FormatContext>
//    auto format(const ln::String& v, FormatContext& ctx) -> decltype(ctx.out()) {
//        std::u16string_view view(v.c_str(), v.length());
//        formatter<std::u16string_view, char16_t> fff;
//        return fff.format(view, ctx);
//
//        ///std::u16string ss = v.c_str();
//        //return fmt::formatter<std::string>::format(ss, ctx);
//        //std::basic_string_view<::ln::Char> view(v.c_str(), v.length());
//        //return formatter<std::basic_string_view<::ln::Char>>::format(view, ctx);
//    }
//};

template<> struct formatter<::ln::String, ln::Char> {

    template<typename ParseContext>
    auto parse(ParseContext& ctx) ->  decltype(ctx.begin()) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const ln::String& v, FormatContext& ctx) -> decltype(ctx.out()) {
        //std::u16string ss = v.c_str();
        //return fmt::formatter<std::string>::format(ss, ctx);
        //std::basic_string_view<::ln::Char> view(v.c_str(), v.length());
       // return formatter<std::basic_string_view<::ln::Char>>::format(view, ctx);
        
        std::basic_string_view<ln::Char> view(v.c_str(), v.length());
        formatter<std::basic_string_view<ln::Char>, ln::Char> fff;
        return fff.format(view, ctx);
    }
};

template<> struct formatter<::ln::StringView, ln::Char> {
    template<typename ParseContext>
    auto parse(ParseContext& ctx) ->  decltype(ctx.begin()) {
        return ctx.begin();
    }

    template<typename FormatContext>
    auto format(const ln::StringView& v, FormatContext& ctx) -> decltype(ctx.out()) {
        std::basic_string_view<ln::Char> view(v.data(), v.length());
        formatter<std::basic_string_view<ln::Char>, ln::Char> fff;
        return fff.format(view, ctx);
    }
};
} // namespace fmt


//==============================================================================
// String

namespace ln {

inline bool String::equals(const StringView rhs) const noexcept {
    return (length() == rhs.length() && startsWith(rhs));
}

inline std::u32string_view toStdStringView(const String& v) {
    return std::u32string_view(v.c_str(), v.length());
}

inline std::u32string_view toStdStringView(const StringView& v) {
    return std::u32string_view(v.data(), v.length());
}

} // namespace ln

#include "StringFormat.hpp"
