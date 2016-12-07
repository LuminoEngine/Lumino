#include "../Internal.h"
#include "NlVariant.h"
#include "NlFunctionNodes.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// NlNode_Add
//==============================================================================

//------------------------------------------------------------------------------
NlNode_Add::NlNode_Add()
{
	m_outputValue = RefPtr<NlVariant>::MakeRef();
	m_inputLhsPin = CreatePin(NlGraphPinCategory::DataFlow, NlGraphPinDirection::Input, _T("left"));
	m_inputRhsPin = CreatePin(NlGraphPinCategory::DataFlow, NlGraphPinDirection::Input, _T("right"));
	m_outputPin = CreatePin(NlGraphPinCategory::DataFlow, NlGraphPinDirection::Output, _T("return"));
}

//------------------------------------------------------------------------------
void NlNode_Add::Execute(NlContext* sc)
{
	//NlVariant* lhs = sc->Evaluate(m_lhs);
	//NlVariant* rhs = sc->Evaluate(m_rhs);
}

LN_NAMESPACE_END

