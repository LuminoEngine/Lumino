#pragma once
#include "String.hpp"

#define LN_RESULT_BOOL_CONVERSION 1

namespace ln {

template<class T>
String toString(const T& value);

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

class ResultBase {
public:
    typedef String (*Serializer)(const ResultBase* self);

    ResultBase(Serializer serializer)
        : m_serializer(serializer) {}
    virtual ~ResultBase() = default;
    virtual std::unique_ptr<ResultBase> moveBoxing() = 0;
    //String toString() const { return m_serializer(this); }
    virtual String toString() const = 0;

    Serializer m_serializer;
};

//==============================================================================
// BasicResult

/** @see https://github.com/LuminoEngine/Lumino/wiki/ErrorHandling */
template<typename T = void, typename E = bool>
class BasicResult : public ResultBase {
public:
    BasicResult(T&& ok)
        : ResultBase(toStringInternal)
        , ok_(true)
        , ok_v(std::forward<T>(ok))
    {
    }

    BasicResult(OkType<T> ok)
        : ResultBase(toStringInternal)
        , ok_(true)
        , ok_v(std::move(ok.val)) {
    }

    BasicResult(ErrType<E> err)
        : ResultBase(toStringInternal)
        , ok_(false)
        , err_v(std::move(err.val)) {
    }

    BasicResult(DefaultErrType)
        : ResultBase(toStringInternal)
        , ok_(false)
        , err_v{}
    {
    }

    template<class U>
    BasicResult(OkType<U> ok)
        : ResultBase(toStringInternal)
        , ok_(true)
        , ok_v(std::move(ok.val)) {
    }

    /** Converting copy constructor. */
    template<class U>
    BasicResult(ErrType<U> err)
        : ResultBase(toStringInternal)
        , ok_(false)
        , err_v(std::move(err.val)) {
    }

    /** Boxing copy(move) constructor. */
    template<class UT, class UE>
    BasicResult(BasicResult<UT, UE>& other)
        : ResultBase(toStringInternal) 
        , ok_(other.isOk())
        , err_v{}
        , internalResult_(other.moveBoxing()) {
    }

#if defined(LN_RESULT_BOOL_CONVERSION)
    explicit BasicResult(bool result)
        : ResultBase(toStringInternal)
        , ok_(result)
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

    //template<typename U = T>
    //typename std::enable_if<!std::is_same<U, void>::value, U>::type unwrap() const {
    //    if (isErr()) {
    //        LN_ERROR(toString());
    //    }
    //    LN_CHECK(isOk());
    //    return ok_v;
    //}

    typename T& unwrap() & {
        if (isErr()) {
            LN_ERROR(toString());
        }
        LN_CHECK(isOk());
        return ok_v;
    }

    typename const T& unwrap() const & {
        if (isErr()) {
            LN_ERROR(toString());
        }
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
        return std::move(ok_v);
    }

    /** 間接参照演算子で値を取得します。 */
    constexpr T&& operator*() && {
        LN_CHECK(ok_);
        return std::move(ok_v);
    }

    /** isOk */
    constexpr explicit operator bool() const noexcept {
        return ok_;
    }

    std::unique_ptr<ResultBase> moveBoxing() override {
        return std::unique_ptr<ResultBase>(new BasicResult<T, E>(std::move(*this)));
    }
    
    String toString() const override {
        if (isOk()) return {};
        if (internalResult_) return ln::toString(err_v) + U"\n" + internalResult_->toString();
        return ln::toString(err_v);
    }

    static String toStringInternal(const ResultBase* s) {
        const auto* self = static_cast<const BasicResult*>(s);
        if (self->isOk()) return {};
        if (self->internalResult_) return ln::toString(self->err_v) + U"\n" + self->internalResult_->toString();
        return ln::toString(self->err_v);
    }

private:
    //BasicResult() 
    //    : ResultBase(toStringInternal)
    //{}

    BasicResult(BasicResult&& other)
        : ResultBase(toStringInternal)
        , ok_(other.ok_)
        , err_v(std::move(other.err_v))
        , internalResult_(std::move(other.internalResult_)) {
    }

    bool ok_;
    union {
        T ok_v;
        E err_v;
    };
    std::unique_ptr<ResultBase> internalResult_;
};

// for reference
template<typename T, typename E>
class BasicResult<T&, E> : public ResultBase {
public:
    BasicResult(OkType<T&> ok)
        : ResultBase(toStringInternal)
        , ok_(true)
        , ok_v(&ok.val) {
    }

    BasicResult(ErrType<E> err)
        : ResultBase(toStringInternal)
        , ok_(false)
        , err_v(std::move(err.val)) {
    }

    BasicResult(DefaultErrType)
        : ResultBase(toStringInternal)
        , ok_(false)
        , err_v{} {
    }

#if defined(LN_RESULT_BOOL_CONVERSION)
    explicit BasicResult(bool result)
        : ResultBase(toStringInternal)
        , ok_(result)
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
        return std::move(*ok_v);
    }

    /** 間接参照演算子で値を取得します。 */
    constexpr T&& operator*() && {
        LN_CHECK(ok_);
        return std::move(*ok_v);
    }

    /** isOk */
    constexpr explicit operator bool() const noexcept {
        return ok_;
    }

    std::unique_ptr<ResultBase> moveBoxing() override {
        return std::unique_ptr<ResultBase>(new BasicResult<T&, E>(std::move(*this)));
    }

    String toString() const override {
        LN_NOTIMPLEMENTED();
        return U"";
    }

    static String toStringInternal(const ResultBase* s) {
        LN_NOTIMPLEMENTED();
        return U"";
    }

private:
    BasicResult(BasicResult&& other)
        : ResultBase(toStringInternal)
        , ok_(other.ok_)
        , err_v(std::move(other.err_v))
        , internalResult_(std::move(other.internalResult_)) {
    }

    bool ok_;
    union {
        T* ok_v;
        E err_v;
    };
    std::unique_ptr<ResultBase> internalResult_;
};

// void type
template<typename E>
class BasicResult<void, E> : public ResultBase {
public:
    BasicResult(OkType<void> ok)
        : ResultBase(toStringInternal)
        , ok_(true) {
    }

    BasicResult(ErrType<E> err)
        : ResultBase(toStringInternal)
        , ok_(false)
        , err_v(std::move(err.val)) {
    }

    BasicResult(DefaultErrType)
        : ResultBase(toStringInternal)
        , ok_(false)
        , err_v{} {
    }

    /** Converting copy constructor. */
    template<class U>
    BasicResult(ErrType<U> err)
        : ResultBase(toStringInternal)
        , ok_(false)
        , err_v(std::move(err.val)) {
    }

    /** Boxing copy(move) constructor. */
    template<class UT, class UE>
    BasicResult(BasicResult<UT, UE>& other)
        : ResultBase(toStringInternal)
        , ok_(false)
        , err_v{}
        , internalResult_(other.moveBoxing()) {
    }

    /** Boxing copy(move) constructor. */
    BasicResult(BasicResult& other)
        : ResultBase(toStringInternal)
        , ok_(other.ok_)
        , err_v{}
        , internalResult_(other.moveBoxing()) {
    }

    //BasicResult(ResultBase&& other)
    //    : ok_(false)
    //    , err_v{}
    //    , internalResult_(other.moveBoxing()) {
    //}


#if defined(LN_RESULT_BOOL_CONVERSION)
    explicit BasicResult(bool result)
        : ResultBase(toStringInternal)
        , ok_(result)
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

    void unwrap() const {
        if (isErr()) {
            LN_ERROR(toString());
        }
        LN_CHECK(isOk());
    }

    constexpr explicit operator bool() const noexcept { return ok_; }

    std::unique_ptr<ResultBase> moveBoxing() override {
        return std::unique_ptr<ResultBase>(new BasicResult<void, E>(std::move(*this), true));
    }
    
    String toString() const override {
        if (isOk()) return {};
        if (internalResult_) return ln::toString(err_v) + U"\n" + internalResult_->toString();
        return ln::toString(err_v);
    }

    static String toStringInternal(const ResultBase* s) {
        const auto* self = static_cast<const BasicResult*>(s);
        if (self->isOk()) return {};
        if (self->internalResult_) return ln::toString(self->err_v) + U"\n" + self->internalResult_->toString();
        return ln::toString(self->err_v);
    }

private:
    //BasicResult()
    //    : ResultBase(toStringInternal) {}

    explicit BasicResult(BasicResult&& other, bool)
        : ResultBase(toStringInternal)
        , ok_(other.ok_)
        , err_v(std::move(other.err_v))
        , internalResult_(std::move(other.internalResult_)) {
    }

    bool ok_;
    E err_v;
    std::unique_ptr<ResultBase> internalResult_;
};


//==============================================================================
// In library common resut

enum class ErrorCode {
	Unknown = 0,
};

/** @see https://github.com/LuminoEngine/Lumino/wiki/ErrorHandling */
using Result = BasicResult<void, ErrorCode>;

template<class T>
using GenericResult = BasicResult<T, ErrorCode>;

template<>
inline String toString<int>(const int& v) {
    return String::fromNumber(v);
}

template<>
inline String toString<bool>(const bool& v) {
    return v ? String(U"true") : String(U"false");
}

template<>
inline String toString<ErrorCode>(const ErrorCode& e) {
    return ln::format(U"ErrorCode:{}", static_cast<int>(e));
}

template<class TResultValue, class TResultError>
inline String toString(const BasicResult<TResultValue, TResultError>& e) {
    return e.toString();
}

} // namespace ln

#define LN_DEFINE_RESULT_ALIAS \
    using Result = ln::BasicResult<void, ln::ErrorCode>; \
    template<typename... Args> \
    auto ok(Args&&... args)->decltype(ln::ok(std::forward<Args>(args)...)) { return ln::ok(std::forward<Args>(args)...); } \
    template<typename... Args> \
    auto err(Args&&... args)->decltype(ln::err(std::forward<Args>(args)...)) { return ln::err(std::forward<Args>(args)...); } \


#define LN_TRY(x) { \
    auto result = x; \
    if (!result) return result; \
}
