
#pragma once
#include <atomic>
#include <cassert>
#include "Common.hpp"

namespace ln {
class RefObjectHelper;
template <class T> class Ref;

// increment reference count
#ifndef LN_SAFE_RETAIN
#define LN_SAFE_RETAIN( p ) { if ( p ) { (p)->retain(); } }
#endif

// decrement reference count
#ifndef LN_SAFE_RELEASE
#define LN_SAFE_RELEASE( p ) { if ( p ) { (p)->release(); (p)= nullptr; } }
#endif

// unitiliy to store b -> a
#define LN_REFOBJ_SET( a, b ) \
{ \
	if (a != b) \
	{ \
		LN_SAFE_RETAIN(b); \
		if (a) (a)->release(); \
		(a) = (b); \
	} \
}

/**
 * 参照カウントを持つオブジェクトのベースクラスです。
 *
 * 一般的なケースでは、ln::makeRef 関数によりインスタンスを作成し、RefObject 用のスマートポインタである Ref クラスを使用して参照を管理します。
 * 参照を直接操作したい場合は RefObjectHelper クラスを使用します。
 */
class RefObject
{
protected:
	RefObject();
	virtual ~RefObject();

	/** 参照がなくなり、オブジェクトが削除されようとしているときに呼び出されます。実装コードでは仮想関数を呼び出すことができます。主にデストラクタの制限を回避するために使用します。 */
	virtual void finalize();

	// experimental
	void setValidObject(bool valid = true) { m_ojectFlags = valid; }
	bool isValidObject() const { return m_ojectFlags; }

private:
	RefObject(const RefObject&) = delete;
	void operator=(const RefObject&) = delete;
	void releaseInternal();
	int32_t getReferenceCount() const;
	int32_t retain();
	int32_t release();

	std::atomic<int32_t> m_referenceCount;
	std::atomic<int32_t> m_internalReferenceCount;
	uint32_t m_ojectFlags;

	friend bool valid(const RefObject* obj);
	friend bool valid(const RefObject& obj);

	friend class RefObjectHelper;
	template<class T> friend class Ref;
};

inline bool valid(const RefObject* obj)
{
	return obj && obj->isValidObject();
}

inline bool valid(const RefObject& obj)
{
	return obj.isValidObject();
}


/** RefObject 参照を直接操作します。 */
class RefObjectHelper
{
public:
	/** 指定された RefObject の参照カウントを取得します。 */
	static int32_t getReferenceCount(RefObject* obj);

	/** 指定された RefObject の参照カウントをインクリメントします。 */
	static int32_t retain(RefObject* obj);

	/** 指定された RefObject の参照カウントをデクリメントします。0 になった場合、RefObject は削除されます。 */
	static int32_t release(RefObject* obj);
};

/** RefObject 用 スマートポインタ */
template <class T>
class Ref
{
public:
	typedef T* PtrType;

public:

	template<typename... TArgs>
	static Ref<T> makeRef(TArgs... args)
	{
		return Ref<T>(LN_NEW T(args...), false);
	}
    
    template<class T2>
    static Ref<T> staticCast(const Ref<T2>& other)
    {
        T* ptr = static_cast<T*>(other.get());
        return Ref<T>(ptr, true);
    }

	/** コンストラクタ */
	Ref();

	/**
		@brief		コンストラクタ
		@param[in]	ptr		: 管理対象としてセットする ReferenceObject インスタンスのポインタ
		@param[in]	retain	: true の場合、セットされた ReferenceObject の参照カウントをインクリメントする
	*/
	Ref(T* ptr, bool retain = true);

	template <class Y>
	Ref(const Ref<Y>& r) LN_NOEXCEPT
		: m_ptr(r.get())
	{
		LN_SAFE_RETAIN(m_ptr);
	}

	/** コピーコンストラクタ */
	Ref(const Ref<T>& obj);

	/** デストラクタ */
	~Ref();

public:

	/**
		@brief		ReferenceObject インスタンスのポインタを管理対象としてセットする
		@param[in]	ptr		: 管理対象としてセットする ReferenceObject インスタンスのポインタ
		@param[in]	retain	: true の場合、セットされた ReferenceObject の参照カウントをインクリメントする
	*/
	void attach(T* ptr, bool retain = false)
    {
		if (ptr == m_ptr) return;
        safeRelease();
		m_ptr = ptr;
		if (retain) safeAddRef();
    }

	/**
		@brief		管理対象オブジェクトの参照カウントをインクリメントする
	*/
	void safeAddRef()
	{ 
		LN_SAFE_RETAIN(m_ptr);
	}

	/**
		@brief		管理対象オブジェクトの参照カウントをデクリメントし、管理対象から外す
	*/
    void safeRelease()
	{
		LN_SAFE_RELEASE(m_ptr);
	}

	T* detachMove()
	{
		RefObject* ptr = m_ptr;
		m_ptr = nullptr;
		return static_cast<T*>(ptr);
	}

	/**
		@brief		管理対象オブジェクトへのポインタが nullptr であるかを確認する
	*/
	bool isNull() const { return (m_ptr == nullptr); }

	/**
		@brief		管理対象オブジェクトへのポインタを取得する
	*/
    T* get() const	{ return static_cast<T*>(m_ptr); }


	/// operator=
	Ref<T>& operator = (const Ref<T>& ptr)
	{
		LN_REFOBJ_SET(m_ptr, ptr.m_ptr);
		return *this;
	}

	/// operator=
	//Ref<T>& operator = (T* ptr)
	//{
	//	LN_REFOBJ_SET(m_ptr, ptr);
	//	return *this;
	//}

	/// operator!
    bool operator ! () const { return (m_ptr == nullptr); }
    
    /// operator== 
    //bool operator == (std::nullptr_t ptr) const { return (mPtr == ptr); }
    //bool operator == (const T* ptr) const { return (mPtr == ptr); }
	//bool operator == (T* ptr) { return (mPtr == ptr); }
	//bool operator == (const Ref<T>& ptr) const { return (mPtr == ptr.mPtr); }

    /// operator!=
    //bool operator != ( const T* ptr ) const { return ( mPtr != ptr ); }
	//bool operator != (std::nullptr_t ptr) const { return (mPtr != ptr); }
	//bool operator != (const T* ptr) const { return (mPtr != ptr); }
	//bool operator != (const Ref<T>& ptr) const { return (mPtr != ptr.mPtr); }

    // operator<
    bool operator < (const T* ptr) const { return m_ptr < ptr; }

	/// operator*
    T& operator* ()
    {
		assert(m_ptr != nullptr);
        return *static_cast<T*>(m_ptr);
    }

	/// operator*
    const T& operator* () const
    {
		assert(m_ptr != nullptr);
        return *static_cast<T*>(m_ptr);
	}

	/// ->
    T* operator -> () const
    {
		assert(m_ptr != nullptr );
        return static_cast<T*>(m_ptr);
    }

    /// convert
	operator T*			() const
	{
		// ここでコンパイルエラーとなる場合、T の定義があるヘッダファイルを include しているか確認すること。
		return static_cast<T*>(m_ptr);
	}
    //operator const T*	() const { return static_cast<T*>(m_ptr); }

protected:
	RefObject* m_ptr;
};


//------------------------------------------------------------------------------
template<typename T>
Ref<T>::Ref()
	: m_ptr(nullptr)
{}

//------------------------------------------------------------------------------
template<typename T>
Ref<T>::Ref(T* ptr, bool retain)
	: m_ptr(ptr)
{
	if (retain)
	{
		LN_SAFE_RETAIN(m_ptr);
	}
}

//------------------------------------------------------------------------------
template<typename T>
Ref<T>::Ref(const Ref<T>& obj)
	: m_ptr(obj.m_ptr)
{
	LN_SAFE_RETAIN(m_ptr);
}

//------------------------------------------------------------------------------
template<typename T>
Ref<T>::~Ref()
{
	LN_SAFE_RELEASE(m_ptr);
}

//------------------------------------------------------------------------------
template<typename T1, typename T2>
bool operator==(const Ref<T1>& left, const Ref<T2>& right) LN_NOEXCEPT
{
	return (left.get() == right.get());
}

//------------------------------------------------------------------------------
template<typename T>
bool operator==(std::nullptr_t left, const Ref<T>& right) LN_NOEXCEPT
{
	return ((T*)0 == right.get());
}

//------------------------------------------------------------------------------
template<typename T>
bool operator==(const Ref<T>& left, std::nullptr_t right) LN_NOEXCEPT
{
	return (left.get() == (T*)0);
}

//------------------------------------------------------------------------------
template<typename T1, typename T2>
bool operator!=(const Ref<T1>& left, const Ref<T2>& right) LN_NOEXCEPT
{
	return (left.get() != right.get());
}

//------------------------------------------------------------------------------
template<typename T>
bool operator!=(std::nullptr_t left, const Ref<T>& right) LN_NOEXCEPT
{
	return ((T*)0 != right.get());
}

//------------------------------------------------------------------------------
template<typename T>
bool operator!=(const Ref<T>& left, std::nullptr_t right) LN_NOEXCEPT
{
	return (left.get() != (T*)0);
}



template<class T, class... TArgs>
inline Ref<T> makeRef(TArgs&&... args)
{
	return Ref<T>(LN_NEW T(std::forward<TArgs>(args)...), false);
}

} // namespace ln
