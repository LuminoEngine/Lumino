

inline RefObject* Ref<LN_SPECIALIZED_TYPE>::basePointer() const
{
    return m_ptr;
}

inline void Ref<LN_SPECIALIZED_TYPE>::safeAddRef()
{
    LN_SAFE_RETAIN(m_ptr);
}

inline void Ref<LN_SPECIALIZED_TYPE>::safeRelease()
{
    LN_SAFE_RELEASE(m_ptr);
}

//template<class TItem>
//const Ref<LN_SPECIALIZED_TYPE> Ref<LN_SPECIALIZED_TYPE>::Null;
//
inline LN_CONSTEXPR Ref<LN_SPECIALIZED_TYPE>::Ref() LN_NOEXCEPT
    : m_ptr(nullptr)
{
}

inline LN_CONSTEXPR Ref<LN_SPECIALIZED_TYPE>::Ref(std::nullptr_t) LN_NOEXCEPT
    : m_ptr(nullptr)
{
}

inline Ref<LN_SPECIALIZED_TYPE>::Ref(LN_SPECIALIZED_TYPE* ptr)
    : Ref(ptr, true)
{
}

inline Ref<LN_SPECIALIZED_TYPE>::Ref(LN_SPECIALIZED_TYPE* ptr, bool retain)
    : m_ptr(ptr)
{
    if (retain) {
        LN_SAFE_RETAIN(m_ptr);
    }
}

inline Ref<LN_SPECIALIZED_TYPE>::Ref(const Ref& ref) LN_NOEXCEPT
    : m_ptr(ref.m_ptr)
{
    LN_SAFE_RETAIN(m_ptr);
}

template<class Y>
Ref<LN_SPECIALIZED_TYPE>::Ref(const Ref<Y>& ref) LN_NOEXCEPT
    : m_ptr(static_cast<LN_SPECIALIZED_TYPE*>(ref.get()))
{
    LN_SAFE_RETAIN(m_ptr);
}

inline Ref<LN_SPECIALIZED_TYPE>::Ref(Ref&& ref) LN_NOEXCEPT
{
    m_ptr = ref.m_ptr;
    ref.m_ptr = nullptr;
}

template<class Y>
Ref<LN_SPECIALIZED_TYPE>::Ref(Ref<Y>&& ref) LN_NOEXCEPT
{
    m_ptr = static_cast<LN_SPECIALIZED_TYPE*>(ref.get());
    ref.m_ptr = nullptr;
}

inline Ref<LN_SPECIALIZED_TYPE>::~Ref()
{
    LN_SAFE_RELEASE(m_ptr);
}

inline void Ref<LN_SPECIALIZED_TYPE>::reset(LN_SPECIALIZED_TYPE* ptr, bool retain)
{
    if (ptr != m_ptr) {
        safeRelease();
        m_ptr = ptr;
        if (retain) {
            safeAddRef();
        }
    }
}

inline void Ref<LN_SPECIALIZED_TYPE>::reset()
{
    safeRelease();
}

inline void Ref<LN_SPECIALIZED_TYPE>::swap(Ref<LN_SPECIALIZED_TYPE>& other)
{
    if (&other != this) {
        LN_SPECIALIZED_TYPE* t = m_ptr;
        m_ptr = other.m_ptr;
        other.m_ptr = t;
    }
}

inline LN_SPECIALIZED_TYPE* Ref<LN_SPECIALIZED_TYPE>::detach()
{
    RefObject* ptr = m_ptr;
    m_ptr = nullptr;
    return static_cast<LN_SPECIALIZED_TYPE*>(ptr);
}

inline LN_SPECIALIZED_TYPE* Ref<LN_SPECIALIZED_TYPE>::get() const
{
    return static_cast<LN_SPECIALIZED_TYPE*>(m_ptr);
}

inline Ref<LN_SPECIALIZED_TYPE>& Ref<LN_SPECIALIZED_TYPE>::operator=(const Ref<LN_SPECIALIZED_TYPE>& ref) LN_NOEXCEPT
{
    LN_REFOBJ_SET(m_ptr, ref.m_ptr);
    return *this;
}

template<class Y>
Ref<LN_SPECIALIZED_TYPE>& Ref<LN_SPECIALIZED_TYPE>::operator=(const Ref<Y>& ref) LN_NOEXCEPT
{
    LN_REFOBJ_SET(m_ptr, static_cast<LN_SPECIALIZED_TYPE*>(ref.get()));
    return *this;
}

inline Ref<LN_SPECIALIZED_TYPE>& Ref<LN_SPECIALIZED_TYPE>::operator=(Ref&& ref) LN_NOEXCEPT
{
    if (&ref != this) {
        LN_SAFE_RELEASE(m_ptr);
        m_ptr = ref.m_ptr;
        ref.m_ptr = nullptr;
    }
    return *this;
}

template<class Y>
Ref<LN_SPECIALIZED_TYPE>& Ref<LN_SPECIALIZED_TYPE>::operator=(Ref<Y>&& ref) LN_NOEXCEPT
{
    LN_SAFE_RELEASE(m_ptr);
    m_ptr = static_cast<LN_SPECIALIZED_TYPE*>(ref.get());
    ref.m_ptr = nullptr;
    return *this;
}

inline LN_SPECIALIZED_TYPE& Ref<LN_SPECIALIZED_TYPE>::operator*() const LN_NOEXCEPT
{
    assert(m_ptr != nullptr);
    return *static_cast<LN_SPECIALIZED_TYPE*>(m_ptr);
}

inline LN_SPECIALIZED_TYPE* Ref<LN_SPECIALIZED_TYPE>::operator->() const LN_NOEXCEPT
{
    assert(m_ptr != nullptr);
    return static_cast<LN_SPECIALIZED_TYPE*>(m_ptr);
}
