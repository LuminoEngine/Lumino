#pragma once
#include "detail/tl/expected.hpp"
#include "String.hpp"

namespace ln {

/**
 * Dummy Value. It is provided for std::expected or tl::expected use. 
 * Lumino communicates details by logging errors, so there are no plans to make error codes more specific. (yet)
 */
enum class ErrorCode {
    Unknown = 0,
};

template<class T>
using Result = tl::expected<T, ErrorCode>;

using MaybeResult = tl::expected<void, ErrorCode>;

#define LN_MAKE_SUCCESS() {}
#define LN_MAKE_ERROR(...) ::ln::detail::makeInternalError(::ln::detail::formatString(__VA_ARGS__), __FILE__, __func__, __LINE__);
#define LN_MAKE_ERROR_UNREACHABLE() LN_MAKE_ERROR("Unreachable code reached.")
#define LN_MAKE_ERROR_NOT_IMPLEMENTED() LN_MAKE_ERROR("Not implemented.")

/** A macro to use during the migration period, which throws an exception if it fails. */
#define LN_ASSERT_RESULT(result) LN_ASSERT(!!result)
#define LN_TO_ERROR(result) ::ln::detail::toError(result)

namespace detail {

inline std::string formatString() { return {}; }
std::string formatString(const char* format, ...);
tl::unexpected<ErrorCode> makeInternalError(const std::string& message, const char* file, const char* function, int line);

} // namespace detail
} // namespace ln

//==============================================================================
// !!! DEPRECATED following !!!
// std::expected 標準化への対応を見据えて、独自の Result 型は廃止します。
//==============================================================================

#define LN_RESULT_BOOL_CONVERSION 1

namespace ln {

template<class T>
String toString(const T& value);


//==============================================================================
// OkType_deprecated

template<typename T>
struct OkType_deprecated {
    OkType_deprecated(const T& val)
        : val(val)
    {}

    OkType_deprecated(T&& val)
        : val(std::move(val))
    {}

    T val;
};


template<typename T>
struct OkType_deprecated<T&> {
    OkType_deprecated(T& val)
        : val(val)
    {}

    T& val;
};

template<>
struct OkType_deprecated<void> {};

//==============================================================================
// ErrType_deprecated

//struct DefaultTag {};

template<typename E>
struct ErrType_deprecated {
    ErrType_deprecated(const E& val)
        : val(val)
    {}

    ErrType_deprecated(E&& val)
        : val(std::move(val))
    {}

    E val;
};

struct DefaultErrType_deprecated {};

//template<>
//struct ErrType_deprecated<DefaultTag> {
//    ErrType_deprecated() {}
//};


//==============================================================================
// ok /err

//template<typename T, typename CleanT = typename std::decay<T>::type>
//OkType_deprecated<CleanT> ok(T&& val) {
//    return OkType_deprecated<CleanT>(std::forward<T>(val));
//}
template<typename T>
OkType_deprecated<T> ok(T&& val) {
    return OkType_deprecated<T>(std::forward<T>(val));
}
//OkType_deprecated<void> ok() {
//    return OkType_deprecated<void>();
//}

inline OkType_deprecated<void> ok() {
    return OkType_deprecated<void>();
}

template<typename E, typename CleanE = typename std::decay<E>::type>
ErrType_deprecated<CleanE> err(E&& val) {
    return ErrType_deprecated<CleanE>(std::forward<E>(val));
}

//inline ErrType_deprecated<DefaultTag> err() {
//    return ErrType_deprecated<DefaultTag>();
//}
inline DefaultErrType_deprecated err() {
    return DefaultErrType_deprecated();
}

class ResultBase_deprecated {
public:
    typedef String (*Serializer)(const ResultBase_deprecated* self);

    ResultBase_deprecated(Serializer serializer)
        : m_serializer(serializer) {}
    virtual ~ResultBase_deprecated() = default;
    virtual std::unique_ptr<ResultBase_deprecated> moveBoxing() = 0;
    //String toString() const { return m_serializer(this); }
    virtual String toString() const = 0;

    Serializer m_serializer;
};

namespace detail {

inline std::string formatString_deprecated() { return {}; }
std::string formatString_deprecated(const char* format, ...);
ErrType_deprecated<ErrorCode> makeInternalError_deprecated(const std::string& message, const char* file, const char* function, int line);

} // namespace detail

#define LN_MAKE_SUCCESS() {}
#define LN_MAKE_ERROR_deprecated(...) ::ln::detail::makeInternalError_deprecated(::ln::detail::formatString_deprecated(__VA_ARGS__), __FILE__, __func__, __LINE__);
#define LN_MAKE_ERROR_UNREACHABLE_deprecated() LN_MAKE_ERROR_deprecated("Unreachable code reached.")
#define LN_MAKE_ERROR_NOT_IMPLEMENTED_deprecated() LN_MAKE_ERROR_deprecated("Not implemented.")

#define LN_TRY(x)                   \
    {                               \
        auto result = x;            \
        if (!result) return result; \
    }

#define LN_TRY_ASSERT(expr)                                                          \
    if (LN_ASSERT(expr)) return LN_MAKE_ERROR();


//==============================================================================
// BasicResult_deprecated

/** @see https://github.com/LuminoEngine/Lumino/wiki/ErrorHandling */
template<typename T = void, typename E = bool>
class [[nodiscard]] BasicResult_deprecated : public ResultBase_deprecated {
public:
    BasicResult_deprecated(T&& ok)
        : ResultBase_deprecated(toStringInternal)
        , ok_(true)
        , ok_v(std::forward<T>(ok))
    {
    }

    BasicResult_deprecated(OkType_deprecated<T> ok)
        : ResultBase_deprecated(toStringInternal)
        , ok_(true)
        , ok_v(std::move(ok.val)) {
    }

    BasicResult_deprecated(ErrType_deprecated<E> err)
        : ResultBase_deprecated(toStringInternal)
        , ok_(false)
        , err_v(std::move(err.val)) {
    }

    BasicResult_deprecated(DefaultErrType_deprecated)
        : ResultBase_deprecated(toStringInternal)
        , ok_(false)
        , err_v{}
    {
    }

    template<class U>
    BasicResult_deprecated(OkType_deprecated<U> ok)
        : ResultBase_deprecated(toStringInternal)
        , ok_(true)
        , ok_v(std::move(ok.val)) {
    }

    /** Converting copy constructor. */
    template<class U>
    BasicResult_deprecated(ErrType_deprecated<U> err)
        : ResultBase_deprecated(toStringInternal)
        , ok_(false)
        , err_v(std::move(err.val)) {
    }

    /** Boxing copy(move) constructor. */
    template<class UT, class UE>
    BasicResult_deprecated(BasicResult_deprecated<UT, UE>& other)
        : ResultBase_deprecated(toStringInternal) 
        , ok_(other.isOk())
        , err_v{}
        , internalResult_(other.moveBoxing()) {
    }

#if defined(LN_RESULT_BOOL_CONVERSION)
    explicit BasicResult_deprecated(bool result)
        : ResultBase_deprecated(toStringInternal)
        , ok_(result)
        , ok_v{} {
    }
#endif

    //BasicResult_deprecated(BasicResult_deprecated&& other) {
    //}

    //BasicResult_deprecated(const BasicResult_deprecated& other) {
    //}

    ~BasicResult_deprecated() {
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

    T& unwrap() & {
        if (isErr()) {
            LN_ERROR(toString());
        }
        LN_CHECK(isOk());
        return ok_v;
    }

    const T& unwrap() const & {
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

    std::unique_ptr<ResultBase_deprecated> moveBoxing() override {
        return std::unique_ptr<ResultBase_deprecated>(new BasicResult_deprecated<T, E>(std::move(*this)));
    }
    
    String toString() const override {
        if (isOk()) return {};
        if (internalResult_) return ln::toString(err_v) + U"\n" + internalResult_->toString();
        return ln::toString(err_v);
    }

    static String toStringInternal(const ResultBase_deprecated* s) {
        const auto* self = static_cast<const BasicResult_deprecated*>(s);
        if (self->isOk()) return {};
        if (self->internalResult_) return ln::toString(self->err_v) + U"\n" + self->internalResult_->toString();
        return ln::toString(self->err_v);
    }

private:
    //BasicResult_deprecated() 
    //    : ResultBase_deprecated(toStringInternal)
    //{}

    BasicResult_deprecated(BasicResult_deprecated&& other)
        : ResultBase_deprecated(toStringInternal)
        , ok_(other.ok_)
        , err_v(std::move(other.err_v))
        , internalResult_(std::move(other.internalResult_)) {
    }

    bool ok_;
    union {
        T ok_v;
        E err_v;
    };
    std::unique_ptr<ResultBase_deprecated> internalResult_;
};

// for reference
template<typename T, typename E>
class BasicResult_deprecated<T&, E> : public ResultBase_deprecated {
public:
    BasicResult_deprecated(OkType_deprecated<T&> ok)
        : ResultBase_deprecated(toStringInternal)
        , ok_(true)
        , ok_v(&ok.val) {
    }

    BasicResult_deprecated(ErrType_deprecated<E> err)
        : ResultBase_deprecated(toStringInternal)
        , ok_(false)
        , err_v(std::move(err.val)) {
    }

    BasicResult_deprecated(DefaultErrType_deprecated)
        : ResultBase_deprecated(toStringInternal)
        , ok_(false)
        , err_v{} {
    }

#if defined(LN_RESULT_BOOL_CONVERSION)
    explicit BasicResult_deprecated(bool result)
        : ResultBase_deprecated(toStringInternal)
        , ok_(result)
        , ok_v{} {
    }
#endif

    //BasicResult_deprecated(BasicResult_deprecated&& other) {
    //}

    //BasicResult_deprecated(const BasicResult_deprecated& other) {
    //}

    ~BasicResult_deprecated() {
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

    std::unique_ptr<ResultBase_deprecated> moveBoxing() override {
        return std::unique_ptr<ResultBase_deprecated>(new BasicResult_deprecated<T&, E>(std::move(*this)));
    }

    String toString() const override {
        LN_NOTIMPLEMENTED();
        return U"";
    }

    static String toStringInternal(const ResultBase_deprecated* s) {
        LN_NOTIMPLEMENTED();
        return U"";
    }

private:
    BasicResult_deprecated(BasicResult_deprecated&& other)
        : ResultBase_deprecated(toStringInternal)
        , ok_(other.ok_)
        , err_v(std::move(other.err_v))
        , internalResult_(std::move(other.internalResult_)) {
    }

    bool ok_;
    union {
        T* ok_v;
        E err_v;
    };
    std::unique_ptr<ResultBase_deprecated> internalResult_;
};

// void type
template<typename E>
class BasicResult_deprecated<void, E> : public ResultBase_deprecated {
public:
    BasicResult_deprecated()
		: ResultBase_deprecated(toStringInternal)
		, ok_(true) {
	}

    BasicResult_deprecated(OkType_deprecated<void> ok)
        : ResultBase_deprecated(toStringInternal)
        , ok_(true) {
    }

    BasicResult_deprecated(ErrType_deprecated<E> err)
        : ResultBase_deprecated(toStringInternal)
        , ok_(false)
        , err_v(std::move(err.val)) {
    }

    BasicResult_deprecated(DefaultErrType_deprecated)
        : ResultBase_deprecated(toStringInternal)
        , ok_(false)
        , err_v{} {
    }

    /** Converting copy constructor. */
    template<class U>
    BasicResult_deprecated(ErrType_deprecated<U> err)
        : ResultBase_deprecated(toStringInternal)
        , ok_(false)
        , err_v(std::move(err.val)) {
    }

    /** Boxing copy(move) constructor. */
    template<class UT, class UE>
    BasicResult_deprecated(BasicResult_deprecated<UT, UE>& other)
        : ResultBase_deprecated(toStringInternal)
        , ok_(false)
        , err_v{}
        , internalResult_(other.moveBoxing()) {
    }

    /** Boxing copy(move) constructor. */
    BasicResult_deprecated(BasicResult_deprecated& other)
        : ResultBase_deprecated(toStringInternal)
        , ok_(other.ok_)
        , err_v{}
        , internalResult_(other.moveBoxing()) {
    }

    //BasicResult_deprecated(ResultBase_deprecated&& other)
    //    : ok_(false)
    //    , err_v{}
    //    , internalResult_(other.moveBoxing()) {
    //}


#if defined(LN_RESULT_BOOL_CONVERSION)
    explicit BasicResult_deprecated(bool result)
        : ResultBase_deprecated(toStringInternal)
        , ok_(result)
        , err_v{} {
    }
#endif

    //BasicResult_deprecated(BasicResult_deprecated&& other) {
    //}

    //BasicResult_deprecated(const BasicResult_deprecated& other) {
    //}

    ~BasicResult_deprecated() {
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

    std::unique_ptr<ResultBase_deprecated> moveBoxing() override {
        return std::unique_ptr<ResultBase_deprecated>(new BasicResult_deprecated<void, E>(std::move(*this), true));
    }
    
    String toString() const override {
        if (isOk()) return {};
        if (internalResult_) return ln::toString(err_v) + U"\n" + internalResult_->toString();
        return ln::toString(err_v);
    }

    static String toStringInternal(const ResultBase_deprecated* s) {
        const auto* self = static_cast<const BasicResult_deprecated*>(s);
        if (self->isOk()) return {};
        if (self->internalResult_) return ln::toString(self->err_v) + U"\n" + self->internalResult_->toString();
        return ln::toString(self->err_v);
    }

private:
    //BasicResult_deprecated()
    //    : ResultBase_deprecated(toStringInternal) {}

    explicit BasicResult_deprecated(BasicResult_deprecated&& other, bool)
        : ResultBase_deprecated(toStringInternal)
        , ok_(other.ok_)
        , err_v(std::move(other.err_v))
        , internalResult_(std::move(other.internalResult_)) {
    }

    bool ok_;
    E err_v;
    std::unique_ptr<ResultBase_deprecated> internalResult_;
};


//==============================================================================
// In library common resut


/** @see https://github.com/LuminoEngine/Lumino/wiki/ErrorHandling */
using ResultV_deprecated = BasicResult_deprecated<void, ErrorCode>;

/** @see https://github.com/LuminoEngine/Lumino/wiki/ErrorHandling
 *
 * Lumino の Result 型は、関数が少なくとも何らかの復帰可能なエラーを返すことがあるのを示すためのものです。
 * これはいくつかの言語で取り入れられている Resut<T, E> と同様のものです。
 * 
 * Lumino の API は次のようなケースで Result を積極的に使います。
 * - 成否を示す bool 戻り値の代替。
 * - nullable な結果をマークする。
 * 
 * 例えば次のコードは、関数が失敗し、戻り値となる Bitmap のインスタンスが生成されないことを示しています。
 * 
 * ```
 * Result<Bitmap*> createBitmap(String filePath);
 * ```
 * 
 * 一方、次のコードは、有効な Bitmap のインスタンスを返すものと考えて差し支えありません。(ドキュメントコメントで触れられていなければ)
 * 
 * ```
 * Bitmap* getBuiltinBitmap();
 * ```
 * 
 * コンセプト
 * ----------
 * 
 * この Result 型は Lumino のエラーハンドリングのコンセプトに近づくような設計と、ユースケースを想定しています。
 * Lumino の内部では積極的にエラーチェックを行いますが、エラーを検出した場合は
 * 「ログを出力」しつつ、「なにもしない」あるいは「デフォルトと想定される動作を行う」のが基本です。
 * このとき Result に格納されるエラーメッセージは、既に Logger に流れていることがほとんどであり、
 * そのようなケースでは、 Result にエラーメッセージを格納しないこともあります。
 * 
 * Lumino は、ロギングにおいて多少ライブラリが出過ぎるとしても、リリースしたアプリの不具合調査に役立つサポートを行うべきと考えています。
 * Logger を一切使用せず、Result のみでエラーメッセージを持ちまわることもできますが、それはロギングの責任をライブラリユーザーに持たせるということです。
 * しかし C++ では関数の呼び出し側に対して Result のチェックを強制するような仕組みは無い (nodiscard 属性などで警告はできますが) ため、
 * ライブラリユーザーは警告に目を光らせ、注意深くコードレビューを行う必要が出てきます。このような厳格な UX は Lumino の目指すところではありません。
 * 
 * より一般的な Result 型が必要な場合、
 * [std::expected](https://cpprefjp.github.io/reference/expected/expected.html) や
 * [outcome](https://github.com/ned14/outcome) が役に立ちます。
 * 
 * 将来的にこれらのライブラリに置き換えることは考えていません。
 * 型の機能以上に、 Lumino の Result を返す関数は上記のようなコンセプトでエラー処理を行っていることを示したいからです。
 */
template<class T = void>
using Result_deprecated = BasicResult_deprecated<T, ErrorCode>;

template<class T>
using GenericResult_deprecated = BasicResult_deprecated<T, ErrorCode>;

using MaybeResult_deprecated = BasicResult_deprecated<void, ErrorCode>;

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
inline String toString(const BasicResult_deprecated<TResultValue, TResultError>& e) {
    return e.toString();
}

namespace detail {

template<class T>
inline tl::unexpected<ErrorCode> toError(const tl::expected<T, ErrorCode>& result) {
    return tl::make_unexpected(result.error());
}
template<class T>
inline tl::unexpected<ErrorCode> toError(const BasicResult_deprecated<T, ErrorCode>& result) {
    return tl::make_unexpected(result.unwrapErr());
}

} // namespace detail

} // namespace ln

#define LN_DEFINE_RESULT_ALIAS \
    using Result = ln::BasicResult_deprecated<void, ln::ErrorCode>; \
    template<typename... Args> \
    auto ok(Args&&... args)->decltype(ln::ok(std::forward<Args>(args)...)) { return ln::ok(std::forward<Args>(args)...); } \
    template<typename... Args> \
    auto err(Args&&... args)->decltype(ln::err(std::forward<Args>(args)...)) { return ln::err(std::forward<Args>(args)...); } \


