// Copyright (c) 2018+ lriki. Distributed under the MIT license.
#pragma once
#include <stack>
#include "../IO/TextReader.hpp"
#include "Common.hpp"
#include "JsonDiagnostics.hpp"

namespace ln {
namespace detail {
class PositioningTextReader;
}

/** SAXスタイルの JSON リーダーです。 */
class JsonReader
{
public:
    /** 読み取る文字列を指定して構築します。 */
    JsonReader(const String& text);

    /** 文字列を読み取るための TextReader を指定して構築します。 */
    JsonReader(TextReader* textReader);

    ~JsonReader();

    /**
	 * 次のノードを読み取ります。　
	 *
	 * 正常に読み取られた場合は true を返します。続いて value() などで値を取得できます。
	 * それ以上読み取るノードが存在しない場合は false を返します。
	 */
    bool read();

    /** 現在のノードの種類を取得します。 */
    JsonNode nodeType() const;

    /** 現在のノードの値 (文字列形式) を取得します。 値が無い場合は空文字列を返します。 */
    const String& value() const;

    /** 現在のプロパティ (Object のメンバ) の名前を取得します。 */
    const String& propertyName() const;

    /** 現在のノードが Bool である場合、パース結果の bool 値を取得できます。 */
    bool boolValue() const;

    /** 現在のノードが Int32 である場合、パース結果の int32_t 値を取得できます。 */
    int32_t int32Value() const;

    /** 現在のノードが Int64 である場合、パース結果の int64_t 値を取得できます。 */
    int64_t int64Value() const;

    /** 現在のノードが Float である場合、パース結果の float 値を取得できます。 */
    float floatValue() const;

    /** 現在のノードが Double である場合、パース結果の double 値を取得できます。 */
    double doubleValue() const;

    /** JSON 解析の診断情報を取得します。 */
    const JsonDiag& diag() const;

private:
    enum class ResultState
    {
        Success = 0,
        Error = 1,
    };

    enum class State
    {
        Start,
        ObjectStart, // { の次の状態。次は } でも良い。
        Object,      // , の次の状態。次は } ではダメ。
        ArrayStart,
        Array,
        Property,
        PostValue,
    };

    enum class ContainerType
    {
        None,
        Object,
        Array,
    };

    struct ParserState
    {
        State state = State::Start;
        String propertyName;
        ContainerType containerType = ContainerType::None; // 現在解析中のブロックの種類
    };

    union
    {
        bool m_bool;
        int32_t m_int32;
        int64_t m_int64;
        float m_float;
        double m_double;

    } m_valueData;

    Ref<detail::PositioningTextReader> m_reader;
    ParserState m_currentState;
    JsonNode m_currentNode;
    String m_value;
    List<Char> m_textCache;
    std::stack<ParserState> m_stateStack;
    JsonDiag m_diag;

    JsonReader();
    bool skipWhitespace();
    bool parseValue();
    bool parseNull();
    bool parseTrue();
    bool parseFalse();
    bool parseNumber();
    bool parseObject();
    bool tryParsePropertyName();
    bool parseString(bool isKey);
    bool ParsePostValue(bool* outSkip);

    bool setNode(JsonNode node, const Char* value = nullptr, int valueLen = 0);
    void pushState();
    void popState();
    void setError(JsonDiagCode code, const String& message = String::Empty);
};

namespace detail {

class PositioningTextReader
    : public TextReader
{
public:
    PositioningTextReader(TextReader* innter);
    virtual int peek() override;
    virtual int read() override;
    virtual bool readLine(String* line) override;
    virtual String readToEnd() override;
    virtual bool isEOF() override;
    int getPosition() const;
    int getLineNumber() const;
    int getColumnNumber() const;

private:
    void advancePosition(Char ch);

    Ref<TextReader> m_innter;
    int m_pos;
    int m_line;
    int m_column;
    bool m_lastCR;
};

} // namespace detail
} // namespace ln
