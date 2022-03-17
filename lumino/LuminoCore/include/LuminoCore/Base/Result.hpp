#pragma once

#define LN_RESULT_BOOL_CONVERSION 1

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
// BasicResult

template<typename T = void, typename E = bool>
struct BasicResult {
    BasicResult(T&& ok)
        : ok_(true)
        , ok_v(std::forward<T>(ok))
    {
    }

    BasicResult(OkType<T> ok)
        : ok_(true)
        , ok_v(std::move(ok.val)) {
    }

    BasicResult(ErrType<E> err)
        : ok_(false)
        , err_v(std::move(err.val)) {
    }

    BasicResult(DefaultErrType)
        : ok_(false)
        , err_v{}
    {
    }

#if defined(LN_RESULT_BOOL_CONVERSION)
    explicit BasicResult(bool result)
        : ok_(result)
        , ok_v{} {
    }
#endif

    //BasicResult(BasicResult&& other) {
    //}

    //BasicResult(const BasicResult& other) {
    //}

    ~BasicResult() {
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
struct BasicResult<T&, E> {
    BasicResult(OkType<T&> ok)
        : ok_(true)
        , ok_v(&ok.val) {
    }

    BasicResult(ErrType<E> err)
        : ok_(false)
        , err_v(std::move(err.val)) {
    }

    BasicResult(DefaultErrType)
        : ok_(false)
        , err_v{} {
    }

#if defined(LN_RESULT_BOOL_CONVERSION)
    explicit BasicResult(bool result)
        : ok_(result)
        , ok_v{} {
    }
#endif

    //BasicResult(BasicResult&& other) {
    //}

    //BasicResult(const BasicResult& other) {
    //}

    ~BasicResult() {
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
struct BasicResult<void, E> {
    BasicResult(OkType<void> ok)
        : ok_(true) {
    }

    BasicResult(ErrType<E> err)
        : ok_(false)
        , err_v(std::move(err.val)) {
    }

    BasicResult(DefaultErrType)
        : ok_(false)
        , err_v{} {
    }

#if defined(LN_RESULT_BOOL_CONVERSION)
    explicit BasicResult(bool result)
        : ok_(result)
        , err_v{} {
    }
#endif

    //BasicResult(BasicResult&& other) {
    //}

    //BasicResult(const BasicResult& other) {
    //}

    ~BasicResult() {
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


//==============================================================================
// In library common resut

enum class ErrorCode {
	Unknown = 0,
};

using Result = BasicResult<void, ErrorCode>;

} // namespace ln

#define LN_DEFINE_RESULT_ALIAS \
    using Result = ln::BasicResult<void, ln::ErrorCode>; \
    template<typename... Args> \
    auto ok(Args&&... args)->decltype(ln::ok(std::forward<Args>(args)...)) { return ln::ok(std::forward<Args>(args)...); } \
    template<typename... Args> \
    auto err(Args&&... args)->decltype(ln::err(std::forward<Args>(args)...)) { return ln::err(std::forward<Args>(args)...); } \


#define LN_TRY(x) { \
    const auto result = x; \
    if (!result) return result; \
}
