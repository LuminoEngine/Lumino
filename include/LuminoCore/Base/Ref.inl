
/** RefObject 用 スマートポインタ */
template <typename TItem>
class Ref<LN_SPECIALIZED_TYPE>
{
public:
    /** null 値を表します。 */
    static const Ref<LN_SPECIALIZED_TYPE> Null;

    /** 参照を持たない空の Ref を構築します。 */
    LN_CONSTEXPR Ref() LN_NOEXCEPT;

    /** 参照を持たない空の Ref を構築します。 */
    LN_CONSTEXPR Ref(std::nullptr_t) LN_NOEXCEPT;

    /** 生ポインタの所有権を受け取ります。 */
    Ref(LN_SPECIALIZED_TYPE* ptr);

    /** 生ポインタの所有権を受け取ります。retain が false の場合、参照カウントをインクリメントせずに参照します。 */
    Ref(LN_SPECIALIZED_TYPE* ptr, bool retain);

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
    void reset(LN_SPECIALIZED_TYPE* ptr, bool retain = true);

    /** 保持しているオブジェクトの参照を放棄します。 */
    void reset();

    /** 2つの Ref オブジェクトを入れ替えます。 */
    void swap(Ref<LN_SPECIALIZED_TYPE>& other);

    /** 保持しているオブジェクトの参照を放棄します。参照カウントはデクリメントしません。 */
    LN_SPECIALIZED_TYPE* detach();

    /** 保持しているオブジェクトへのポインタを取得します。 */
    LN_SPECIALIZED_TYPE* get() const;

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
    LN_SPECIALIZED_TYPE& operator*() const LN_NOEXCEPT;

    /** ポインタを通してオブジェクトにアクセスします。 */
    LN_SPECIALIZED_TYPE* operator->() const LN_NOEXCEPT;

    /** 有効なポインタを保持しているかを確認します。 */
    explicit operator bool() const LN_NOEXCEPT { return (m_ptr != nullptr); }

    /** オブジェクトのポインタへの変換をサポートします。 */
    operator LN_SPECIALIZED_TYPE*() const { return static_cast<LN_SPECIALIZED_TYPE*>(m_ptr); } // ここでコンパイルエラーとなる場合、LN_SPECIALIZED_TYPE の定義があるヘッダファイルを include しているか確認すること。

    RefObject* basePointer() const { return m_ptr; }






    /** 指定したインデックスにある要素への参照を取得します。*/
    TItem& operator[](int index) { return m_ptr->operator[](index); }

    /** 指定したインデックスにある要素への参照を取得します。*/
    const TItem& operator[](int index) const { return m_ptr->operator[](index); }

    typedef typename std::vector<TItem>::iterator iterator;
    typedef typename std::vector<TItem>::const_iterator const_iterator;

    /** 先頭要素を指すイテレータを取得します。 */
    iterator begin() LN_NOEXCEPT { return m_ptr->begin(); }

    /** 先頭要素を指すイテレータを取得します。 */
    const_iterator begin() const LN_NOEXCEPT { return m_ptr->begin(); }

    /** 末尾の次を指すイテレータを取得します。 */
    iterator end() LN_NOEXCEPT { return m_ptr->end(); }

    /** 末尾の次を指すイテレータを取得します。 */
    const_iterator end() const LN_NOEXCEPT { return m_ptr->end(); }

protected:
    void safeAddRef()
    {
        LN_SAFE_RETAIN(m_ptr);
    }

    void safeRelease()
    {
        LN_SAFE_RELEASE(m_ptr);
    }

    LN_SPECIALIZED_TYPE* m_ptr;

    template<class U> friend class Ref;
};

//template<class TItem>
//const Ref<LN_SPECIALIZED_TYPE> Ref<LN_SPECIALIZED_TYPE>::Null;
//
template<class TItem>
LN_CONSTEXPR Ref<LN_SPECIALIZED_TYPE>::Ref() LN_NOEXCEPT
    : m_ptr(nullptr)
{
}

template<class TItem>
LN_CONSTEXPR Ref<LN_SPECIALIZED_TYPE>::Ref(std::nullptr_t) LN_NOEXCEPT
    : m_ptr(nullptr)
{
}

template<class TItem>
Ref<LN_SPECIALIZED_TYPE>::Ref(LN_SPECIALIZED_TYPE* ptr)
    : Ref(ptr, true)
{
}

template<class TItem>
Ref<LN_SPECIALIZED_TYPE>::Ref(LN_SPECIALIZED_TYPE* ptr, bool retain)
    : m_ptr(ptr)
{
    if (retain) {
        LN_SAFE_RETAIN(m_ptr);
    }
}

template<class TItem>
Ref<LN_SPECIALIZED_TYPE>::Ref(const Ref& ref) LN_NOEXCEPT
    : m_ptr(ref.m_ptr)
{
    LN_SAFE_RETAIN(m_ptr);
}

template<class TItem>
template<class Y>
Ref<LN_SPECIALIZED_TYPE>::Ref(const Ref<Y>& ref) LN_NOEXCEPT
    : m_ptr(static_cast<LN_SPECIALIZED_TYPE*>(ref.get()))
{
    LN_SAFE_RETAIN(m_ptr);
}

template<class TItem>
Ref<LN_SPECIALIZED_TYPE>::Ref(Ref&& ref) LN_NOEXCEPT
{
    m_ptr = ref.m_ptr;
    ref.m_ptr = nullptr;
}

template<class TItem>
template<class Y>
Ref<LN_SPECIALIZED_TYPE>::Ref(Ref<Y>&& ref) LN_NOEXCEPT
{
    m_ptr = static_cast<LN_SPECIALIZED_TYPE*>(ref.get());
    ref.m_ptr = nullptr;
}

template<class TItem>
Ref<LN_SPECIALIZED_TYPE>::~Ref()
{
    LN_SAFE_RELEASE(m_ptr);
}

template<class TItem>
void Ref<LN_SPECIALIZED_TYPE>::reset(LN_SPECIALIZED_TYPE* ptr, bool retain)
{
    if (ptr != m_ptr) {
        safeRelease();
        m_ptr = ptr;
        if (retain) {
            safeAddRef();
        }
    }
}

template<class TItem>
void Ref<LN_SPECIALIZED_TYPE>::reset()
{
    safeRelease();
}

template<class TItem>
void Ref<LN_SPECIALIZED_TYPE>::swap(Ref<LN_SPECIALIZED_TYPE>& other)
{
    if (&other != this) {
        LN_SPECIALIZED_TYPE* t = m_ptr;
        m_ptr = other.m_ptr;
        other.m_ptr = t;
    }
}

template<class TItem>
LN_SPECIALIZED_TYPE* Ref<LN_SPECIALIZED_TYPE>::detach()
{
    RefObject* ptr = m_ptr;
    m_ptr = nullptr;
    return static_cast<LN_SPECIALIZED_TYPE*>(ptr);
}

template<class TItem>
LN_SPECIALIZED_TYPE* Ref<LN_SPECIALIZED_TYPE>::get() const
{
    return static_cast<LN_SPECIALIZED_TYPE*>(m_ptr);
}

template<class TItem>
Ref<LN_SPECIALIZED_TYPE>& Ref<LN_SPECIALIZED_TYPE>::operator=(const Ref<LN_SPECIALIZED_TYPE>& ref) LN_NOEXCEPT
{
    LN_REFOBJ_SET(m_ptr, ref.m_ptr);
    return *this;
}

template<class TItem>
template<class Y>
Ref<LN_SPECIALIZED_TYPE>& Ref<LN_SPECIALIZED_TYPE>::operator=(const Ref<Y>& ref) LN_NOEXCEPT
{
    LN_REFOBJ_SET(m_ptr, static_cast<LN_SPECIALIZED_TYPE*>(ref.get()));
    return *this;
}

template<class TItem>
Ref<LN_SPECIALIZED_TYPE>& Ref<LN_SPECIALIZED_TYPE>::operator=(Ref&& ref) LN_NOEXCEPT
{
    if (&ref != this) {
        LN_SAFE_RELEASE(m_ptr);
        m_ptr = ref.m_ptr;
        ref.m_ptr = nullptr;
    }
    return *this;
}

template<class TItem>
template<class Y>
Ref<LN_SPECIALIZED_TYPE>& Ref<LN_SPECIALIZED_TYPE>::operator=(Ref<Y>&& ref) LN_NOEXCEPT
{
    LN_SAFE_RELEASE(m_ptr);
    m_ptr = static_cast<LN_SPECIALIZED_TYPE*>(ref.get());
    ref.m_ptr = nullptr;
    return *this;
}

template<class TItem>
LN_SPECIALIZED_TYPE& Ref<LN_SPECIALIZED_TYPE>::operator*() const LN_NOEXCEPT
{
    assert(m_ptr != nullptr);
    return *static_cast<LN_SPECIALIZED_TYPE*>(m_ptr);
}

template<class TItem>
LN_SPECIALIZED_TYPE* Ref<LN_SPECIALIZED_TYPE>::operator->() const LN_NOEXCEPT
{
    assert(m_ptr != nullptr);
    return static_cast<LN_SPECIALIZED_TYPE*>(m_ptr);
}
