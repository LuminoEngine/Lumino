#pragma once

namespace ln {
class TypeInfo;

#define LN_OBJECT
#ifndef LN_CONSTRUCT_ACCESS
#define LN_CONSTRUCT_ACCESS \
		template<class T, typename... TArgs> friend ln::Ref<T> ln::newObject(TArgs&&... args); \
		template<class T, typename... TArgs> friend void ln::placementNewObject(void* ptr, TArgs&&... args); \
		protected
#endif

template<class T, typename... TArgs>
Ref<T> newObject(TArgs&&... args)
{
	auto ptr = Ref<T>(new T(), false);
	ptr->initialize(std::forward<TArgs>(args)...);
	return ptr;
}

template<class T, typename... TArgs>
void placementNewObject(void* ptr, TArgs&&... args)
{
	new (ptr)T();
	static_cast<T*>(ptr)->initialize(std::forward<TArgs>(args)...);
}

class LN_API Object
	: public RefObject
{
LN_CONSTRUCT_ACCESS:
	Object();
	void initialize();

protected:
	virtual ~Object();
	virtual void finalize() override;

public:
	/**
	 * このオブジェクトが保持しているリソースを開放します。
	 *
	 * このメソッドはフレームワークから呼び出されます。通常、明示的に呼び出す必要はありません。
	 * 派生クラスで何らかのリソースを開放する必要がある場合は finalize ではなく dispose を実装して開放してください。
	 *
	 * @attention このメソッドは virtual です。RAII の実装を目的としてデストラクタで呼び出すことはできません。代わりに finalize() からコールされます。
	 */
	virtual void dispose();

private:
    virtual TypeInfo* _lnref_getThisTypeInfo() const;
    friend TypeInfo;
};

class ObjectHelper
{
public:
	static void dispose(Object* obj) { if (obj) obj->dispose(); }
};

} // namespace ln

