// Copyright (c) 2018 lriki. Distributed under the MIT license.
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

    void setFormatting(JsonFormatting formatting);
    void writeStartObject();
    void writeEndObject();
    void writeStartArray();
    void writeEndArray();
    void writePropertyName(const Char* str, int length = -1);
    void writeNull();
    void writeBool(bool value);
    void writeInt32(int32_t value);
    void writeInt64(int64_t value);
    void writeFloat(float value);
    void writeDouble(double value);
    void writeString(const StringRef& str);
    bool isComplete() const;

protected:
    enum class PrefixType
    {
        Array,
        Object,
        Key,
    };

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
    void autoComplete(JsonToken token);

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
