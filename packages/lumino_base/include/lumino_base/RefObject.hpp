#pragma once

#include <atomic>
#include <cstdint>
#include <utility>

namespace ln {

/** Intrusive reference-counted base class. */
class RefObject {
public:
    RefObject() = default;
    virtual ~RefObject() = default;

    void addRef() const;

    void release() const;

    uint32_t refCount() const { return refCount_.load(std::memory_order_relaxed); }

protected:
    /** 参照がなくなり、オブジェクトが削除されようとしているときに呼び出されます。実装コードでは仮想関数を呼び出すことができます。主にデストラクタの制限を回避するために使用します。 */
    virtual void finalize();

private:
    RefObject(const RefObject&) = delete;
    RefObject& operator=(const RefObject&) = delete;
    mutable std::atomic<uint32_t> refCount_{1};
};

/**
 * Smart pointer for RefObject objects. Does NOT addRef on construction from raw pointer
 * (assumes ownership of the initial ref). Use Ref::adopt() or Ref::create().
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

    /** Adopt a raw pointer (takes ownership of existing ref). */
    static Ref adopt(T* p) {
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

    T* get() const { return ptr_; }
    T& operator*() const { return *ptr_; }
    T* operator->() const { return ptr_; }
    explicit operator bool() const { return ptr_ != nullptr; }

    bool operator==(const Ref& o) const { return ptr_ == o.ptr_; }
    bool operator!=(const Ref& o) const { return ptr_ != o.ptr_; }

private:
    T* ptr_ = nullptr;
};

} // namespace ln
