#pragma once
#include "ScriptingManager.h"

LN_NAMESPACE_BEGIN
class NlVariant;

/**
	@brief	
*/
class NlNode_Add
	: public NlGraphNode
{
public:
	NlNode_Add();

	virtual ~NlNode_Add() = default;

	NlGraphPin* GetDataInputLhsPin() const { return m_inputLhsPin; }
	NlGraphPin* GetDataInputRhsPin() const { return m_inputRhsPin; }
	NlGraphPin* GetDataOutputPin() const { return m_outputPin; }

protected:
	virtual void Execute(NlContext* sc) override;

private:
	NlGraphPin*			m_inputLhsPin;
	NlGraphPin*			m_inputRhsPin;
	NlGraphPin*			m_outputPin;
};

LN_NAMESPACE_END

