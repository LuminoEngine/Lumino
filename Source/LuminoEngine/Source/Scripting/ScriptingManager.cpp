
#include "../Internal.h"
#include "NlVariant.h"
#include "ScriptingManager.h"

LN_NAMESPACE_BEGIN

//==============================================================================
// NlGraphNode
//==============================================================================

//------------------------------------------------------------------------------
NlGraphNode::NlGraphNode()
	: m_category(NlGraphNodeCategory::Command)
{
}

//------------------------------------------------------------------------------
void NlGraphNode::initialize(NlGraphNodeCategory category)
{
	m_category = category;
}

//------------------------------------------------------------------------------
NlGraphPin* NlGraphNode::CreatePin(NlGraphPinCategory category, NlGraphPinDirection direction, const StringRef& name)
{
	if (category == NlGraphPinCategory::CommandFlow)
	{
		if (direction == NlGraphPinDirection::Input)
		{
			if (LN_CHECK_STATE(m_inputCommandFlowPin == nullptr)) return nullptr;
			m_inputCommandFlowPin = NlGraphPinPtr::makeRef();
			m_inputCommandFlowPin->initialize(this, category, direction);
			return m_inputCommandFlowPin;
		}
		else if (direction == NlGraphPinDirection::Output)
		{
			auto ptr = NlGraphPinPtr::makeRef();
			ptr->initialize(this, category, direction);
			m_outputCommandFlowPinList.add(ptr);
			return ptr;
		}
	}
	else if (category == NlGraphPinCategory::DataFlow)
	{
		if (direction == NlGraphPinDirection::Input)
		{
			auto ptr = NlGraphPinPtr::makeRef();
			ptr->initialize(this, category, direction);
			m_inputDataFlowPinList.add(ptr);
			return ptr;
		}
		else if (direction == NlGraphPinDirection::Output)
		{
			auto ptr = NlGraphPinPtr::makeRef();
			ptr->initialize(this, category, direction);
			m_outputCDataFlowPinList.add(ptr);
			return ptr;
		}
	}

	LN_UNREACHABLE();
	return nullptr;
}

//==============================================================================
// EntryPointNode
//==============================================================================

//------------------------------------------------------------------------------
EntryPointNode::EntryPointNode()
{
}

//------------------------------------------------------------------------------
void EntryPointNode::initialize(const StringRef& name)
{
	m_flowOutput = CreatePin(NlGraphPinCategory::CommandFlow, NlGraphPinDirection::Output, _T("name"));
}

//------------------------------------------------------------------------------
void EntryPointNode::execute(NlContext* sc)
{
	sc->Goto(m_flowOutput);
}

//==============================================================================
// NlGraphPin
//==============================================================================

//------------------------------------------------------------------------------
NlGraphPin::NlGraphPin()
	: m_ownerNode(nullptr)
	, m_category(NlGraphPinCategory::CommandFlow)
	, m_direction(NlGraphPinDirection::Input)
{
}

//------------------------------------------------------------------------------
void NlGraphPin::initialize(NlGraphNode* ownerNode, NlGraphPinCategory category, NlGraphPinDirection direction)
{
	m_ownerNode = ownerNode;
	m_category = category;
	m_direction = direction;
	m_valueCache = RefPtr<NlVariant>::makeRef();
}

//------------------------------------------------------------------------------
NlGraphNode* NlGraphPin::GetLinkedToNode()
{
	if (LN_CHECK_STATE(m_linkedTo.getCount() <= 1)) return nullptr;
	if (m_linkedTo.isEmpty()) return nullptr;
	return m_linkedTo[0]->GetOwnerNode();
}

//------------------------------------------------------------------------------
void NlGraphPin::MakeLinkTo(NlGraphPin* toPin)
{
	if (LN_CHECK_ARG(toPin != nullptr)) return;
	if (LN_CHECK_STATE(!m_linkedTo.contains(toPin))) return;
	if (LN_CHECK_STATE(!toPin->m_linkedTo.contains(this))) return;

	// 双方向接続
	m_linkedTo.add(toPin);
	toPin->m_linkedTo.add(this);
}

//------------------------------------------------------------------------------
void NlGraphPin::BreakLinkTo(NlGraphPin* toPin)
{
	if (LN_CHECK_ARG(toPin != nullptr)) return;

	m_linkedTo.remove(toPin);
	toPin->m_linkedTo.remove(this);
}
//------------------------------------------------------------------------------
NlVariant* NlGraphPin::GetValueCache() const
{
	return m_valueCache;
}

//------------------------------------------------------------------------------
void NlGraphPin::SetInlineValue(NlVariant* value)
{
	if (LN_CHECK_ARG(value != nullptr)) return;
	if (LN_CHECK_STATE(m_category == NlGraphPinCategory::DataFlow)) return;
	if (LN_CHECK_STATE(m_direction == NlGraphPinDirection::Output)) return;
	m_valueCache = value;
}

//------------------------------------------------------------------------------
NlVariant* NlGraphPin::GetInputValue() const
{
	if (LN_CHECK_STATE(m_category == NlGraphPinCategory::DataFlow)) return nullptr;
	if (LN_CHECK_STATE(m_direction == NlGraphPinDirection::Input)) return nullptr;
	if (m_linkedTo.isEmpty())
	{
		return GetValueCache();
	}
	else
	{
		return m_linkedTo[0]->GetValueCache();
	}
}

//==============================================================================
// NlContext
//==============================================================================

//------------------------------------------------------------------------------
NlContext::NlContext()
	: m_pc(nullptr)
	, m_lastExecutePC(nullptr)
{
}

//------------------------------------------------------------------------------
NlVariant* NlContext::evaluate(NlGraphPin* dataInputPin)
{
	NlGraphNode* node = dataInputPin->GetLinkedToNode();
	if (node != nullptr)
	{
		node->execute(this);
	}
	else
	{
		// use default or inline value.
	}
	return dataInputPin->GetInputValue();
}

//------------------------------------------------------------------------------
void NlContext::CallInterface(NlGraphInterface* inter)
{
	GotoNode(inter->GetEntryPoint());

	while (!isCompleted())
	{
		Step();
	}
}

//------------------------------------------------------------------------------
void NlContext::Goto(NlGraphPin* nextFlowPin)
{
	if (LN_CHECK_ARG(nextFlowPin != nullptr)) return;

	m_pc = nullptr;
	NlGraphNode* next = nextFlowPin->GetLinkedToNode();

	// 接続先 pin が無ければ nullptr
	GotoNode(next);
}

//------------------------------------------------------------------------------
bool NlContext::isCompleted() const
{
	return m_pc == nullptr;
}

//------------------------------------------------------------------------------
void NlContext::GotoNode(NlGraphNode* next)
{
	if (next != nullptr)
	{
		if (LN_CHECK_ARG(next->GetCategory() == NlGraphNodeCategory::Command))
		{
			m_pc = nullptr;
			return;
		}
	}

	m_pc = next;
}

//------------------------------------------------------------------------------
void NlContext::Step()
{
	if (LN_CHECK_STATE(m_pc != nullptr)) return;
	m_pc->execute(this);
}

//==============================================================================
// NlGraph
//==============================================================================

//------------------------------------------------------------------------------
void NlGraph::AddGraphNode(NlGraphNode* node)
{
	if (LN_CHECK_ARG(node != nullptr)) return;
	m_nodeList.add(node);
}

//==============================================================================
// NlGraphInterface
//==============================================================================

//------------------------------------------------------------------------------
NlGraphInterface::NlGraphInterface()
	: m_entryPoint(nullptr)
{
}

//------------------------------------------------------------------------------
void NlGraphInterface::initialize()
{
	m_graph = NlGraphPtr::makeRef();

	// 1つのエントリーポイントは必ず存在している
	auto ep = RefPtr<EntryPointNode>::makeRef();
	ep->initialize(_T(""));
	m_graph->AddGraphNode(ep);
	m_entryPoint = ep;
}




//==============================================================================
// NlNode_Print
//==============================================================================

//------------------------------------------------------------------------------
NlNode_Print::NlNode_Print()
{
	m_inputValuePin = CreatePin(NlGraphPinCategory::DataFlow, NlGraphPinDirection::Input, _T("value"));
}

//------------------------------------------------------------------------------
void NlNode_Print::execute(NlContext* sc)
{
	printf("test\n");

	NlVariant* v = sc->evaluate(m_inputValuePin);
	printf("test %d\n", v->getValue<int32_t>());

	sc->Goto(GetFlowOutputPin());
}


namespace detail {

//==============================================================================
// ScriptingManager
//==============================================================================

//------------------------------------------------------------------------------
ScriptingManager::ScriptingManager()
{
}

//------------------------------------------------------------------------------
ScriptingManager::~ScriptingManager()
{
}

//------------------------------------------------------------------------------
void ScriptingManager::initialize(const ConfigData& configData)
{
}


} // namespace detail
LN_NAMESPACE_END
