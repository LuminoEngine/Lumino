
#pragma once
#include "Internal.h"
#include <Lumino/RoutedEvent.h>

namespace Lumino
{

//=============================================================================
// EventArgs
//=============================================================================

LN_CORE_OBJECT_TYPE_INFO_IMPL(EventArgs, CoreObject);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
EventArgs::EventArgs()
	: /*HandlerOwner(NULL)
	,*/ Handled(false)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
EventArgs::~EventArgs()
{
}

//=============================================================================
// PropertyChangedEventArgs
//=============================================================================

LN_CORE_OBJECT_TYPE_INFO_IMPL(PropertyChangedEventArgs, CoreObject);

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
PropertyChangedEventArgs::PropertyChangedEventArgs(const String& propertyName, const Variant& newValue)
	: PropertyName(propertyName)
	, NewValue(newValue)
{
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
PropertyChangedEventArgs::~PropertyChangedEventArgs()
{
}

} // namespace Lumino
