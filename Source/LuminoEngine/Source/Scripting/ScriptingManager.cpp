
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
NlGraphPin* NlGraphNode::createPin(NlGraphPinCategory category, NlGraphPinDirection direction, const StringRef& name)
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
	m_flowOutput = createPin(NlGraphPinCategory::CommandFlow, NlGraphPinDirection::Output, _LT("name"));
}

//------------------------------------------------------------------------------
void EntryPointNode::execute(NlContext* sc)
{
	sc->gotoPin(m_flowOutput);
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
	m_valueCache = Ref<NlVariant>::makeRef();
}

//------------------------------------------------------------------------------
NlGraphNode* NlGraphPin::getLinkedToNode()
{
	if (LN_CHECK_STATE(m_linkedTo.getCount() <= 1)) return nullptr;
	if (m_linkedTo.isEmpty()) return nullptr;
	return m_linkedTo[0]->getOwnerNode();
}

//------------------------------------------------------------------------------
void NlGraphPin::makeLinkTo(NlGraphPin* toPin)
{
	if (LN_CHECK_ARG(toPin != nullptr)) return;
	if (LN_CHECK_STATE(!m_linkedTo.contains(toPin))) return;
	if (LN_CHECK_STATE(!toPin->m_linkedTo.contains(this))) return;

	// 双方向接続
	m_linkedTo.add(toPin);
	toPin->m_linkedTo.add(this);
}

//------------------------------------------------------------------------------
void NlGraphPin::breakLinkTo(NlGraphPin* toPin)
{
	if (LN_CHECK_ARG(toPin != nullptr)) return;

	m_linkedTo.remove(toPin);
	toPin->m_linkedTo.remove(this);
}
//------------------------------------------------------------------------------
NlVariant* NlGraphPin::getValueCache() const
{
	return m_valueCache;
}

//------------------------------------------------------------------------------
void NlGraphPin::setInlineValue(NlVariant* value)
{
	if (LN_CHECK_ARG(value != nullptr)) return;
	if (LN_CHECK_STATE(m_category == NlGraphPinCategory::DataFlow)) return;
	if (LN_CHECK_STATE(m_direction == NlGraphPinDirection::Output)) return;
	m_valueCache = value;
}

//------------------------------------------------------------------------------
NlVariant* NlGraphPin::getInputValue() const
{
	if (LN_CHECK_STATE(m_category == NlGraphPinCategory::DataFlow)) return nullptr;
	if (LN_CHECK_STATE(m_direction == NlGraphPinDirection::Input)) return nullptr;
	if (m_linkedTo.isEmpty())
	{
		return getValueCache();
	}
	else
	{
		return m_linkedTo[0]->getValueCache();
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
	NlGraphNode* node = dataInputPin->getLinkedToNode();
	if (node != nullptr)
	{
		node->execute(this);
	}
	else
	{
		// use default or inline value.
	}
	return dataInputPin->getInputValue();
}

//------------------------------------------------------------------------------
void NlContext::callInterface(NlGraphInterface* inter)
{
	GotoNode(inter->getEntryPoint());

	while (!isCompleted())
	{
		Step();
	}
}

//------------------------------------------------------------------------------
void NlContext::gotoPin(NlGraphPin* nextFlowPin)
{
	if (LN_CHECK_ARG(nextFlowPin != nullptr)) return;

	m_pc = nullptr;
	NlGraphNode* next = nextFlowPin->getLinkedToNode();

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
void NlGraph::addGraphNode(NlGraphNode* node)
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
	auto ep = Ref<EntryPointNode>::makeRef();
	ep->initialize(_LT(""));
	m_graph->addGraphNode(ep);
	m_entryPoint = ep;
}




//==============================================================================
// NlNode_Print
//==============================================================================

//------------------------------------------------------------------------------
NlNode_Print::NlNode_Print()
{
	m_inputValuePin = createPin(NlGraphPinCategory::DataFlow, NlGraphPinDirection::Input, _LT("value"));
}

//------------------------------------------------------------------------------
void NlNode_Print::execute(NlContext* sc)
{
	printf("test\n");

	NlVariant* v = sc->evaluate(m_inputValuePin);
	printf("test %d\n", v->getValue<int32_t>());

	sc->gotoPin(getFlowOutputPin());
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
