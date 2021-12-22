#pragma once

namespace ln {

    
//==============================================================================
// OkType

template<typename T>
struct OkType {
    OkType(const T& val)
        : val(val)
    {}

    OkType(T&& val)
        : val(std::move(val))
    {}

    T val;
};


template<typename T>
struct OkType<T&> {
    OkType(T& val)
        : val(val)
    {}

    T& val;
};

template<>
struct OkType<void> {};

//==============================================================================
// ErrType

//struct DefaultTag {};

template<typename E>
struct ErrType {
    ErrType(const E& val)
        : val(val)
    {}

    ErrType(E&& val)
        : val(std::move(val))
    {}

    E val;
};

struct DefaultErrType {};

//template<>
//struct ErrType<DefaultTag> {
//    ErrType() {}
//};


//==============================================================================
// ok /err

//template<typename T, typename CleanT = typename std::decay<T>::type>
//OkType<CleanT> ok(T&& val) {
//    return OkType<CleanT>(std::forward<T>(val));
//}
template<typename T>
OkType<T> ok(T&& val) {
    return OkType<T>(std::forward<T>(val));
}
//OkType<void> ok() {
//    return OkType<void>();
//}

inline OkType<void> ok() {
    return OkType<void>();
}

template<typename E, typename CleanE = typename std::decay<E>::type>
ErrType<CleanE> err(E&& val) {
    return ErrType<CleanE>(std::forward<E>(val));
}

//inline ErrType<DefaultTag> err() {
//    return ErrType<DefaultTag>();
//}
inline DefaultErrType err() {
    return DefaultErrType();
}

//==============================================================================
// Result

template<typename T = void, typename E = bool>
struct Result {
    Result(T&& ok)
        : ok_(true)
        , ok_v(std::forward<T>(ok))
    {
    }

    Result(OkType<T> ok)
        : ok_(true)
        , ok_v(std::move(ok.val)) {
    }

    Result(ErrType<E> err)
        : ok_(false)
        , err_v(std::move(err.val)) {
    }

    Result(DefaultErrType)
        : ok_(false)
        , err_v{}
    {
    }

    //Result(Result&& other) {
    //}

    //Result(const Result& other) {
    //}

    ~Result() {
        if (ok_) {
            ok_v.~T();
        }
        else {
            err_v.~E();
        }
    }

    bool isOk() const {
        return ok_;
    }

    bool isErr() const {
        return !ok_;
    }

    template<typename U = T>
    typename std::enable_if< !std::is_same<U, void>::value, U>::type unwrapOr(const U& defaultValue) const {
        if (isOk()) {
            return ok_v;
        }
        return defaultValue;
    }

    template<typename U = T>
    typename std::enable_if<!std::is_same<U, void>::value, U>::type unwrap() const {
        LN_CHECK(isOk());
        return ok_v;
    }

    E unwrapErr() const {
        LN_CHECK(isErr());
        return err_v;
    }

    /** 保持している値へのポインタを返します。 */
    constexpr const T* operator->() const {
        LN_CHECK(ok_);
        return &ok_v;
    }

    /** 保持している値へのポインタを返します。 */
    constexpr T* operator->() {
        LN_CHECK(ok_);
        return &ok_v;
    }

    /** 間接参照演算子で値を取得します。 */
    constexpr const T& operator*() const& {
        LN_CHECK(ok_);
        return ok_v;
    }

    /** 間接参照演算子で値を取得します。 */
    constexpr T& operator*() & {
        LN_CHECK(ok_);
        return ok_v;
    }

    /** 間接参照演算子で値を取得します。 */
    constexpr const T&& operator*() const&& {
        LN_CHECK(ok_);
        return ok_v;
    }

    /** 間接参照演算子で値を取得します。 */
    constexpr T&& operator*() && {
        LN_CHECK(ok_);
        return ok_v;
    }

    /** isOk */
    constexpr explicit operator bool() const noexcept {
        return ok_;
    }

private:
    bool ok_;
    union {
        T ok_v;
        E err_v;
    };
};

// for reference
template<typename T, typename E>
struct Result<T&, E> {
    Result(OkType<T&> ok)
        : ok_(true)
        , ok_v(&ok.val) {
    }

    Result(ErrType<E> err)
        : ok_(false)
        , err_v(std::move(err.val)) {
    }

    Result(DefaultErrType)
        : ok_(false)
        , err_v{} {
    }

    //Result(Result&& other) {
    //}

    //Result(const Result& other) {
    //}

    ~Result() {
        if (!ok_) {
            err_v.~E();
        }
    }

    bool isOk() const {
        return ok_;
    }

    bool isErr() const {
        return !ok_;
    }

    template<typename U = T>
    typename std::enable_if<!std::is_same<U, void>::value, U>::type unwrapOr(const U& defaultValue) const {
        if (isOk()) {
            return *ok_v;
        }
        return defaultValue;
    }

    T& unwrap() const {
        LN_CHECK(isOk());
        return *ok_v;
    }

    E unwrapErr() const {
        LN_CHECK(isErr());
        return err_v;
    }

    /** 保持している値へのポインタを返します。 */
    constexpr const T* operator->() const {
        LN_CHECK(ok_);
        return ok_v;
    }

    /** 保持している値へのポインタを返します。 */
    constexpr T* operator->() {
        LN_CHECK(ok_);
        return ok_v;
    }

    /** 間接参照演算子で値を取得します。 */
    constexpr const T& operator*() const& {
        LN_CHECK(ok_);
        return *ok_v;
    }

    /** 間接参照演算子で値を取得します。 */
    constexpr T& operator*() & {
        LN_CHECK(ok_);
        return *ok_v;
    }

    /** 間接参照演算子で値を取得します。 */
    constexpr const T&& operator*() const&& {
        LN_CHECK(ok_);
        return *ok_v;
    }

    /** 間接参照演算子で値を取得します。 */
    constexpr T&& operator*() && {
        LN_CHECK(ok_);
        return *ok_v;
    }

    /** isOk */
    constexpr explicit operator bool() const noexcept {
        return ok_;
    }


private:
    bool ok_;
    union {
        T* ok_v;
        E err_v;
    };
};

template<typename E>
struct Result<void, E> {
    Result(OkType<void> ok)
        : ok_(true) {
    }

    Result(ErrType<E> err)
        : ok_(false)
        , err_v(std::move(err.val)) {
    }

    Result(DefaultErrType)
        : ok_(false)
        , err_v{} {
    }

    //Result(Result&& other) {
    //}

    //Result(const Result& other) {
    //}

    ~Result() {
        if (!ok_) {
            err_v.~E();
        }
    }

    bool isOk() const {
        return ok_;
    }

    bool isErr() const {
        return !ok_;
    }

    E unwrapErr() const {
        LN_CHECK(isErr());
        return err_v;
    }

    constexpr explicit operator bool() const noexcept { return ok_; }

private:
    bool ok_;
    E err_v;
};

} // namespace ln
