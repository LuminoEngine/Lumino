// Copyright (c) 2018+ lriki. Distributed under the MIT license..
#pragma once
#include <atomic>
#include <cassert>
#include <type_traits>
#include "Common.hpp"

namespace ln {
template<class T>
class URef;

/**
 * ユニークな参照で所有されることを示すオブジェクト。 URef<> とセットで使う。
 *
 * 一般的なユニーク参照を利用したいときは、 std::unique_ptr が使えます。
 * このクラスを継承する動機は、クラスの定義を見た時にそのインスタンスが ユニークな参照で所有されること を明確に示すことです。
 */
class URefObject {
protected:
    URefObject() = default;
    virtual ~URefObject() LN_NOEXCEPT = default;

    /** 参照がなくなり、オブジェクトが削除されようとしているときに呼び出されます。実装コードでは仮想関数を呼び出すことができます。主にデストラクタの制限を回避するために使用します。 */
    virtual void finalize();

private:
    URefObject(const URefObject&) = delete;
    void operator=(const URefObject&) = delete;

    template<class T>
    friend class URef;
};

/**
 * URefObject 用 スマートポインタ。
 *
 * std::unique_ptr と同様に使用できますが、生ポインタへの暗黙的なキャストが許可されています。
 */
template<class T>
class URef {
public:
    /** 参照を持たない空の URef を構築します。 */
    LN_CONSTEXPR URef() LN_NOEXCEPT;

    /** 参照を持たない空の URef を構築します。 */
    LN_CONSTEXPR URef(std::nullptr_t) LN_NOEXCEPT;

    /** 生ポインタの所有権を受け取ります。 */
    explicit URef(T* ptr) LN_NOEXCEPT;

    /** Move constructor. */
    URef(URef&& ref) LN_NOEXCEPT;

    /** Move constructor. */
    template<class Y>
    URef(URef<Y>&& ref) LN_NOEXCEPT;

    /** 保持しているオブジェクトの参照を開放します。 */
    ~URef() LN_NOEXCEPT;

    // Disable copy
    URef(const URef&) = delete;
    URef& operator=(const URef&) = delete;
    URef& operator=(T* value) = delete;

    /** Deletes the owned pointer.  */
    void reset();

    /** Deletes the owned pointer and takes ownership of the passed in pointer.  */
    void reset(T* ptr);

    /** This simply forgets the owned pointer. It doesn't free it but rather assumes that the user does. */
    T* detach() LN_NOEXCEPT;

    T* release() LN_NOEXCEPT { return detach(); }

    /** Exchanges the owned pointer.  */
    void swap(URef<T>& other) LN_NOEXCEPT;

    /** Returns the owned pointer. */
    T* get() const LN_NOEXCEPT;

    /** Move assignment. */
    URef& operator=(URef&& ref) LN_NOEXCEPT;

    /** Move assignment. */
    template<class Y>
    URef& operator=(URef<Y>&& ref) LN_NOEXCEPT;

    /** Null assignment. */
    URef& operator=(std::nullptr_t) LN_NOEXCEPT;

    /** Returns the owner pointer dereferenced. */
    T& operator*() const LN_NOEXCEPT;

    /** Allows access to the owned pointer via operator->() */
    T* operator->() const LN_NOEXCEPT;

    /** Allows for using a URef as a boolean.  */
    explicit operator bool() const LN_NOEXCEPT { return (m_ptr != nullptr); }

    /** オブジェクトのポインタへの変換をサポートします。 */
    operator T*() const { return static_cast<T*>(m_ptr); }

    //URefObject* basePointer() const { return m_ptr; }

private:
    void safeDelete() {
        if (m_ptr) {
            // ここでコンパイルエラーとなる場合、T の定義があるヘッダファイルを include しているか確認すること。
            static_assert(0 < sizeof(T), "can't delete an incomplete type");
            delete m_ptr;
            m_ptr = nullptr;
        }
    }

    T* m_ptr;

    template<class U>
    friend class URef;
};

template<class T>
LN_CONSTEXPR URef<T>::URef() LN_NOEXCEPT
    : m_ptr(nullptr) {
}

template<class T>
LN_CONSTEXPR URef<T>::URef(std::nullptr_t) LN_NOEXCEPT
    : m_ptr(nullptr) {
}

template<class T>
URef<T>::URef(T* ptr) LN_NOEXCEPT
    : m_ptr(ptr) {
}

template<class T>
URef<T>::URef(URef&& ref) LN_NOEXCEPT
    : m_ptr(ref.detach()) {
}

template<class T>
template<class Y>
URef<T>::URef(URef<Y>&& ref) LN_NOEXCEPT
    : m_ptr(ref.detach()) {
}

template<class T>
URef<T>::~URef() LN_NOEXCEPT {
    reset();
}

template<class T>
void URef<T>::reset() {
    safeDelete();
}

template<class T>
void URef<T>::reset(T* ptr) {
    if (ptr != m_ptr) {
        safeDelete();
        m_ptr = ptr;
    }
}

template<class T>
T* URef<T>::detach() LN_NOEXCEPT {
    URefObject* ptr = m_ptr;
    m_ptr = nullptr;
    return static_cast<T*>(ptr);
}

template<class T>
void URef<T>::swap(URef<T>& other) LN_NOEXCEPT {
    if (&other != this) {
        T* t = m_ptr;
        m_ptr = other.m_ptr;
        other.m_ptr = t;
    }
}

template<class T>
T* URef<T>::get() const LN_NOEXCEPT {
    return static_cast<T*>(m_ptr);
}

template<class T>
URef<T>& URef<T>::operator=(URef&& ref) LN_NOEXCEPT {
    reset(ref.detach());
    return *this;
}

template<class T>
template<class Y>
URef<T>& URef<T>::operator=(URef<Y>&& ref) LN_NOEXCEPT {
    reset(ref.detach());
    return *this;
}

template<class T>
URef<T>& URef<T>::operator=(std::nullptr_t) LN_NOEXCEPT {
    reset();
    return *this;
}

template<class T>
T& URef<T>::operator*() const LN_NOEXCEPT {
    assert(m_ptr != nullptr);
    return *static_cast<T*>(m_ptr);
}

template<class T>
T* URef<T>::operator->() const LN_NOEXCEPT {
    assert(m_ptr != nullptr);
    return static_cast<T*>(m_ptr);
}

template<class T, class U>
bool operator==(const URef<T>& lhs, const URef<U>& rhs) LN_NOEXCEPT {
    return (lhs.get() == rhs.get());
}

template<class T>
bool operator==(const URef<T>& lhs, std::nullptr_t) LN_NOEXCEPT {
    return (lhs.get() == nullptr);
}

template<class T>
bool operator==(std::nullptr_t, const URef<T>& rhs) LN_NOEXCEPT {
    return (nullptr == rhs.get());
}

template<class T, class U>
bool operator!=(const URef<T>& lhs, const URef<U>& rhs) LN_NOEXCEPT {
    return (lhs.get() != rhs.get());
}

template<class T>
bool operator!=(const URef<T>& lhs, std::nullptr_t) LN_NOEXCEPT {
    return (lhs.get() != nullptr);
}

template<class T>
bool operator!=(std::nullptr_t, const URef<T>& rhs) LN_NOEXCEPT {
    return (nullptr != rhs.get());
}

template<class T, class U>
bool operator<(const URef<T>& lhs, const URef<U>& rhs) LN_NOEXCEPT {
    return (lhs.get() < rhs.get());
}

template<class T>
bool operator<(const URef<T>& lhs, std::nullptr_t) LN_NOEXCEPT {
    return std::less<URefObject*>()(lhs.get(), nullptr);
}

template<class T>
bool operator<(std::nullptr_t, const URef<T>& rhs) LN_NOEXCEPT {
    return std::less<T*>()(nullptr, rhs.m_ptr);
}

template<class T, class U>
bool operator<=(const URef<T>& lhs, const URef<U>& rhs) LN_NOEXCEPT {
    return (lhs.m_ptr <= rhs.m_ptr);
}

template<class T>
bool operator<=(const URef<T>& lhs, std::nullptr_t) LN_NOEXCEPT {
    return (lhs.m_ptr <= nullptr);
}

template<class T>
bool operator<=(std::nullptr_t, const URef<T>& rhs) LN_NOEXCEPT {
    return (nullptr <= rhs.m_ptr);
}

template<class T, class U>
bool operator>(const URef<T>& lhs, const URef<U>& rhs) LN_NOEXCEPT {
    return (lhs.m_ptr > rhs.m_ptr);
}

template<class T>
bool operator>(const URef<T>& lhs, std::nullptr_t) LN_NOEXCEPT {
    return (lhs.m_ptr > nullptr);
}

template<class T>
bool operator>(std::nullptr_t, const URef<T>& rhs) LN_NOEXCEPT {
    return (nullptr > rhs.m_ptr);
}

template<class T, class U>
bool operator>=(const URef<T>& lhs, const URef<U>& rhs) LN_NOEXCEPT {
    return (lhs.m_ptr >= rhs.m_ptr);
}

template<class T>
bool operator>=(const URef<T>& lhs, std::nullptr_t) LN_NOEXCEPT {
    return (lhs.m_ptr >= nullptr);
}

template<class T>
bool operator>=(std::nullptr_t, const URef<T>& rhs) LN_NOEXCEPT {
    return (nullptr >= rhs.m_ptr);
}

///** Cast between RefPtr types statically. */
// template<class T, class U>
// URef<T> static_pointer_cast(const URef<U>& ref)
//{
//     return URef<T>(static_cast<T*>(ref.m_ptr));
// }
//
///** Cast between RefPtr types dynamically. */
// template<class T, class U>
// URef<T> dynamic_pointer_cast(const URef<U>& ref)
//{
//     return URef<T>(dynamic_cast<T*>(ref.m_ptr));
// }

/** URef を構築します。受け取った引数リストを型 T のコンストラクタへ渡してオブジェクトを構築します。 */
template<class T, class... TArgs>
inline URef<T> makeURef(TArgs&&... args) {
    return URef<T>(LN_NEW T(std::forward<TArgs>(args)...));
}

} // namespace ln
