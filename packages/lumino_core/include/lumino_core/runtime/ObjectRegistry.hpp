#pragma once

#include <lumino_core/Object.hpp>
#include <lumino_base/RefCounted.hpp>
#include <cstdint>
#include <mutex>
#include <vector>

namespace ln {

/** ハンドル型。上位16bit が世代番号、下位16bit がスロットインデックス。 */
using LNHandle = uint32_t;
constexpr LNHandle LN_NULL_HANDLE = 0;

/** C-API で公開するオブジェクトを世代番号付きハンドルで管理するレジストリ。 */
class ObjectRegistry {
public:
    ObjectRegistry();
    ~ObjectRegistry();

    /** オブジェクトを登録しハンドルを返す。レジストリが Ref で所有権を持つ。 */
    LNHandle registerObject(Ref<Object> obj);

    /** ハンドルからオブジェクトポインタを解決する。無効なハンドルは nullptr を返す。 */
    Object* resolve(LNHandle handle) const;

    /** テンプレート版 resolve。型キャストを行う。 */
    template <typename T>
    T* resolve(LNHandle handle) const {
        return static_cast<T*>(resolve(handle));
    }

    /**
     * ハンドルに対応するオブジェクトの所有権を解放する。
     * 他に参照がなければオブジェクトは破棄される。
     * 成功時 true、無効ハンドル時 false を返す。
     */
    bool release(LNHandle handle);

    /** オブジェクトのデストラクタから呼ばれる内部用。スロットをクリーンアップする。 */
    void unregister(Object* obj);

    /** 現在登録されているオブジェクト数。 */
    size_t size() const;

    /** ハンドルユーティリティ */
    static LNHandle makeHandle(uint16_t index, uint16_t generation) {
        return (static_cast<uint32_t>(generation) << 16) | static_cast<uint32_t>(index);
    }
    static uint16_t handleIndex(LNHandle handle) {
        return static_cast<uint16_t>(handle & 0xFFFF);
    }
    static uint16_t handleGeneration(LNHandle handle) {
        return static_cast<uint16_t>(handle >> 16);
    }

private:
    struct Slot {
        Ref<Object> object;
        uint16_t generation = 1;
    };

    mutable std::mutex mutex_;
    std::vector<Slot> slots_;
    std::vector<uint16_t> freeList_;
};

} // namespace ln
