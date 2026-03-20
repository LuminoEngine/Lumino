// Copyright (c) 2018+ lriki. Distributed under the MIT license...
#pragma once

#include <stack>
#include "../IO/TextWriter.hpp"
#include "Common.hpp"

namespace ln {

/** SAXスタイルの JSON ジェネレータです。 */
class JsonWriter
{
public:
    JsonWriter(TextWriter* textWriter);
    virtual ~JsonWriter();

    /** 出力する JSON 文字列の整形方法を設定します。 */
    void setFormatting(JsonFormatting formatting);

    /** JSON object の開始トークンを書き込みます。 */
    void writeStartObject();

    /** JSON object の終了トークンを書き込みます。 */
    void writeEndObject();

    /** JSON array の開始トークンを書き込みます。 */
    void writeStartArray();

    /** JSON array の終了トークンを書き込みます。 */
    void writeEndArray();

    /** JSON object のメンバの名前を書き込みます。 */
    void writePropertyName(const StringView& str);

    /** Null 値を書き込みます。 */
    void writeNull();

    /** Bool 値を書き込みます。 */
    void writeBool(bool value);

    /** 整数値を書き込みます。 */
    void writeInt32(int32_t value);

    /** 整数値を書き込みます。 */
    void writeInt64(int64_t value);

    /** 小数値を書き込みます。 */
    void writeFloat(float value);

    /** 小数値を書き込みます。 */
    void writeDouble(double value);

    /** 文字列値を書き込みます。 */
    void writeString(const StringView& str);

protected:
    enum class PrefixType
    {
        Array,
        Object,
        Key,
    };

    bool isComplete() const;
    void onPrefix(PrefixType type, int valueCount);
    void onStartObject();
    void onEndObject();
    void onStartArray();
    void onEndArray();
    void onKey(const Char* str, int length);
    void onNull();
    void onBool(bool value);
    void onInt32(int32_t value);
    void onInt64(int64_t value);
    void onFloat(float value);
    void onDouble(double value);
    void onString(const Char* str, int length);

private:
    void autoComplete(JsonNode token);

    struct Level
    {
        Level(bool inArray)
            : valueCount(0)
            , inArray(inArray)
            , justSawKey(false)
            , justSawContainerEnd(false)
        {
        }

        int valueCount; // number of values contained in current nest level.
        bool inArray;   // true=Array, false=Object
        bool justSawKey;
        bool justSawContainerEnd; // for indent
    };

    JsonFormatting m_formatting;
    TextWriter* m_textWriter;
    std::stack<Level> m_levelStack;
};

} // namespace ln
