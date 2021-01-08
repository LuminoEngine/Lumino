

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

    RefObject* basePointer() const;


protected:
    void safeAddRef();

    void safeRelease();

    LN_SPECIALIZED_TYPE* m_ptr;

    template<class U> friend class Ref;
