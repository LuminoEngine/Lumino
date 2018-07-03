#pragma once

namespace ln {

class Object
	: public RefObject
{
protected:
	Object();
	virtual ~Object();
	virtual void finalize() override;

public:
	virtual void dispose();
};

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

#ifndef LN_CONSTRUCT_ACCESS
#define LN_CONSTRUCT_ACCESS \
		template<class T, typename... TArgs> friend ln::Ref<T> ln::newObject(TArgs&&... args); \
		template<class T, typename... TArgs> friend void ln::placementNewObject(void* ptr, TArgs&&... args); \
		protected
#endif

} // namespace ln

