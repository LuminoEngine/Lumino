// Copyright (c) 2018 lriki. Distributed under the MIT license.
#pragma once
#include <atomic>
#include <cassert>
#include <type_traits>
#include "Common.hpp"

namespace ln {
class RefObjectHelper;
template<class T>
class Ref;

// unitiliy to store b -> a
#ifndef LN_REFOBJ_SET
#define LN_REFOBJ_SET(a, b)     \
    {                           \
        if (a != b) {           \
            if (b) {            \
                (b)->retain();  \
            }                   \
            if (a) {            \
                (a)->release(); \
            }                   \
            (a) = (b);          \
        }                       \
    }
#endif

// increment reference count
#ifndef LN_SAFE_RETAIN
#define LN_SAFE_RETAIN(p)  \
    {                      \
        if (p) {           \
            (p)->retain(); \
        }                  \
    }
#endif

// decrement reference count
#ifndef LN_SAFE_RELEASE
#define LN_SAFE_RELEASE(p)  \
    {                       \
        if (p) {            \
            (p)->release(); \
            (p) = nullptr;  \
        }                   \
    }
#endif

/**
 * 参照カウントを持つオブジェクトのベースクラスです。
 *
 * 一般的なケースでは、ln::makeRef 関数によりインスタンスを作成し、RefObject 用のスマートポインタである Ref クラスを使用して参照を管理します。
 * 参照を直接操作したい場合は RefObjectHelper クラスを使用します。
 */
class LN_API RefObject
{
protected:
    RefObject();
    virtual ~RefObject();

    /** 参照がなくなり、オブジェクトが削除されようとしているときに呼び出されます。実装コードでは仮想関数を呼び出すことができます。主にデストラクタの制限を回避するために使用します。 */
    virtual void finalize();

    // experimental
    void setValidObject(bool valid = true) { m_ojectFlags = valid; }
    bool isValidObject() const { return m_ojectFlags; }

private:
    RefObject(const RefObject&) = delete;
    void operator=(const RefObject&) = delete;
    void releaseInternal();
    int32_t getReferenceCount() const;
    int32_t retain();
    int32_t release();

    std::atomic<int32_t> m_referenceCount;
    std::atomic<int32_t> m_internalReferenceCount;
    uint32_t m_ojectFlags;

    friend bool valid(const RefObject* obj);
    friend bool valid(const RefObject& obj);

    friend class RefObjectHelper;
    template<class T>
    friend class Ref;
};

inline bool valid(const RefObject* obj)
{
    return obj && obj->isValidObject();
}

inline bool valid(const RefObject& obj)
{
    return obj.isValidObject();
}

/** RefObject 参照を直接操作します。 */
class RefObjectHelper
{
public:
    /** 指定された RefObject の参照カウントを取得します。 */
    static int32_t getReferenceCount(RefObject* obj);

    /** 指定された RefObject の参照カウントをインクリメントします。 */
    static int32_t retain(RefObject* obj);

    /** 指定された RefObject の参照カウントをデクリメントします。0 になった場合、RefObject は削除されます。 */
    static int32_t release(RefObject* obj);
};

/** RefObject 用 スマートポインタ */
template<class T>
class Ref
{
public:
	/** null 値を表します。 */
	static const Ref<T> Null;

    /** 参照を持たない空の Ref を構築します。 */
    LN_CONSTEXPR Ref() LN_NOEXCEPT;

    /** 参照を持たない空の Ref を構築します。 */
    LN_CONSTEXPR Ref(std::nullptr_t) LN_NOEXCEPT;

    /** 生ポインタの所有権を受け取ります。 */
    Ref(T* ptr);

    /** 生ポインタの所有権を受け取ります。retain が false の場合、参照カウントをインクリメントせずに参照します。 */
    Ref(T* ptr, bool retain);

    /** 他の Ref と、参照を共有します。(コピーコンストラクタ) */
    Ref(const Ref& ref) LN_NOEXCEPT;

    /** 他の Ref と、参照を共有します。 */
    template<class Y>
    Ref(const Ref<Y>& ref) LN_NOEXCEPT;

    /** 他の Ref から参照をムーブします。 */
    Ref(Ref&& ref) LN_NOEXCEPT;

    /** 他の Ref から参照をムーブします。 */
    template<class Y>
    Ref(Ref<Y>&& ref) LN_NOEXCEPT;

    /** 保持しているオブジェクトの参照を開放します。 */
    ~Ref();

    /**
     * 保持しているオブジェクトの所有権を放棄し、指定されたオブジェクトの参照を設定します。
     *
     * @param[in]  ptr    : 管理対象としてセットするオブジェクト
     * @param[in]  retain : 参照カウントをインクリメントするかどうか。false の場合、参照カウントをインクリメントせずに参照します。
     */
    void reset(T* ptr, bool retain = true);

    /** 保持しているオブジェクトの参照を放棄します。 */
    void reset();

    /** 2つの Ref オブジェクトを入れ替えます。 */
    void swap(Ref<T>& other);

    /** 保持しているオブジェクトの参照を放棄します。参照カウントはデクリメントしません。 */
    T* detach();

    /** 保持しているオブジェクトへのポインタを取得します。 */
    T* get() const;

    /** 保持しているオブジェクトの所有権を放棄し、他の Ref が持つ参照を共有します。 */
    Ref& operator=(const Ref& ref) LN_NOEXCEPT;

    /** 保持しているオブジェクトの所有権を放棄し、他の Ref が持つ参照を共有します。 */
    template<class Y>
    Ref& operator=(const Ref<Y>& ref) LN_NOEXCEPT;

    /** 保持しているオブジェクトの所有権を放棄し、他の Ref が持つをムーブします。 */
    Ref& operator=(Ref&& ref) LN_NOEXCEPT;

    /** 保持しているオブジェクトの所有権を放棄し、他の Ref が持つをムーブします。 */
    template<class Y>
    Ref& operator=(Ref<Y>&& ref) LN_NOEXCEPT;

    /** ポインタを間接参照します。 */
    T& operator*() const LN_NOEXCEPT;

    /** ポインタを通してオブジェクトにアクセスします。 */
    T* operator->() const LN_NOEXCEPT;

    /** 有効なポインタを保持しているかを確認します。 */
    explicit operator bool() const LN_NOEXCEPT { return (m_ptr != nullptr); }

    /** オブジェクトのポインタへの変換をサポートします。 */
    operator T*() const { return static_cast<T*>(m_ptr); } // ここでコンパイルエラーとなる場合、T の定義があるヘッダファイルを include しているか確認すること。

	RefObject* basePointer() const { return m_ptr; }

protected:
    void safeAddRef()
    {
        LN_SAFE_RETAIN(m_ptr);
    }

    void safeRelease()
    {
        LN_SAFE_RELEASE(m_ptr);
    }

    RefObject* m_ptr;

    template<class U> friend class Ref;
};

template<class T>
const Ref<T> Ref<T>::Null;

template<class T>
LN_CONSTEXPR Ref<T>::Ref() LN_NOEXCEPT
    : m_ptr(nullptr)
{
}

template<class T>
LN_CONSTEXPR Ref<T>::Ref(std::nullptr_t) LN_NOEXCEPT
    : m_ptr(nullptr)
{
}

template<class T>
Ref<T>::Ref(T* ptr)
    : Ref(ptr, true)
{
}

template<class T>
Ref<T>::Ref(T* ptr, bool retain)
    : m_ptr(ptr)
{
    if (retain) {
        LN_SAFE_RETAIN(m_ptr);
    }
}

template<class T>
Ref<T>::Ref(const Ref& ref) LN_NOEXCEPT
    : m_ptr(nullptr)
{
    m_ptr = ref.get();
    LN_SAFE_RETAIN(m_ptr);
}

template<class T>
template<class Y>
Ref<T>::Ref(const Ref<Y>& ref) LN_NOEXCEPT
    : m_ptr(nullptr)
{
    T* t = ref.get();   // confirm implicit cast
    m_ptr = t;
    LN_SAFE_RETAIN(m_ptr);
}

template<class T>
Ref<T>::Ref(Ref&& ref) LN_NOEXCEPT
{
    m_ptr = ref.m_ptr;
    ref.m_ptr = nullptr;
}

template<class T>
template<class Y>
Ref<T>::Ref(Ref<Y>&& ref) LN_NOEXCEPT
{
    T* t = ref.get();   // confirm implicit cast
    m_ptr = t;
    ref.m_ptr = nullptr;
}

template<class T>
Ref<T>::~Ref()
{
    LN_SAFE_RELEASE(m_ptr);
}

template<class T>
void Ref<T>::reset(T* ptr, bool retain)
{
    if (ptr != m_ptr) {
        safeRelease();
        m_ptr = ptr;
        if (retain) {
            safeAddRef();
        }
    }
}

template<class T>
void Ref<T>::reset()
{
    safeRelease();
}

template<class T>
void Ref<T>::swap(Ref<T>& other)
{
    if (&other != this) {
        T* t = m_ptr;
        m_ptr = other.m_ptr;
        other.m_ptr = t;
    }
}

template<class T>
T* Ref<T>::detach()
{
    RefObject* ptr = m_ptr;
    m_ptr = nullptr;
    return static_cast<T*>(ptr);
}

template<class T>
T* Ref<T>::get() const
{
    return static_cast<T*>(m_ptr);
}

template<class T>
Ref<T>& Ref<T>::operator=(const Ref<T>& ref) LN_NOEXCEPT
{
    LN_REFOBJ_SET(m_ptr, ref.m_ptr);
    return *this;
}

template<class T>
template<class Y>
Ref<T>& Ref<T>::operator=(const Ref<Y>& ref) LN_NOEXCEPT
{
    T* t = ref.get();   // confirm implicit cast
    LN_REFOBJ_SET(m_ptr, t);
    return *this;
}

template<class T>
Ref<T>& Ref<T>::operator=(Ref&& ref) LN_NOEXCEPT
{
    if (&ref != this) {
        LN_SAFE_RELEASE(m_ptr);
        m_ptr = ref.m_ptr;
        ref.m_ptr = nullptr;
    }
    return *this;
}

template<class T>
template<class Y>
Ref<T>& Ref<T>::operator=(Ref<Y>&& ref) LN_NOEXCEPT
{
    LN_SAFE_RELEASE(m_ptr);
    T* t = ref.get();   // confirm implicit cast
    m_ptr = t;
    ref.m_ptr = nullptr;
    return *this;
}

template<class T>
T& Ref<T>::operator*() const LN_NOEXCEPT
{
    assert(m_ptr != nullptr);
    return *static_cast<T*>(m_ptr);
}

template<class T>
T* Ref<T>::operator->() const LN_NOEXCEPT
{
    assert(m_ptr != nullptr);
    return static_cast<T*>(m_ptr);
}

template<class T, class U>
bool operator==(const Ref<T>& lhs, const Ref<U>& rhs) LN_NOEXCEPT
{
    return (lhs.basePointer() == rhs.basePointer());
}

template<class T>
bool operator==(const Ref<T>& lhs, std::nullptr_t) LN_NOEXCEPT
{
    return (lhs.basePointer() == nullptr);
}

template<class T>
bool operator==(std::nullptr_t, const Ref<T>& rhs) LN_NOEXCEPT
{
    return (nullptr == rhs.basePointer());
}

template<class T, class U>
bool operator!=(const Ref<T>& lhs, const Ref<U>& rhs) LN_NOEXCEPT
{
    return (lhs.basePointer() != rhs.basePointer());
}

template<class T>
bool operator!=(const Ref<T>& lhs, std::nullptr_t) LN_NOEXCEPT
{
    return (lhs.basePointer() != nullptr);
}

template<class T>
bool operator!=(std::nullptr_t, const Ref<T>& rhs) LN_NOEXCEPT
{
    return (nullptr != rhs.basePointer());
}

template<class T, class U>
bool operator<(const Ref<T>& lhs, const Ref<U>& rhs) LN_NOEXCEPT
{
    return (lhs.basePointer() < rhs.basePointer());
}
	
template<class T>
bool operator<(const Ref<T>& lhs, std::nullptr_t) LN_NOEXCEPT
{
	return std::less<RefObject*>()(lhs.basePointer(), nullptr);
}

template<class T>
bool operator<(std::nullptr_t, const Ref<T>& rhs) LN_NOEXCEPT
{
	return std::less<T*>()(nullptr, rhs.basePointer());
}

template<class T, class U>
bool operator<=(const Ref<T>& lhs, const Ref<U>& rhs) LN_NOEXCEPT
{
    return (lhs.basePointer() <= rhs.basePointer());
}

template<class T>
bool operator<=(const Ref<T>& lhs, std::nullptr_t) LN_NOEXCEPT
{
    return (lhs.basePointer() <= nullptr);
}

template<class T>
bool operator<=(std::nullptr_t, const Ref<T>& rhs) LN_NOEXCEPT
{
    return (nullptr <= rhs.basePointer());
}

template<class T, class U>
bool operator>(const Ref<T>& lhs, const Ref<U>& rhs) LN_NOEXCEPT
{
    return (lhs.basePointer() > rhs.basePointer());
}

template<class T>
bool operator>(const Ref<T>& lhs, std::nullptr_t) LN_NOEXCEPT
{
    return (lhs.basePointer() > nullptr);
}

template<class T>
bool operator>(std::nullptr_t, const Ref<T>& rhs) LN_NOEXCEPT
{
    return (nullptr > rhs.basePointer());
}

template<class T, class U>
bool operator>=(const Ref<T>& lhs, const Ref<U>& rhs) LN_NOEXCEPT
{
    return (lhs.basePointer() >= rhs.basePointer());
}

template<class T>
bool operator>=(const Ref<T>& lhs, std::nullptr_t) LN_NOEXCEPT
{
    return (lhs.basePointer() >= nullptr);
}

template<class T>
bool operator>=(std::nullptr_t, const Ref<T>& rhs) LN_NOEXCEPT
{
    return (nullptr >= rhs.basePointer());
}

/** Cast between RefPtr types statically. */
template<class T, class U>
Ref<T> static_pointer_cast(const Ref<U>& ref)
{
    return Ref<T>(static_cast<T*>(ref.basePointer()));
}

/** Cast between RefPtr types dynamically. */
template<class T, class U>
Ref<T> dynamic_pointer_cast(const Ref<U>& ref)
{
    return Ref<T>(dynamic_cast<T*>(ref.basePointer()));
}

/** Ref を構築します。受け取った引数リストを型 T のコンストラクタへ渡してオブジェクトを構築します。 */
template<class T, class... TArgs>
inline Ref<T> makeRef(TArgs&&... args)
{
    return Ref<T>(LN_NEW T(std::forward<TArgs>(args)...), false);
}


} // namespace ln
