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


template<typename T>
struct Ok<T&> {
    Ok(T& val)
        : val(val) {}
    T& val;
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
//
//template<typename E>
//struct Storage<void, E> {
//    typedef typename std::aligned_storage<sizeof(E), alignof(E)>::type type;
//
//    void construct(detail::Ok<void>) {
//        initialized_ = true;
//    }
//
//    void construct(detail::Err<E> err) {
//        new (&storage_) E(err.val);
//        initialized_ = true;
//    }
//
//    template<typename U>
//    void rawConstruct(U&& val) {
//        typedef typename std::decay<U>::type CleanU;
//
//        new (&storage_) CleanU(std::forward<U>(val));
//        initialized_ = true;
//    }
//
//    void destroy(ok_tag) { initialized_ = false; }
//    void destroy(err_tag) {
//        if (initialized_) {
//            get<E>().~E();
//            initialized_ = false;
//        }
//    }
//
//    template<typename U>
//    const U& get() const {
//        return *reinterpret_cast<const U*>(&storage_);
//    }
//
//    template<typename U>
//    U& get() {
//        return *reinterpret_cast<U*>(&storage_);
//    }
//
//    type storage_;
//    bool initialized_;
//};

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


//template<typename T, typename CleanT = typename std::decay<T>::type>
//detail::Ok<CleanT> Ok(T&& val) {
//    return detail::Ok<CleanT>(std::forward<T>(val));
//}
template<typename T>
detail::Ok<T> Ok(T&& val) {
    return detail::Ok<T>(std::forward<T>(val));
}
//detail::Ok<void> Ok() {
//    return detail::Ok<void>();
//}

inline detail::Ok<void> Ok() {
    return detail::Ok<void>();
}

template<typename E, typename CleanE = typename std::decay<E>::type>
detail::Err<CleanE> Err(E&& val) {
    return detail::Err<CleanE>(std::forward<E>(val));
}

inline detail::Err<bool> Err() {
    return detail::Err<bool>(false);
}


template<typename T, typename E = bool>
struct Result {
    Result(T&& ok)
        : ok_(true)
        , ok_v(std::forward<T>(ok)) {
    }

   Result(detail::Ok<T> ok)
        : ok_(true)
        , ok_v(std::move(ok.val)) {
    }

    Result(detail::Err<E> err)
        : ok_(false)
        , err_v(std::move(err.val)) {
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
    Result(detail::Ok<T&> ok)
        : ok_(true)
        , ok_v(&ok.val) {
    }

    Result(detail::Err<E> err)
        : ok_(false)
        , err_v(std::move(err.val)) {
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

private:
    bool ok_;
    union {
        T* ok_v;
        E err_v;
    };
};

template<typename E>
struct Result<void, E> {
    Result(detail::Ok<void> ok)
        : ok_(true) {
    }

    Result(detail::Err<E> err)
        : ok_(false)
        , err_v(std::move(err.val)) {
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

private:
    bool ok_;
    E err_v;
};

} // namespace ln
