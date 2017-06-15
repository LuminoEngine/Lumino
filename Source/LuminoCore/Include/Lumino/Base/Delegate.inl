
template < LN_DELEGATE_TEMPLATE_ARGS >
class LN_DELEGATE_CLASS_NAME
{
private:
	/// 関数ポインタを保持する内部クラスの基底
	class DelegateHolderBase
		: public RefObject
	{
	public:
		virtual void call(LN_DELEGATE_ARGS_DECL) const = 0;
		virtual bool equals(const DelegateHolderBase* p) const = 0;	// pの実態は サブクラスと同じ型であることが前提
	};

	/// DelegateHolderStatic (static 関数呼び出し時の実体)
	class DelegateHolderStatic
		: public DelegateHolderBase
	{
	private:
		typedef void (LN_STDCALL *StaticFunction)( LN_DELEGATE_TEMPLATE_TYPES );

	private:
		StaticFunction	mFunction;

	public:
		DelegateHolderStatic( StaticFunction function )
			: mFunction	( function )
		{}

		virtual void call( LN_DELEGATE_ARGS_DECL ) const
		{
			mFunction( LN_DELEGATE_CALL_ARGS );
		}

		virtual bool equals(const DelegateHolderBase* p) const
		{
			return (mFunction == static_cast<const DelegateHolderStatic*>(p)->mFunction);
		}
	};

	/// DelegateHolderDynamics (メンバ関数呼び出し時の実体)
	template < typename T >
	class DelegateHolderDynamics
		: public DelegateHolderBase
	{
	private:
		typedef void (T::*ClassMethod)( LN_DELEGATE_ARGS_DECL );

	private:
		T*			mObjPtr;
		ClassMethod	mMethod;

	public:
		DelegateHolderDynamics( T* objPtr, ClassMethod method )
			: mObjPtr		( objPtr )
			, mMethod		( method )
		{}

		virtual void call( LN_DELEGATE_ARGS_DECL ) const
		{
			(mObjPtr->*mMethod)( LN_DELEGATE_CALL_ARGS );
		}

		virtual bool equals(const DelegateHolderBase* p) const
		{
			return (mObjPtr == static_cast<const DelegateHolderDynamics*>(p)->mObjPtr &&
				mMethod == static_cast<const DelegateHolderDynamics*>(p)->mMethod);
		}
	};

private:
	DelegateHolderBase*		mDelegate;
	bool				m_isStatic;		///< Equals 高速化のため

public:
	LN_DELEGATE_CLASS_NAME()
		: mDelegate(NULL)
		, m_isStatic(false)
	{
	}

	LN_DELEGATE_CLASS_NAME( const LN_DELEGATE_CLASS_NAME& value )
		: mDelegate(NULL)
		, m_isStatic(false)
	{
		*this = value;
	}

	template < typename T >
	LN_DELEGATE_CLASS_NAME( T* objPtr, void (T::*method)(LN_DELEGATE_ARGS_DECL) )
		: mDelegate(new DelegateHolderDynamics<T>(objPtr, method))
		, m_isStatic(false)
	{
	}

	LN_DELEGATE_CLASS_NAME( void (LN_STDCALL *function)(LN_DELEGATE_ARGS_DECL) )
		: mDelegate(new DelegateHolderStatic(function))
		, m_isStatic(true)
	{
	}

	~LN_DELEGATE_CLASS_NAME()
	{
		reset();
	}

public:
	void reset()
	{
		LN_SAFE_RELEASE( mDelegate );
	}

	bool isEmpty() const
	{
		return mDelegate == NULL;
	}

	void call( LN_DELEGATE_ARGS_DECL ) const
	{
		if ( mDelegate != NULL )
		{
			mDelegate->call( LN_DELEGATE_CALL_ARGS );
		}
	}

	bool equals(const LN_DELEGATE_CLASS_NAME& obj) const
	{
		if (m_isStatic != obj.m_isStatic) { return false; }
		if (mDelegate == NULL &&  obj.mDelegate == NULL) { return true; }
		//if (IsEmpty() && obj.IsEmpty()) { return true; }

		if (mDelegate != NULL && obj.mDelegate != NULL)
		{
			return mDelegate->equals(obj.mDelegate);
			/*
			if (m_isStatic) { 
				return static_cast<DelegateHolderStatic*>(mDelegate)->Equals(*static_cast<DelegateHolderStatic*>(obj.mDelegate));
			}
			else {
				return static_cast<DelegateHolderDynamics<T>*>(mDelegate)->Equals(*static_cast<DelegateHolderDynamics<T>*>(obj.mDelegate));
			}*/
		}
		else {
			// this か obj 一方が NULL で、もう一方が 非NULL であればここに来る。
			return false;
		}
	}

	void operator ()( LN_DELEGATE_ARGS_DECL ) const
	{
		call( LN_DELEGATE_CALL_ARGS );
	}

	LN_DELEGATE_CLASS_NAME& operator = (const LN_DELEGATE_CLASS_NAME& obj)
	{
		reset();
		m_isStatic = obj.m_isStatic;
		mDelegate = obj.mDelegate;
		if ( mDelegate != NULL )
			mDelegate->addRef();

		return *this;
	}

	bool operator == (const LN_DELEGATE_CLASS_NAME& obj) const { return equals(obj); }
	bool operator != (const LN_DELEGATE_CLASS_NAME& obj) const { return !equals(obj); }
};

template < typename T, LN_DELEGATE_TEMPLATE_ARGS >
LN_DELEGATE_CLASS_NAME< LN_DELEGATE_TEMPLATE_TYPES > LN_CreateDelegate( T* objPtr, void (T::*method)(LN_DELEGATE_TEMPLATE_TYPES) )
{
	return LN_DELEGATE_CLASS_NAME< LN_DELEGATE_TEMPLATE_TYPES >( objPtr, method );
}

template < LN_DELEGATE_TEMPLATE_ARGS >
LN_DELEGATE_CLASS_NAME< LN_DELEGATE_TEMPLATE_TYPES > LN_CreateDelegate( void (LN_STDCALL *method)(LN_DELEGATE_TEMPLATE_TYPES) )
{
	return LN_DELEGATE_CLASS_NAME< LN_DELEGATE_TEMPLATE_TYPES >( method );
}

#undef LN_DELEGATE_CLASS_NAME
#undef LN_DELEGATE_TEMPLATE_ARGS
#undef LN_DELEGATE_ARGS_DECL
#undef LN_DELEGATE_CALL_ARGS
#undef LN_DELEGATE_TEMPLATE_TYPES
