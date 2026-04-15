#pragma once

#include <LuminoCore/Object.hpp>
#include <LuminoBase/RefObject.hpp>
#include <cstdint>
#include <mutex>
#include <vector>

namespace ln {

/** ハンドル型。上位16bit が世代番号、下位16bit がスロットインデックス。 */
using LNHandle = uint32_t;

/** C-API で公開するオブジェクトを世代番号付きハンドルで管理するレジストリ。 */
class ObjectRegistry {
    // NOTE: ポインタ vs ハンドル
    // 次の李湯で、ハンドルを採用しています。
    // | 観点 | ポインタ (`void*`) | ハンドル (`int32_t`) |
    // |---|---|---|
    // | スクリプト言語との互換性 | JavaScript の `Number` は 53bit 精度。64bit ポインタは欠落する | 32bit 整数なら問題なし |
    // | セキュリティ | 内部メモリアドレスが外部に漏れる | アドレス空間が隠蔽される |
    // | バリデーション | 不正ポインタ検出が困難 | 範囲チェック＋世代番号で検証可能 |
    // | シリアライズ | ポインタは永続化不可 | ハンドルはセーブ/リプレイ対象にできる |
    // | デバッグ | アドレスは読みにくい | 小さな整数は追いやすい |

public:
    ObjectRegistry();
    ~ObjectRegistry();

    /** オブジェクトを登録しハンドルを返す。レジストリが Ref で所有権を持つ。 */
    LNHandle registerObject(Object* obj);
    LNHandle wrapOrRegisterObject(Object* obj);

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
        Object* object;
        uint16_t generation = 1;
    };

    mutable std::mutex m_mutex;
    std::vector<Slot> m_slots;
    std::vector<uint16_t> m_freeList;
};

} // namespace ln
