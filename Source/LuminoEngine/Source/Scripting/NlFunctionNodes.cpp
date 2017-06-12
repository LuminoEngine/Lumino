#include "../Internal.h"
#include "NlVariant.h"
#include "NlFunctionNodes.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// NlNode_Add
//==============================================================================

static void NlVariant_AddInt32(NlVariant* lhs, NlVariant* rhs, NlVariant* outResult)
{
	outResult->SetValue(lhs->getValue<int32_t>() + rhs->getValue<int32_t>());
}

using OpFunc = void(NlVariant* lhs, NlVariant* rhs, NlVariant* outResult);

//static OpFunc g_opOpFuncs[][]
//{
//
//};

//------------------------------------------------------------------------------
NlNode_Add::NlNode_Add()
{
	m_inputLhsPin = CreatePin(NlGraphPinCategory::DataFlow, NlGraphPinDirection::Input, _T("left"));
	m_inputRhsPin = CreatePin(NlGraphPinCategory::DataFlow, NlGraphPinDirection::Input, _T("right"));
	m_outputPin = CreatePin(NlGraphPinCategory::DataFlow, NlGraphPinDirection::Output, _T("return"));
}

//------------------------------------------------------------------------------
void NlNode_Add::execute(NlContext* sc)
{
	NlVariant* lhs = sc->Evaluate(m_inputLhsPin);
	NlVariant* rhs = sc->Evaluate(m_inputRhsPin);
	NlVariant_AddInt32(lhs, rhs, m_outputPin->GetValueCache());
}

LN_NAMESPACE_END

