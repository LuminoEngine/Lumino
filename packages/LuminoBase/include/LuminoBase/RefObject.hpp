#pragma once

#include <atomic>
#include <cstdint>
#include <utility>

namespace ln {

/** 侵入型参照カウントの基底クラス。 */
class RefObject {
public:
    RefObject() = default;
    virtual ~RefObject() = default;

    void addRef();

    void release();

    uint32_t refCount() const { return refCount_.load(std::memory_order_relaxed); }

protected:
    /**
     * 参照がなくなり、オブジェクトが削除されようとしているときに呼び出されます。
     * デストラクタと違って仮想関数を呼び出せるため、デストラクタの制限を回避したいときに使用します。
     */
    virtual void finalize();

private:
    RefObject(const RefObject&) = delete;
    RefObject& operator=(const RefObject&) = delete;
    std::atomic<uint32_t> refCount_{1};
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
    Ref(const Ref& o) : ptr_(o.ptr_) {
        if (ptr_) ptr_->addRef();
    }
    Ref& operator=(const Ref& o) {
        if (this != &o) {
            reset();
            ptr_ = o.ptr_;
            if (ptr_) ptr_->addRef();
        }
        return *this;
    }

    // Move
    Ref(Ref&& o) noexcept : ptr_(o.ptr_) { o.ptr_ = nullptr; }
    Ref& operator=(Ref&& o) noexcept {
        if (this != &o) {
            reset();
            ptr_ = o.ptr_;
            o.ptr_ = nullptr;
        }
        return *this;
    }

    // Upcasting
    template <typename U, typename = std::enable_if_t<std::is_base_of_v<T, U>>>
    Ref(const Ref<U>& o) : ptr_(o.get()) {
        if (ptr_) ptr_->addRef();
    }

    /** 生ポインタを引き受ける (既存の参照の所有権を取得し、参照カウントは増やさない)。 */
    static Ref adopt(T* p) {
        Ref r;
        r.ptr_ = p;
        return r;
    }

    /** 生ポインタを引き受ける (参照カウントもインクリメントする)。 */
    static Ref retain(T* p) {
        if (p) {
            p->addRef();
        }
        Ref r;
        r.ptr_ = p;
        return r;
    }


    void reset() {
        if (ptr_) {
            ptr_->release();
            ptr_ = nullptr;
        }
    }

    void detach() { ptr_ = nullptr; }

    T* get() const { return ptr_; }
    T* operator->() const { return ptr_; }
    explicit operator bool() const { return ptr_ != nullptr; }

    bool operator==(const Ref& o) const { return ptr_ == o.ptr_; }
    bool operator!=(const Ref& o) const { return ptr_ != o.ptr_; }

private:
    T* ptr_ = nullptr;
};

} // namespace ln
