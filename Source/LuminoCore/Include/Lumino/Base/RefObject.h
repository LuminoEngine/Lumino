
#pragma once
#include <atomic>
#include "../Threading/AtomicCounter.h"

LN_NAMESPACE_BEGIN
class RefObject;
namespace tr { class ReflectionHelper; }

/// 参照カウントのインクリメント
#ifndef LN_SAFE_ADDREF
	#define LN_SAFE_ADDREF( p ) { if ( p ) { (p)->AddRef(); } }
#endif

/// 参照カウントのデクリメント
#ifndef LN_SAFE_RELEASE
	#define LN_SAFE_RELEASE( p ) { if ( p ) { (p)->Release(); (p)= nullptr; } }
#endif

/// a に b を格納するユーティリティ
#define LN_REFOBJ_SET( a, b ) \
{ \
	if (a != b) \
	{ \
		LN_SAFE_ADDREF(b); \
		if (a) (a)->Release(); \
		(a) = (b); \
	} \
}


/**
	@brief	参照カウントを持つクラスの基底
*/
class RefObject
{
protected:
	RefObject();
	virtual ~RefObject();

public:

	/** 参照カウントを取得します。*/
	virtual int32_t GetReferenceCount() const;

	/** 参照カウントをインクリメントします。*/
	virtual int32_t AddRef();

	/** 参照カウントをデクリメントします。*/
	virtual int32_t Release();

protected:
    Atomic					m_referenceCount;	///< 参照カウント	TODO: atomic<> の方が高速

private:
	LN_DISALLOW_COPY_AND_ASSIGN(RefObject);

	void ReleaseInternal();

	std::atomic<int32_t>	m_internalReferenceCount;

	friend class tr::ReflectionHelper;
};

//namespace detail
//{
class RefPtrBase {};

//} // namespace detail

/**
	@brief		RefObject 用 スマートポインタ
*/
template <class T>
class RefPtr final : public /*detail::*/RefPtrBase
{
public:
	typedef T* PtrType;

public:

	template<typename... TArgs>
	static RefPtr<T> MakeRef(TArgs... args)
	{
		return RefPtr<T>(LN_NEW T(args...), false);
	}
    
    template<class T2>
    static RefPtr<T> StaticCast(const RefPtr<T2>& other)
    {
        T* ptr = static_cast<T*>(other.Get());
        return RefPtr<T>(ptr, true);
    }

	/** コンストラクタ */
	RefPtr();

	/**
		@brief		コンストラクタ
		@param[in]	ptr		: 管理対象としてセットする ReferenceObject インスタンスのポインタ
		@param[in]	addRef	: true の場合、セットされた ReferenceObject の参照カウントをインクリメントする
	*/
	RefPtr(T* ptr, bool addRef = true);

	/** コピーコンストラクタ */
	RefPtr(const RefPtr<T>& obj);

	/** デストラクタ */
	~RefPtr();

public:

	/**
		@brief		ReferenceObject インスタンスのポインタを管理対象としてセットする
		@param[in]	ptr		: 管理対象としてセットする ReferenceObject インスタンスのポインタ
		@param[in]	addRef	: true の場合、セットされた ReferenceObject の参照カウントをインクリメントする
	*/
	void Attach(T* ptr, bool addRef = false)
    {
		if (ptr == m_ptr) return;
        SafeRelease();
		m_ptr = ptr;
		if (addRef) SafeAddRef();
    }

	/**
		@brief		管理対象オブジェクトの参照カウントをインクリメントする
	*/
	void SafeAddRef()
	{ 
		LN_SAFE_ADDREF(m_ptr);
	}

	/**
		@brief		管理対象オブジェクトの参照カウントをデクリメントし、管理対象から外す
	*/
    void SafeRelease()
	{
		LN_SAFE_RELEASE(m_ptr);
	}

	T* DetachMove()
	{
		RefObject* ptr = m_ptr;
		m_ptr = nullptr;
		return static_cast<T*>(ptr);
	}

	/**
		@brief		管理対象オブジェクトへのポインタが nullptr であるかを確認する
	*/
	bool IsNull() const { return (m_ptr == nullptr); }

	/**
		@brief		管理対象オブジェクトへのポインタを取得する
	*/
    T* Get() const	{ return static_cast<T*>(m_ptr); }


	/// operator=
	RefPtr<T>& operator = (const RefPtr<T>& ptr)
	{
		LN_REFOBJ_SET(m_ptr, ptr.m_ptr);
		return *this;
	}

	/// operator=
	RefPtr<T>& operator = (T* ptr)
	{
		LN_REFOBJ_SET(m_ptr, ptr);
		return *this;
	}

	/// operator!
    bool operator ! () const { return (m_ptr == nullptr); }
    
    /// operator== 
    //bool operator == (std::nullptr_t ptr) const { return (mPtr == ptr); }
    //bool operator == (const T* ptr) const { return (mPtr == ptr); }
	//bool operator == (T* ptr) { return (mPtr == ptr); }
	//bool operator == (const RefPtr<T>& ptr) const { return (mPtr == ptr.mPtr); }

    /// operator!=
    //bool operator != ( const T* ptr ) const { return ( mPtr != ptr ); }
	//bool operator != (std::nullptr_t ptr) const { return (mPtr != ptr); }
	//bool operator != (const T* ptr) const { return (mPtr != ptr); }
	//bool operator != (const RefPtr<T>& ptr) const { return (mPtr != ptr.mPtr); }

    // operator<
    bool operator < (const T* ptr) const { return m_ptr < ptr; }

	/// operator*
    T& operator* ()
    {
        LN_ASSERT(m_ptr != nullptr);
        return *static_cast<T*>(m_ptr);
    }

	/// operator*
    const T& operator* () const
    {
        LN_ASSERT(m_ptr != nullptr);
        return *static_cast<T*>(m_ptr);
	}

	/// ->
    T* operator -> () const
    {
        LN_ASSERT(m_ptr != nullptr );
        return static_cast<T*>(m_ptr);
    }

    /// convert
	operator T*			() const { return static_cast<T*>(m_ptr); }
    //operator const T*	() const { return static_cast<T*>(m_ptr); }

protected:
	RefObject* m_ptr;
};


//------------------------------------------------------------------------------
template<typename T>
RefPtr<T>::RefPtr()
	: m_ptr(nullptr)
{}

//------------------------------------------------------------------------------
template<typename T>
RefPtr<T>::RefPtr(T* ptr, bool addRef)
	: m_ptr(ptr)
{
	if (addRef)
	{
		LN_SAFE_ADDREF(m_ptr);
	}
}

//------------------------------------------------------------------------------
template<typename T>
RefPtr<T>::RefPtr(const RefPtr<T>& obj)
	: m_ptr(obj.m_ptr)
{
	LN_SAFE_ADDREF(m_ptr);
}

//------------------------------------------------------------------------------
template<typename T>
RefPtr<T>::~RefPtr()
{
	LN_SAFE_RELEASE(m_ptr);
}

//------------------------------------------------------------------------------
template<typename T1, typename T2>
bool operator==(const RefPtr<T1>& left, const RefPtr<T2>& right) LN_NOEXCEPT
{
	return (left.Get() == right.Get());
}

//------------------------------------------------------------------------------
template<typename T>
bool operator==(std::nullptr_t left, const RefPtr<T>& right) LN_NOEXCEPT
{
	return ((T*)0 == right.Get());
}

//------------------------------------------------------------------------------
template<typename T>
bool operator==(const RefPtr<T>& left, std::nullptr_t right) LN_NOEXCEPT
{
	return (left.Get() == (T*)0);
}

//------------------------------------------------------------------------------
template<typename T1, typename T2>
bool operator!=(const RefPtr<T1>& left, const RefPtr<T2>& right) LN_NOEXCEPT
{
	return (left.Get() != right.Get());
}

//------------------------------------------------------------------------------
template<typename T>
bool operator!=(std::nullptr_t left, const RefPtr<T>& right) LN_NOEXCEPT
{
	return ((T*)0 != right.Get());
}

//------------------------------------------------------------------------------
template<typename T>
bool operator!=(const RefPtr<T>& left, std::nullptr_t right) LN_NOEXCEPT
{
	return (left.Get() != (T*)0);
}

LN_NAMESPACE_END
