#include "../Internal.h"
#include "NlVariant.h"

LN_NAMESPACE_BEGIN


//==============================================================================
// NlGraphNode
//==============================================================================

//------------------------------------------------------------------------------
NlVariant::NlVariant()
	: m_type(NlVariantType::Int32)
	, m_int32(0)
{
}

//------------------------------------------------------------------------------
NlVariant::~NlVariant()
{
}

//NlVariant::NlVariant(const NlVariant& other)
//{
//
//}

LN_NAMESPACE_END

