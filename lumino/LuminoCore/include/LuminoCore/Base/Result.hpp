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
class [[nodiscard]] BasicResult : public ResultBase {
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
using ResultV = BasicResult<void, ErrorCode>;

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
using Result = BasicResult<T, ErrorCode>;

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
