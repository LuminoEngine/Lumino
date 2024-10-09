// Copyright (c) 2018+ lriki. Distributed under the MIT license.
#pragma once
#include "Archive.hpp"

#if 0 // prototype

namespace ln {
class Stream;
namespace detail {
class ArchiveStore3;
class BinaryArchiveStore3;
} // namespace detail


class BinaryTextOutputSerializer : public Archive {
public:
    BinaryTextOutputSerializer(Stream* stream);
    virtual ~BinaryTextOutputSerializer();

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


private:
    std::unique_ptr<detail::BinaryArchiveStore3> m_store;
    bool m_processing;
};

class BinaryTextInputSerializer : public Archive {
public:
    BinaryTextInputSerializer(Stream* stream);
    virtual ~BinaryTextInputSerializer();

    template<typename TValue>
    void load(TValue&& value) {

        m_processing = true;
        try {
            Archive::process(std::forward<TValue>(value));
        }
        catch (SerializationException& e) {
            onError(e.message());
        }
        m_processing = false;
    }

private:
    std::unique_ptr<detail::BinaryArchiveStore3> m_store;
    bool m_processing;
};

template<class T = void>
using ArchiveResult = BasicResult<T, String>;

/**
 * オブジェクトとバイナリデータ間のシリアライズ/デシリアライズ行うユーティリティです。
 */
class BinarySerializer {
public:
    template<class TValue>
    static MaybeResult serialize(TValue& value, Stream* outputStream) {
        BinaryTextOutputSerializer ar(outputStream);
        ar.save(value);
        if (ar.hasError()) return LN_MAKE_ERROR(ar.errorMessage());
        return LN_MAKE_SUCCESS();
    }

    ///**
    // * バイナリデータをオブジェクトへデシリアライズします。
    // */
    //template<typename TObject>
    //static ArchiveResult<Ref<TObject>> deserialize(const StringView& jsonText) {
    //    BinaryTextInputSerializer ar;
    //    Ref<TObject> value;
    //    auto r = ar.load(jsonText, value);
    //    if (!r) return r;
    //    if (ar.hasError()) return err(ar.errorMessage());
    //    return ok(value);
    //}

    template<typename TObject>
    static MaybeResult deserialize(Stream* inputStream, TObject* value) {
        BinaryTextInputSerializer ar(inputStream);
        ar.load(*value);
        if (ar.hasError()) return LN_MAKE_ERROR(ar.errorMessage());
        return LN_MAKE_SUCCESS();
    }
};

} // namespace ln

#endif
