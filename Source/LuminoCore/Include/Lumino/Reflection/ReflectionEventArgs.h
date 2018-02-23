
#pragma once
#include "Common.h"
#include "ReflectionObject.h"

LN_NAMESPACE_BEGIN
namespace tr
{
	
/**
	@brief		
*/
class ReflectionEventArgs
	: public Object
{
	LN_OBJECT;
public:
	ReflectionEventArgs();
	virtual ~ReflectionEventArgs();

public:
	bool	handled;
};

/**
	@brief	PropertyChanged イベントの引数です。
*/
class PropertyChangedEventArgs
	: public ReflectionEventArgs
{
	//LN_OBJECT;
public:
	PropertyChangedEventArgs(const PropertyInfo* prop, PropertySetSource cause);
	virtual ~PropertyChangedEventArgs();

public:
	const PropertyInfo*		changedProperty;
	PropertySetSource		cause;
};

} // namespace tr
LN_NAMESPACE_END
