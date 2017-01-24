
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
	: public tr::ReflectionObject
{
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
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
	LN_TR_REFLECTION_TYPEINFO_DECLARE();
public:
	PropertyChangedEventArgs(const PropertyInfo* prop, PropertySetSource cause);
	virtual ~PropertyChangedEventArgs();

public:
	const PropertyInfo*		changedProperty;
	PropertySetSource		cause;
};

} // namespace tr
LN_NAMESPACE_END
