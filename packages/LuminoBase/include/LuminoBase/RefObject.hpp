#pragma once

#include <atomic>
#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <utility>

namespace ln {

/** 侵入型参照カウントの基底クラス。 */
class RefObject {
public:
    RefObject() = default;
    virtual ~RefObject() = default;

    void addRef();

    void release();

    uint32_t refCount() const { return m_refCount.load(std::memory_order_relaxed); }

protected:
    /**
     * 参照がなくなり、オブジェクトが削除されようとしているときに呼び出されます。
     * デストラクタと違って仮想関数を呼び出せるため、デストラクタの制限を回避したいときに使用します。
     */
    virtual void finalize();

private:
    RefObject(const RefObject&) = delete;
    RefObject& operator=(const RefObject&) = delete;
    std::atomic<uint32_t> m_refCount{1};
};

/**
 * RefObject 用のスマートポインタ。
 * 生ポインタからは暗黙に構築できないため、Ref::adopt() または Ref::retain() を使用してください。
 * adopt() は既存の参照の所有権をそのまま引き継ぎ、retain() は参照カウントを 1 つ増やします。
 */
template <typename T>
class Ref {
public:
    Ref() = default;
    Ref(std::nullptr_t) {}
    ~Ref() { reset(); }

    // Copy
    Ref(const Ref& o) : m_ptr(o.m_ptr) {
        if (m_ptr) {
            m_ptr->addRef();
        }
    }
    Ref& operator=(const Ref& o) {
        T* p = o.m_ptr;
        if (p) {
            p->addRef();
        }
        reset();
        m_ptr = p;
        return *this;
    }

    // Move
    Ref(Ref&& o) noexcept : m_ptr(o.m_ptr) { o.m_ptr = nullptr; }
    Ref& operator=(Ref&& o) noexcept {
        if (this != &o) {
            T* p = o.m_ptr;
            o.m_ptr = nullptr;
            reset();
            m_ptr = p;
        }
        return *this;
    }

    // Upcasting
    template <typename U, typename = std::enable_if_t<std::is_base_of_v<T, U>>>
    Ref(const Ref<U>& o) : m_ptr(o.get()) {
        if (m_ptr) {
            m_ptr->addRef();
        }
    }

    /** 生ポインタを引き受ける (既存の参照の所有権を取得し、参照カウントは増やさない)。 */
    static Ref adopt(T* p) {
        Ref r;
        r.m_ptr = p;
        return r;
    }

    /** 生ポインタを引き受ける (参照カウントもインクリメントする)。 */
    static Ref retain(T* p) {
        if (p) {
            p->addRef();
        }
        Ref r;
        r.m_ptr = p;
        return r;
    }


    void reset() {
        if (m_ptr) {
            m_ptr->release();
            m_ptr = nullptr;
        }
    }

    void detach() { m_ptr = nullptr; }

    T* get() const { return m_ptr; }
    T* operator->() const { return m_ptr; }
    explicit operator bool() const { return m_ptr != nullptr; }

    bool operator==(const Ref& o) const { return m_ptr == o.m_ptr; }
    bool operator!=(const Ref& o) const { return m_ptr != o.m_ptr; }

private:
    T* m_ptr = nullptr;
};

} // namespace ln
