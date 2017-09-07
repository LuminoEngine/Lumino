#include "../Internal.h"
#include "NlVariant.h"
#include "NlFunctionNodes.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// NlNode_Add
//==============================================================================

static void NlVariant_AddInt32(NlVariant* lhs, NlVariant* rhs, NlVariant* outResult)
{
	outResult->setValue(lhs->getValue<int32_t>() + rhs->getValue<int32_t>());
}

using OpFunc = void(NlVariant* lhs, NlVariant* rhs, NlVariant* outResult);

//static OpFunc g_opOpFuncs[][]
//{
//
//};

//------------------------------------------------------------------------------
NlNode_Add::NlNode_Add()
{
	m_inputLhsPin = createPin(NlGraphPinCategory::DataFlow, NlGraphPinDirection::Input, _LT("left"));
	m_inputRhsPin = createPin(NlGraphPinCategory::DataFlow, NlGraphPinDirection::Input, _LT("right"));
	m_outputPin = createPin(NlGraphPinCategory::DataFlow, NlGraphPinDirection::Output, _LT("return"));
}

//------------------------------------------------------------------------------
void NlNode_Add::execute(NlContext* sc)
{
	NlVariant* lhs = sc->evaluate(m_inputLhsPin);
	NlVariant* rhs = sc->evaluate(m_inputRhsPin);
	NlVariant_AddInt32(lhs, rhs, m_outputPin->getValueCache());
}

LN_NAMESPACE_END

