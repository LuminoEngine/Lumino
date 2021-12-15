/*
エラー処理の方針 (テスト運用中)
========

### 例外 (LN_REQUIRE 系マクロ)
前提条件のミスで使用する。プログラマが悪い系のエラー。
一応例外を発生させているが、assert に直してもユーザーストーリーとしては影響ないようなものに使う。

### Result 戻り値
入力された引数やデータが悪い系のエラーで使用する。
いわゆるビジネスエラー。

return Result::Fail; する前に、原因となった個所では必ず CLI::error() して問題を通知する。
※関数を使う側としては、↑のようにログが出て失敗する可能性があることを知ることができるようにする。bool はこの目的では使わない。


### 将来的な共有モジュール化
Project とかあたりは、今後作成予定の GUI ツールでも共有したい。
で、多分 Qt Widgets 使うことになる。が、これは例外中立であって例外安全ではない。
例外したら基本的にアプリ終了、が良い流れ。
http://doc.qt.io/qt-5/exceptionsafety.html
イベントハンドラの中で、必ず、キャンセルするのか再試行させるのか判断しなければならない。

*/

#pragma once

namespace ln {
//
//enum class ResultCode
//{
//	Fail,
//	Success,
//};
//
//class Result
//{
//public:
//	static const Result Fail;
//	static const Result Success;
//
//    Result(bool result)
//        : m_code(result ? ResultCode::Success : ResultCode::Fail)
//    {}
//
//	Result(ResultCode code)
//		: m_code(code)
//	{}
//
//	Result(const Result& other)
//		: m_code(other.m_code)
//	{}
//
//	Result& operator=(const Result& other)
//	{
//		m_code = other.m_code;
//		return *this;
//	}
//
//	operator bool() const { return m_code != ResultCode::Fail; }
//
//private:
//	ResultCode m_code;
//};

namespace detail {
template<typename T>
struct Ok {
    Ok(const T& val)
        : val(val) {}
    Ok(T&& val)
        : val(std::move(val)) {}

    T val;
};

template<>
struct Ok<void> {};

template<typename E>
struct Err {
    Err(const E& val)
        : val(val) {}
    Err(E&& val)
        : val(std::move(val)) {}

    E val;
};

struct ok_tag {};
struct err_tag {};

template<typename T, typename E>
struct Storage {
    static constexpr size_t Size = sizeof(T) > sizeof(E) ? sizeof(T) : sizeof(E);
    static constexpr size_t Align = sizeof(T) > sizeof(E) ? alignof(T) : alignof(E);

    typedef typename std::aligned_storage<Size, Align>::type type;

    Storage()
        : initialized_(false) {}

    void construct(detail::Ok<T> ok) {
        new (&storage_) T(ok.val);
        initialized_ = true;
    }
    void construct(detail::Err<E> err) {
        new (&storage_) E(err.val);
        initialized_ = true;
    }

    template<typename U>
    void rawConstruct(U&& val) {
        typedef typename std::decay<U>::type CleanU;

        new (&storage_) CleanU(std::forward<U>(val));
        initialized_ = true;
    }

    template<typename U>
    const U& get() const {
        return *reinterpret_cast<const U*>(&storage_);
    }

    template<typename U>
    U& get() {
        return *reinterpret_cast<U*>(&storage_);
    }

    void destroy(ok_tag) {
        if (initialized_) {
            get<T>().~T();
            initialized_ = false;
        }
    }

    void destroy(err_tag) {
        if (initialized_) {
            get<E>().~E();
            initialized_ = false;
        }
    }

    type storage_;
    bool initialized_;
};

template<typename E>
struct Storage<void, E> {
    typedef typename std::aligned_storage<sizeof(E), alignof(E)>::type type;

    void construct(detail::Ok<void>) {
        initialized_ = true;
    }

    void construct(detail::Err<E> err) {
        new (&storage_) E(err.val);
        initialized_ = true;
    }

    template<typename U>
    void rawConstruct(U&& val) {
        typedef typename std::decay<U>::type CleanU;

        new (&storage_) CleanU(std::forward<U>(val));
        initialized_ = true;
    }

    void destroy(ok_tag) { initialized_ = false; }
    void destroy(err_tag) {
        if (initialized_) {
            get<E>().~E();
            initialized_ = false;
        }
    }

    template<typename U>
    const U& get() const {
        return *reinterpret_cast<const U*>(&storage_);
    }

    template<typename U>
    U& get() {
        return *reinterpret_cast<U*>(&storage_);
    }

    type storage_;
    bool initialized_;
};

template<typename T, typename E>
struct Constructor {

    static void move(Storage<T, E>&& src, Storage<T, E>& dst, ok_tag) {
        dst.rawConstruct(std::move(src.template get<T>()));
        src.destroy(ok_tag());
    }

    static void copy(const Storage<T, E>& src, Storage<T, E>& dst, ok_tag) {
        dst.rawConstruct(src.template get<T>());
    }

    static void move(Storage<T, E>&& src, Storage<T, E>& dst, err_tag) {
        dst.rawConstruct(std::move(src.template get<E>()));
        src.destroy(err_tag());
    }

    static void copy(const Storage<T, E>& src, Storage<T, E>& dst, err_tag) {
        dst.rawConstruct(src.template get<E>());
    }
};

template<typename E>
struct Constructor<void, E> {
    static void move(Storage<void, E>&& src, Storage<void, E>& dst, ok_tag) {
    }

    static void copy(const Storage<void, E>& src, Storage<void, E>& dst, ok_tag) {
    }

    static void move(Storage<void, E>&& src, Storage<void, E>& dst, err_tag) {
        dst.rawConstruct(std::move(src.template get<E>()));
        src.destroy(err_tag());
    }

    static void copy(const Storage<void, E>& src, Storage<void, E>& dst, err_tag) {
        dst.rawConstruct(src.template get<E>());
    }
};

} // namespace detail


template<typename T, typename CleanT = typename std::decay<T>::type>
detail::Ok<CleanT> Ok(T&& val) {
    return detail::Ok<CleanT>(std::forward<T>(val));
}

inline detail::Ok<void> Ok() {
    return detail::Ok<void>();
}

template<typename E, typename CleanE = typename std::decay<E>::type>
detail::Err<CleanE> Err(E&& val) {
    return detail::Err<CleanE>(std::forward<E>(val));
}

template<typename T, typename E>
struct Result {
    //static_assert(!std::is_same<E, void>::value, "void error type is not allowed");

    typedef detail::Storage<T, E> storage_type;

    Result(detail::Ok<T> ok)
        : ok_(true) {
        storage_.construct(std::move(ok));
    }

    Result(detail::Err<E> err)
        : ok_(false) {
        storage_.construct(std::move(err));
    }

    Result(Result&& other) {
        if (other.isOk()) {
            detail::Constructor<T, E>::move(std::move(other.storage_), storage_, detail::ok_tag());
            ok_ = true;
        } else {
            detail::Constructor<T, E>::move(std::move(other.storage_), storage_, detail::err_tag());
            ok_ = false;
        }
    }

    Result(const Result& other) {
        if (other.isOk()) {
            detail::Constructor<T, E>::copy(other.storage_, storage_, detail::ok_tag());
            ok_ = true;
        } else {
            detail::Constructor<T, E>::copy(other.storage_, storage_, detail::err_tag());
            ok_ = false;
        }
    }

    ~Result() {
        if (ok_)
            storage_.destroy(detail::ok_tag());
        else
            storage_.destroy(detail::err_tag());
    }

    bool isOk() const {
        return ok_;
    }

    bool isErr() const {
        return !ok_;
    }

    T expect(const char* str) const {
        if (!isOk()) {
            std::fprintf(stderr, "%s\n", str);
            std::terminate();
        }
        return expect_impl(std::is_same<T, void>());
    }

    template<typename Func, typename Ret = Result<typename detail::ResultOkType<typename detail::result_of<Func>::type>::type, E>>
    Ret map(Func func) const {
        return detail::map(*this, func);
    }

    template<typename Func, typename Ret = Result<T, typename detail::ResultErrType<typename detail::result_of<Func>::type>::type>>
    Ret mapError(Func func) const {
        return detail::mapError(*this, func);
    }

    template<typename Func>
    Result<T, E> then(Func func) const {
        return detail::then(*this, func);
    }

    template<typename Func>
    Result<T, E> otherwise(Func func) const {
        return detail::otherwise(*this, func);
    }

    template<typename Func, typename Ret = Result<T, typename detail::ResultErrType<typename detail::result_of<Func>::type>::type>>
    Ret orElse(Func func) const {
        return detail::orElse(*this, func);
    }

    storage_type& storage() {
        return storage_;
    }

    const storage_type& storage() const {
        return storage_;
    }

    template<typename U = T>
    typename std::enable_if<
        !std::is_same<U, void>::value,
        U>::type
    unwrapOr(const U& defaultValue) const {
        if (isOk()) {
            return storage().template get<U>();
        }
        return defaultValue;
    }

    template<typename U = T>
    typename std::enable_if<
        !std::is_same<U, void>::value,
        U>::type
    unwrap() const {
        if (isOk()) {
            return storage().template get<U>();
        }

        std::fprintf(stderr, "Attempting to unwrap an error Result\n");
        std::terminate();
    }

    E unwrapErr() const {
        if (isErr()) {
            return storage().template get<E>();
        }

        std::fprintf(stderr, "Attempting to unwrapErr an ok Result\n");
        std::terminate();
    }

private:
    T expect_impl(std::true_type) const {}
    T expect_impl(std::false_type) const { return storage_.template get<T>(); }

    bool ok_;
    storage_type storage_;
};

} // namespace ln
