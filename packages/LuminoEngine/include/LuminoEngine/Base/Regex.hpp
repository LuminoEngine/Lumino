
#pragma once

namespace ln {

class MatchResult;

/**
 * 正規表現を用いたパターンマッチングを提供します。
 * 
 * 検証、検索、文字列分割 をサポートします。
 * 
 * 正規表現の実装は PCRE（Perl互換の正規表現のリファレンス実装）です。
 * 詳細については以下を参照してください。
 * - http://pcre.org/pcre.txt
 * - http://perldoc.perl.org/perlre.html
 */
class Regex
{
public:
    Regex(const StringView& pattern);
    ~Regex();
    
    bool match(const StringView& input, MatchResult* outResult = nullptr) const;
    
    bool search(const StringView& input, MatchResult* outResult = nullptr) const;

    /** 
        @brief    対象の文字列が、正規表現パターンで表現できているかを調べます。（完全一致）
    */
    static bool match(const StringView& input, const StringView& pattern, MatchResult* outResult = nullptr);

    /**
        @brief    文字列の中から、正規表現パターンに該当する文字列があるかを調べます。（検索, 部分一致）
    */
    static bool search(const StringView& input, const StringView& pattern, MatchResult* outResult = nullptr);

private:
    //std::basic_regex<Char>    m_regex;
    void* m_pcre;
};

/**
 * 正規表現検索に一致した結果を表します。 
 */
class MatchResult
{
public:
    MatchResult();
    ~MatchResult();

    /** マッチ範囲の先頭インデックスを返します。*/
    int position() const;    // TODO: position

    /** マッチ範囲の文字数を返します。*/
    int length() const;

    /** マッチ範囲全体の文字列を返します。*/
    StringView value() const;
    
    int groupCount() const;
    
    // index=0 はマッチした全体を返す
    StringView groupValue(int index) const;
    
    //StringView operator[](int index) const
    //{
    //    return group(index);
    //}

private:
    friend class Regex;
    const Char* m_subject;
    void* m_matchData;

    //typedef std::match_results<const Char*>    std_match_results;

    //std_match_results    m_matchResults;
};



////==============================================================================
//// Regex
////==============================================================================
//
//inline Regex::Regex(const StringView& pattern)
//    : m_regex(pattern.getBegin(), pattern.getLength(), std::basic_regex<Char>::ECMAScript)
//{
//}
//
//inline bool Regex::match(const StringView& input, MatchResult* outResult) const
//{
//    if (outResult != nullptr)
//    {
//        return std::regex_match(input.getBegin(), input.getEnd(), outResult->m_matchResults, m_regex, std::regex_constants::match_default);
//    }
//    else
//    {
//        return std::regex_match(input.getBegin(), input.getEnd(), m_regex, std::regex_constants::match_default);
//    }
//}
//
//inline bool Regex::search(const StringView& input, MatchResult* outResult) const
//{
//    if (outResult != nullptr)
//    {
//        return std::regex_search(input.getBegin(), input.getEnd(), outResult->m_matchResults, m_regex, std::regex_constants::match_default);
//    }
//    else
//    {
//        return std::regex_search(input.getBegin(), input.getEnd(), m_regex, std::regex_constants::match_default);
//    }
//}
//
//inline bool Regex::match(const StringView& input, const StringView& pattern, MatchResult* outResult)
//{
//    Regex re(pattern);
//    return re.search(input, outResult);
//}
//
//inline bool Regex::search(const StringView& input, const StringView& pattern, MatchResult* outResult)
//{
//    Regex re(pattern);
//    return re.search(input, outResult);
//}
//
} // namespace ln
