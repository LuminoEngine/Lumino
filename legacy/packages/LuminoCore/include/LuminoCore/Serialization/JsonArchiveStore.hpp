// Copyright (c) 2018+ lriki. Distributed under the MIT license.
#pragma once
#include "Archive.hpp"

namespace ln {
namespace detail {
class ArchiveStore3;
class JsonArchiveStore3;
} // namespace detail

class JsonTextOutputSerializer : public Archive {
public:
    JsonTextOutputSerializer();
    virtual ~JsonTextOutputSerializer();

    template<typename TValue>
    void save(TValue&& value) {
        if (LN_REQUIRE(!m_processing)) return;
        m_processing = true;
        try {
            Archive::process(std::forward<TValue>(value));
        }
        catch (SerializationException& e) {
            onError(e.message());
        }
        m_processing = false;
    }

    String toString(JsonFormatting formatting = JsonFormatting::Indented);

private:
    std::unique_ptr<detail::JsonArchiveStore3> m_store;
    bool m_processing;
};

class JsonTextInputSerializer : public Archive {
public:
    JsonTextInputSerializer();
    virtual ~JsonTextInputSerializer();

    template<typename TValue>
    Result_deprecated<> load(const String& jsonText, TValue&& value) {
        if (LN_REQUIRE(!m_processing)) return err();
        auto r = setup(jsonText);
        if (!r) return r;

        m_processing = true;
        try {
            Archive::process(std::forward<TValue>(value));
        }
        catch (SerializationException& e) {
            onError(e.message());
        }
        m_processing = false;

        return ok();
    }

private:
    Result_deprecated<> setup(const String& jsonText);

    std::unique_ptr<detail::JsonArchiveStore3> m_store;
    bool m_processing;
};

template<class T = void>
using ArchiveResult = BasicResult_deprecated<T, String>;

/**
 * オブジェクトと JSON 文字列間のシリアライズ/デシリアライズ行うユーティリティです。
 */
class JsonSerializer {
public:
    /**
     * オブジェクトを JSON 文字列へシリアライズします。
     * @param[in]     value         : データが格納されたオブジェクトへの参照
     * @param[in]     formatting    : JSON 文字列の整形方法
     * @return        JSON 文字列
     */
    template<class TObject>
    static ArchiveResult<String> serialize(Ref<TObject>& value, JsonFormatting formatting = JsonFormatting::Indented) {
        if (LN_ASSERT(value)) return err();
        JsonTextOutputSerializer ar;
        ar.save(value);
        String str = ar.toString(formatting);
        if (ar.hasError()) return err(ar.errorMessage());
        return ok(str);
    }

    template<class TValue>
    static ArchiveResult<String> serialize(TValue& value, JsonFormatting formatting = JsonFormatting::Indented) {
        JsonTextOutputSerializer ar;
        ar.save(value);
        String str = ar.toString(formatting);
        if (ar.hasError()) return err(ar.errorMessage());
        return ok(str);
    }

    /**
     * JSON 文字列をオブジェクトへデシリアライズします。
     * @param[in]     jsonText    : JSON 文字列
     * @param[in]     value         : データを格納するオブジェクトへの参照
     */
    template<typename TObject>
    static ArchiveResult<Ref<TObject>> deserialize(const StringView& jsonText) {
        JsonTextInputSerializer ar;
        Ref<TObject> value;
        auto r = ar.load(jsonText, value);
        if (!r) return r;
        if (ar.hasError()) return err(ar.errorMessage());
        return ok(value);
    }

    template<typename TObject>
    static ArchiveResult<> deserialize(const StringView& jsonText, TObject* value) {
        JsonTextInputSerializer ar;
        auto r = ar.load(jsonText, *value);
        if (!r) return r;
        if (ar.hasError()) return err(ar.errorMessage());
        return ok();
    }
};

} // namespace ln
